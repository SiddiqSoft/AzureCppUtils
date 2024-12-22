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

#include <cstdint>
#include <memory>
#include <type_traits>
#ifndef BASE64_UTILS_UNIX_HPP
#define BASE64_UTILS_UNIX_HPP

#if defined(__linux__) || defined(__APPLE__)

#include <iostream>
#include <string>
#include <concepts>
#include <format>
#include <algorithm>
#include <ranges>

#include "openssl/ssl.h"
#include "openssl/crypto.h"
#include "openssl/evp.h"


/// @brief SiddiqSoft
namespace siddiqsoft
{
    /// @brief Base64 encode/decode functions
    struct Base64Utils
    {
        /// @brief URL escape the base64 encoded string
        ///        This is *not* the same as URL-encoding a string!
        /// @tparam T std::string or std::wstring
        /// @param src The source MUST be a base64 encoded string
        /// @return url encoded base64 source string
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> urlEscape(const std::basic_string<T>& src)
        {
            if (!src.empty()) {
                auto encodeVal {src};

                // Make the value url-safe per https://tools.ietf.org/html/rfc4648#section-5
                if constexpr (std::is_same_v<T, char>) {
                    std::ranges::for_each(encodeVal, [](T& ch) {
                        if (ch == '+')
                            ch = '-';
                        else if (ch == '/')
                            ch = '_';
                    });
                    encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '\r'), encodeVal.end());
                    encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '\n'), encodeVal.end());
                    encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '='), encodeVal.end());

                    return encodeVal;
                }
                else if constexpr (std::is_same_v<T, wchar_t>) {
                    std::ranges::for_each(encodeVal, [](T& ch) {
                        if (ch == L'+')
                            ch = L'-';
                        else if (ch == L'/')
                            ch = L'_';
                    });
                    encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), L'\r'), encodeVal.end());
                    encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), L'\n'), encodeVal.end());
                    encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), L'='), encodeVal.end());

                    return encodeVal;
                }
            }

            return {};
        }


        /// @brief Base64 encode a given "binary" string and optionally url escape
        /// @param argBin The bytes to encode
        /// @return Base64 encoded string
        /// @remarks This is not a generic be-all implementation. For this, you should really call the underlying function directly
        /// with the necessary options. The implementation here is focussed on meeting the requirements for Azure services.
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> encode(const std::basic_string<T>& source)
        {
            if constexpr (std::is_same_v<T, char>) {
                // The OpenSSL interface only supports char/utf-8
                uint32_t destSize = 0;
                // Overallocate the destination buffer this is especially true for utf-8 contents
                std::basic_string<T> dest(4 * source.length(), 0);

                if (destSize = EVP_EncodeBlock(reinterpret_cast<unsigned char*>(dest.data()),
                                               reinterpret_cast<const unsigned char*>(source.data()),
                                               source.length());
                    destSize > 0)
                {
                    dest.resize(destSize);
                    return dest;
                }
            }
            else {
                // Convert the source to char from wchar_t
                // encode as char
                // Then encode the result back to the wchar_t
                return ConversionUtils::convert_to<char, wchar_t>(encode<char>(ConversionUtils::convert_to<wchar_t, char>(source)));
            }

            // Fall-through is failure; return empty string
            return std::basic_string<T> {};
        }


        /// @brief Base64 decode the given encoded string back to the binary value
        /// @param textuallyEncoded Previously encoded value.
        /// @return Base64 decoded string
        /// @remarks This is not a generic be-all implementation. For this, you should really call the underlying function directly
        /// with the necessary options. The implementation here is focussed on meeting the requirements for Azure services.
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> decode(const std::basic_string<T>& source)
        {
            if constexpr (std::is_same_v<T, char>) {
                // The OpenSSL interface only supports char/utf-8
                uint32_t destSize = 0;
                // Going from encoded to normal utf results in a "smaller" destination so we
                // will leave the reservation to same length as the source and resize
                std::basic_string<T> dest(source.length(), 0);

                if (destSize = EVP_DecodeBlock(reinterpret_cast<unsigned char*>(dest.data()),
                                               reinterpret_cast<const unsigned char*>(source.data()),
                                               source.length());
                    destSize > 0)
                {
                    // Remove/trim the padded \0 at the end
                    // Start from the end and if we see a \0 then decrease the destSize
                    while ((destSize > 0) && dest.at(destSize) == '\0') {
                        if (destSize > 0 && dest.at(destSize - 1) != '\0')
                            break;
                        else
                            destSize--;
                    };

                    dest.resize(destSize);
                    return dest;
                }
            }
            else {
                // Convert the source to char from wchar_t
                // encode as char
                // Then encode the result back to the wchar_t
                return ConversionUtils::convert_to<char, wchar_t>(decode<char>(ConversionUtils::convert_to<wchar_t, char>(source)));
            }

            // Fall-through is failure; return empty string
            return {};
        }
    };
} // namespace siddiqsoft
#else
// This file may be used for Windows implementations or you may use the native implementation on Windows
#endif

#endif // !AZURECPPUTILS_HPP
