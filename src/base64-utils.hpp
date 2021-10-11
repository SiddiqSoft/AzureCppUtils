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

#ifndef BASE64_UTILS_HPP
#define BASE64_UTILS_HPP


#include <iostream>
#include <string>
#include <concepts>
#include <format>

#if defined(_WIN64) || defined(WIN64) || defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt")
#pragma comment(lib, "crypt32")
#endif


/// @brief SiddiqSoft
namespace siddiqsoft
{
    /// @brief Base64 encode/decode functions
    struct Base64Utils
    {
        /// @brief URL escape the base64 encoded string
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
                if constexpr (std::is_same_v<T, wchar_t>) {
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
        /// @param textuallyEncoded The bytes to encode
        /// @return Base64 encoded string
        /// @remarks This is not a generic be-all implementation. For this, you should really call the underlying function directly
        /// with the necessary options. The implementation here is focussed on meeting the requirements for Azure services.
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> encode(const std::basic_string<T>& argBin)
        {
            DWORD destSize = 0;

            /*
             * NOTE: Cannot fold after the std::invoke() or std::apply() as the signature varies between the two functions.
             * std::invoke( std::is_same_v<T,wchar_t> ? CryptBinaryToStringW : CryptBinaryToStringA, ... )
             * fails to compile since the member types differ and it cannot cast the return into a single type.
             */
            if (!argBin.empty() &&
                (TRUE == std::is_same_v<T, wchar_t> ? CryptBinaryToStringW((const BYTE*)(argBin.data()),
                                                                           static_cast<DWORD>(argBin.length() * sizeof(wchar_t)),
                                                                           CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
                                                                           nullptr,
                                                                           &destSize)
                                                    : CryptBinaryToStringA((const BYTE*)(argBin.data()),
                                                                           static_cast<DWORD>(argBin.length() * sizeof(char)),
                                                                           CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
                                                                           nullptr,
                                                                           &destSize)))
            {
                std::vector<T> dest(static_cast<size_t>(destSize) + 1);

                if (TRUE == std::is_same_v<T, wchar_t> ? CryptBinaryToStringW((const BYTE*)(argBin.data()),
                                                                              static_cast<DWORD>(argBin.length() * sizeof(wchar_t)),
                                                                              CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
                                                                              LPWSTR(dest.data()),
                                                                              &destSize)
                                                       : CryptBinaryToStringA((const BYTE*)(argBin.data()),
                                                                              static_cast<DWORD>(argBin.length() * sizeof(char)),
                                                                              CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
                                                                              LPSTR(dest.data()),
                                                                              &destSize))
                {
                    return std::basic_string<T> {reinterpret_cast<T*>(dest.data()), destSize};
                }
            }

            // Fall-through is failure; return empty string
            return {};
        }


        /// @brief Base64 decode the given encoded string back to the binary value
        /// @param textuallyEncoded Previously encoded value.
        /// @return Base64 decoded string
        /// @remarks This is not a generic be-all implementation. For this, you should really call the underlying function directly
        /// with the necessary options. The implementation here is focussed on meeting the requirements for Azure services.
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> decode(const std::basic_string<T>& textuallyEncoded)
        {
            DWORD destSize = 0;

            if (textuallyEncoded.empty()) return {};

            /*
             * NOTE: Cannot fold after the std::invoke() or std::apply() as the signature varies between the two functions.
             * std::invoke( std::is_same_v<T,wchar_t> ? CryptStringToBinaryW : CryptStringToBinaryA, ... )
             * fails to compile since the member types differ and it cannot cast the return into a single type.
             */
            if (TRUE == std::is_same_v<wchar_t, T>
                        ? CryptStringToBinaryW(LPCWSTR(textuallyEncoded.c_str()),
                                               static_cast<DWORD>(textuallyEncoded.length()), // in character count
                                               CRYPT_STRING_BASE64,
                                               nullptr,   // output; NULL to calculate destSize
                                               &destSize, // this is in bytes
                                               nullptr,
                                               nullptr)
                        : CryptStringToBinaryA(LPCSTR(textuallyEncoded.c_str()),
                                               static_cast<DWORD>(textuallyEncoded.length()), // in character count
                                               CRYPT_STRING_BASE64,
                                               nullptr,   // output; NULL to calculate destSize
                                               &destSize, // this is in bytes
                                               nullptr,
                                               nullptr))
            {
                // The destSize is in bytes.. when we allocate, we must allocated for number of items
                // For the case of wchar_t this would be to divide by sizeof(wchar_t)-->2.
                // The +1 ensures we pad with blank/NUL item.
                std::vector<T> dest(static_cast<size_t>(destSize / sizeof(T)) + 1);

                if (TRUE == std::is_same_v<wchar_t, T> ? CryptStringToBinaryW(LPCWSTR(textuallyEncoded.c_str()),
                                                                              static_cast<DWORD>(textuallyEncoded.length()),
                                                                              CRYPT_STRING_BASE64,
                                                                              reinterpret_cast<BYTE*>(dest.data()),
                                                                              &destSize,
                                                                              nullptr,
                                                                              nullptr)
                                                       : CryptStringToBinaryA(LPCSTR(textuallyEncoded.c_str()),
                                                                              static_cast<DWORD>(textuallyEncoded.length()),
                                                                              CRYPT_STRING_BASE64,
                                                                              reinterpret_cast<BYTE*>(dest.data()),
                                                                              &destSize,
                                                                              nullptr,
                                                                              nullptr))
                {
                    // The destSize is byte count and to get the character count we must divide by the sizeof(T) to obtain the
                    // correct character/string size.
                    return std::basic_string<T> {reinterpret_cast<T*>(dest.data()), destSize / sizeof(T)};
                }
            }
            else {
                throw std::runtime_error(std::format("{} - CryptStringToBinary returned:{}", __func__, GetLastError()));
            }

            // Fall-through is failure; return empty string
            return {};
        }
    };
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
