/*
    AzureCppUtils : Azure REST API Utilities for Modern C++

    BSD 3-Clause License

    Copyright (c) 2021, Siddiq Software
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, d_, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#ifndef ENCRYPTION_UTILS_UNIX_HPP
#define ENCRYPTION_UTILS_UNIX_HPP

#if defined(__linux__) || defined(__APPLE__)
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <string>
#include <functional>
#include <memory>
#include <ranges>
#include <concepts>
#include <format>

#include "siddiqsoft/conversion-utils.hpp"
#include "base64-utils.hpp"
#include "url-utils.hpp"
#include "siddiqsoft/RunOnEnd.hpp"

#include "openssl/evp.h"
#include "openssl/md5.h"
#include "openssl/hmac.h"
#include "openssl/params.h"
#include "openssl/err.h"


/// @brief SiddiqSoft
namespace siddiqsoft
{
    /**
     * @brief Encryption utility functions for ServiceBus, Cosmos, EventGrid, EventHub
     *        Implementation Note!
     *        The support for wstring is for completeness and typically the use-case is where we
     *        fiddle with utf8 data and not utf16 over the internet and especially json documents!
     */
    struct EncryptionUtils
    {
        /**
         * @brief Calculate digest MD4, MD5
         *
         * @param digestType "MD5" or "MD4"
         * @param source The source string to calculate the digest
         * @return std::string returns a string containing the digest as a sequence of hex characters.
         */
        static std::string calcDigest(const std::string& digestType, const std::string& source)
        {
            std::string result;

            if (!source.empty() && ((digestType.find("MD5") == 0) || (digestType.find("MD4") == 0))) {
                if (const auto digestAlgorithm = EVP_get_digestbyname(digestType.c_str()); digestAlgorithm != NULL) {
                    std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), &EVP_MD_CTX_free);

                    if (EVP_DigestInit_ex2(ctx.get(), digestAlgorithm, NULL)) {
                        if (EVP_DigestUpdate(ctx.get(), source.c_str(), source.length())) {
                            unsigned char digestValue[EVP_MAX_MD_SIZE];
                            unsigned int  digestValueLength = 0;

                            if (EVP_DigestFinal_ex(ctx.get(), digestValue, &digestValueLength)) {
                                // encode and return..
                                for (int i = 0; i < digestValueLength; i++) {
                                    std::format_to(std::back_inserter(result), "{:02x}", digestValue[i]);
                                }

                                return result;
                            }
                        }
                    }
                }
                else {
                    throw std::runtime_error(std::format("Unknown or unsupported `{}` digest type.", digestType).c_str());
                }
            }

            return result;
        }


        /**
         * @brief Create a MD5 hash for the given source as a string
         *
         * @tparam T char or wchar_t
         * @param source Maybe std::string or std::wstring
         * @return  MD5 of the source argument empty if there is a failure
         */
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::string MD5(const std::basic_string<T>& source)
        {
            // Convert from wchar_t or we return as-is for the conversion. The MD5 works on the
            // utf8 character set.
            return EncryptionUtils::calcDigest("MD5", ConversionUtils::convert_to<T, char>(source));
        }


        /**
         * @brief Returns binary HMAC using SHA-256.
         *        https://www.liavaag.org/English/SHA-Generator/HMAC/
         * @tparam T Can be char or wchar_t
         * @param message The message to generate the HMAC
         * @param key The key for the given digest generation
         * @return Binary enclosed in string; you must base64 encode.
         */
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::string HMAC(const std::basic_string<T>& message, const std::string& key)
        {
            std::string result;

            if constexpr (std::is_same_v<T, char>) {
                if (!message.empty() && !key.empty()) {
                    if (const auto digestAlgorithm = EVP_get_digestbyname("SHA256"); digestAlgorithm != NULL) {
                        std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), &EVP_MD_CTX_free);


                        unsigned char digestValue[EVP_MAX_MD_SIZE];
                        unsigned int  digestValueLength = 0;

                        if (auto rc = ::HMAC(digestAlgorithm,
                                             key.c_str(),
                                             key.length(),
                                             reinterpret_cast<const unsigned char*>(message.c_str()),
                                             message.length(),
                                             digestValue,
                                             &digestValueLength);
                            rc != NULL)
                        {
                            return std::string(reinterpret_cast<char*>(digestValue), digestValueLength);
                        }
                    }
                    else {
                        throw std::runtime_error(std::format("Unknown or unsupported  digest type.").c_str());
                    }
                }
            }
            else {
                return HMAC(ConversionUtils::convert_to<T, char>(message), key);
            }

            return result;
        }


        /// @brief Create a JsonWebToken authorization with HMAC 256
        /// @param key Must be std::string as the contents are the "key" and treated as "binary"
        /// @param header The JWT header
        /// @param payload The string wiht json tokens
        /// @return HMAC256 encoded JWT token
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T>
        JWTHMAC256(const std::string& key, const std::basic_string<T>& header, const std::basic_string<T>& payload)
        {
            if constexpr (std::is_same_v<T, char>) {
                auto s1        = Base64Utils::urlEscape<char>(Base64Utils::encode<char>(header));
                auto s2        = Base64Utils::urlEscape<char>(Base64Utils::encode<char>(payload));
                auto a3        = std::format("{}.{}", s1, s2);
                auto a4        = HMAC<char>(a3, key);
                auto signature = Base64Utils::urlEscape<char>(Base64Utils::encode<char>(a4));

                return std::format("{}.{}.{}", s1, s2, signature);
            }
            else {
                // Delegate to the narrow version; conversion at the edge
                return ConversionUtils::convert_to<char, T>(JWTHMAC256<char>(
                        key, ConversionUtils::convert_to<T, char>(header), ConversionUtils::convert_to<T, char>(payload)));
            }
        }


        /// @brief Create a Shared Access Signature for Azure storage
        /// https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token
        /// @param key The key is "binary" in std::string
        /// @param url The url for the session
        /// @param keyName The key name
        /// @param timeout Amount of ticks past the "now" (number of seconds since 1970-1-1)
        /// @return SAS token
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> SASToken(const std::string&          key,
                                             const std::basic_string<T>& url,
                                             const std::basic_string<T>& keyName,
                                             const std::chrono::seconds& timeout)
        {
            time_t epoch {};

            time(&epoch); // number of seconds since 1970-1-1

            if constexpr (std::is_same_v<T, wchar_t>) {
                return SASToken<wchar_t>(key, url, keyName, std::to_wstring(int64_t(epoch) + timeout.count()));
            }
            else if constexpr (std::is_same_v<T, char>) {
                return SASToken<char>(key, url, keyName, std::to_string(int64_t(epoch) + timeout.count()));
            }
        }


        /// @brief Create a Shared Access Signature for Azure storage
        /// https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token
        /// @param key The key to sign the url with time
        /// @param url The url for the session
        /// @param keyName The key name
        /// @param expiry The expiry is the number of seconds since 1970-1-1 plus timeout
        /// @return SAS token
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> SASToken(const std::string&          key,
                                             const std::basic_string<T>& url,
                                             const std::basic_string<T>& keyName,
                                             const std::basic_string<T>& expiry)
        {
            if (url.empty()) throw std::invalid_argument("SASToken: url may not be empty");
            if (keyName.empty()) throw std::invalid_argument("SASToken: keyName may not be empty");
            if (key.empty()) throw std::invalid_argument("SASToken: key may not be empty");
            if (expiry.empty()) throw std::invalid_argument("SASToken: expiry may not be empty");

            if constexpr (std::is_same_v<T, char>) {
                auto s1    = UrlUtils::encode<char>(url, true); // lowercase
                auto sig   = HMAC<char>(std::format("{}\n{}", s1, expiry), key);
                auto esign = Base64Utils::encode<char>(sig);
                esign      = UrlUtils::encode<char>(esign, true); // lowercase

                return std::format("SharedAccessSignature sr={}&sig={}&se={}&skn={}", s1, esign, expiry, keyName);
            }
            else {
                // Delegate to the narrow version and convert at the edges.
                return ConversionUtils::convert_to<char, T>(SASToken<char>(key,
                                                                           ConversionUtils::convert_to<T, char>(url),
                                                                           ConversionUtils::convert_to<T, char>(keyName),
                                                                           ConversionUtils::convert_to<T, char>(expiry)));
            }
        }


        /// @brief Create the Cosmos Authorization Token using the Key for this connection.
        /// @param key Binary. The key must be decoded from the base64 value in the connection string from the Azure portal
        /// @param verb GET, POST, PUT, DELETE
        /// @param type One of the following: dbs, docs, colls, attachments or empty
        /// @param resourceLink The resource link sub-uri
        /// @param date Date in RFC7231 as string
        /// @return Cosmos Authorization signature as std::string. It is base64 encoded and urlsafe
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> CosmosToken(const std::string&          key,
                                                const std::basic_string<T>& verb,
                                                const std::basic_string<T>& type,
                                                const std::basic_string<T>& resourceLink,
                                                const std::basic_string<T>& date)
        {
            if (key.empty()) throw std::invalid_argument("CosmosToken: key may not be empty");
            if (date.empty()) throw std::invalid_argument("CosmosToken: date may not be empty");
            if (verb.empty()) throw std::invalid_argument("CosmosToken: verb may not be empty");

            if constexpr (std::is_same_v<T, char>) {
                // The formula is expressed as per
                // https://docs.microsoft.com/en-us/rest/api/documentdb/access-control-on-documentdb-resources?redirectedfrom=MSDN
                std::string strToHash {};

                std::ranges::transform(verb, std::back_inserter(strToHash), [](auto& ch) { return std::tolower(ch); });
                std::format_to(std::back_inserter(strToHash), "\n");
                std::ranges::transform(type, std::back_inserter(strToHash), [](auto& ch) { return std::tolower(ch); });
                std::format_to(std::back_inserter(strToHash), "\n{}\n", resourceLink);
                std::ranges::transform(date, std::back_inserter(strToHash), [](auto& ch) { return std::tolower(ch); });
                std::format_to(std::back_inserter(strToHash), "\n\n");

                if (!strToHash.empty()) {
                    // Sign using SHA256 using the master key and base64 encode. force lowercase
                    if (auto hmacBase64UrlEscaped = UrlUtils::encode<char>(
                                Base64Utils::encode<char>(EncryptionUtils::HMAC<char>(strToHash, key)), true);
                        !hmacBase64UrlEscaped.empty())
                    {
                        return std::format("type%3dmaster%26ver%3d1.0%26sig%3d{}", hmacBase64UrlEscaped);
                    }
                }
            }
            else {
                // Delegate to the narrow version, conversion at the edges.
                return ConversionUtils::convert_to<char, T>(CosmosToken<char>(key,
                                                                              ConversionUtils::convert_to<T, char>(verb),
                                                                              ConversionUtils::convert_to<T, char>(type),
                                                                              ConversionUtils::convert_to<T, char>(resourceLink),
                                                                              ConversionUtils::convert_to<T, char>(date)));
            }

            // Fall-through failure
            return {};
        }
    };
} // namespace siddiqsoft
#else
// Not the file you're looking for
#endif

#endif