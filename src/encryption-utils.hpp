/*
    azure-cpp-utils : Azure REST API Utilities for Modern C++

    BSD 3-Clause License

    Copyright (c) 2021, Siddiq Software LLC
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

#ifndef ENCRYPTION_UTILS_HPP
#define ENCRYPTION_UTILS_HPP


#include <algorithm>
#include <iostream>
#include <chrono>
#include <string>
#include <functional>
#include <memory>
#include <ranges>
#include <concepts>
#include <format>

#if defined(_WIN64) || defined(WIN64) || defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt")
#pragma comment(lib, "crypt32")
#endif


#include "base64-utils.hpp"
#include "url-utils.hpp"
#include "siddiqsoft/RunOnEnd.hpp"


/// @brief SiddiqSoft
namespace siddiqsoft
{
    /// @brief Encryption utility functions for ServiceBus, Cosmos, EventGrid, EventHub
    /// Implementation Note!
    /// The support for wstring is for completeness and typically the use-case is where we
    /// fiddle with utf8 data and not utf16 over the internet and especially json documents!
    struct EncryptionUtils
    {
        /// @brief Create a MD5 hash for the given source as a string
        /// @param source Maybe std::string or std::wstring
        /// @return MD5 of the source argument empty if there is a failure
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::string MD5(const std::basic_string<T>& source)
        {
            if constexpr (std::is_same_v<T, char>) {
                HCRYPTPROV hProv {};
                HCRYPTHASH hHash {};
                RunOnEnd   cleanUpOnEnd {[&hProv, &hHash] {
                    // Cleanup on exit of this function scope
                    if (hHash != NULL) CryptDestroyHash(hHash);
                    if (hProv != NULL) CryptReleaseContext(hProv, 0);
                }};

                // Get handle to the crypto provider
                if (TRUE == CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
                    // Get the hash library, choose MD5..
                    if (TRUE == CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
                        // Hash the source..
                        if (TRUE ==
                            CryptHashData(
                                    hHash, reinterpret_cast<const BYTE*>(source.data()), static_cast<DWORD>(source.length()), 0)) {
                            const char rgbDigits[] {"0123456789abcdef"};
                            BYTE       rgbHash[sizeof(rgbDigits)] {};
                            DWORD      rgbHashSize = sizeof(rgbDigits);
                            // Fetch the results using the gethashparam call..
                            if (TRUE == CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &rgbHashSize, 0)) {
                                std::string result {};

                                for (DWORD i = 0; i < rgbHashSize; i++) {
                                    std::format_to(std::back_inserter(result),
                                                   "{}{}",
                                                   rgbDigits[rgbHash[i] >> 4],
                                                   rgbDigits[rgbHash[i] & 0xf]);
                                }

                                return result;
                            }
                        }
                    }
                }
            }
            else {
                // The MD5 result is a "binary" so we must not try and convert.
                return MD5<char>(ConversionUtils::utf8FromWide(source));
            }

            // Fall-through failure
            return {};
        }


        /// @brief Returns binary HMAC using SHA-256.
        /// https://www.liavaag.org/English/SHA-Generator/HMAC/
        /// @param message Source text
        /// @param key Source key; MUST NOT be base64 encoded
        /// @return Binary enclosed in string (narrow); you must base64encode to get useful result.
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::string HMAC(const std::basic_string<T>& message, const std::string& key)
        {
            if constexpr (std::is_same_v<T, char>) {
                BCRYPT_ALG_HANDLE  hAlg {};
                BCRYPT_HASH_HANDLE hHash {};
                NTSTATUS           status {0};
                RunOnEnd           cleanupOnEnd {[&hAlg, &hHash] {
                    // All handles we allocate are cleaned up when this function returns to caller
                    if (hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
                    if (hHash) BCryptDestroyHash(hHash);
                }};


                if (status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, nullptr, BCRYPT_ALG_HANDLE_HMAC_FLAG);
                    status == 0) {
                    // Set the key for the hash function..
                    // Passing NULL, 0 to the pbHashObject and cbHashObject asks the method to allocate
                    // memory on our behalf.
                    if (status = BCryptCreateHash(hAlg,
                                                  &hHash,
                                                  nullptr,
                                                  0,
                                                  reinterpret_cast<UCHAR*>(const_cast<char*>(key.data())),
                                                  static_cast<DWORD>(key.length()),
                                                  0);
                        status == 0)
                    {
                        // Let's hash our message!
                        if (status = BCryptHashData(hHash,
                                                    reinterpret_cast<UCHAR*>(const_cast<char*>(message.data())),
                                                    static_cast<DWORD>(message.length()),
                                                    0);
                            status == 0)
                        {
                            // Get the size of the hash so we can fetch it..
                            DWORD cbHash {0};
                            ULONG cbData {0};
                            if (status = BCryptGetProperty(
                                        hAlg, BCRYPT_HASH_LENGTH, reinterpret_cast<UCHAR*>(&cbHash), sizeof(DWORD), &cbData, 0);
                                status == 0) {
                                std::vector<BYTE> pbHash(cbHash);
                                // Fetch the hash value
                                if (status = BCryptFinishHash(hHash, reinterpret_cast<UCHAR*>(pbHash.data()), cbHash, 0);
                                    status == 0) {
                                    // Return the HMAC as a raw binary..client must choose to encode or leave as-is
                                    return std::string {reinterpret_cast<char*>(pbHash.data()), cbHash};
                                }
                            }
                        }
                    }
                }
            }
            else {
                // The HMAC result is "binary" and must not be treated as wstring
                return HMAC<char>(ConversionUtils::asciiFromWide(message), key);
            }

            // Fall-through is failure
            return {};
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
                return ConversionUtils::wideFromUtf8(
                        JWTHMAC256<char>(key, ConversionUtils::utf8FromWide(header), ConversionUtils::utf8FromWide(payload)));
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

            return SASToken<T>(key,
                               url,
                               keyName,
                               std::is_same_v<T, wchar_t> ? std::to_wstring(int64_t(epoch) + timeout.count())
                                                          : std::to_string(int64_t(epoch) + timeout.count()));
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
                return ConversionUtils::wideFromUtf8(SASToken<char>(key,
                                                                    ConversionUtils::utf8FromWide(url),
                                                                    ConversionUtils::utf8FromWide(keyName),
                                                                    ConversionUtils::utf8FromWide(expiry)));
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
                return ConversionUtils::wideFromAscii(CosmosToken<char>(key,
                                                                        ConversionUtils::asciiFromWide(verb),
                                                                        ConversionUtils::asciiFromWide(type),
                                                                        ConversionUtils::asciiFromWide(resourceLink),
                                                                        ConversionUtils::asciiFromWide(date)));
            }

            // Fall-through failure
            return {};
        }
    };
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
