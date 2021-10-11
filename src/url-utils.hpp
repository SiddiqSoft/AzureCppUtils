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

#ifndef URL_UTILS_HPP
#define URL_UTILS_HPP


#include <string>
#include <concepts>
#include <format>

/// @brief SiddiqSoft
namespace siddiqsoft
{
#if !defined(_NORW)
    /// @brief In support of the macro NORW which allows us to declare/use narrow/wide strings as needed. Plucked from the MS stl
    /// implementation
    template <typename _NorWT>
        requires std::same_as<_NorWT, char> || std::same_as<_NorWT, wchar_t>
    [[nodiscard]] constexpr const _NorWT* NorW_1(const char* const _Str, const wchar_t* const _WStr) noexcept
    {
        if constexpr (std::is_same_v<_NorWT, char>) {
            return _Str;
        }
        else {
            return _WStr;
        }
    }
#define _NORW(_NorWT, _Literal) NorW_1<_NorWT>(_Literal, L##_Literal)
#endif

    /// @brief Url encode function
    struct UrlUtils
    {
        /// @brief Helper to encode the given string in context of the HTTP url
        /// @param source Source string
        /// @param lowerCase Default false --> all conversions to uppercase
        /// @return Encoded string with uppercase replacement
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> encode(const std::basic_string<T>& source, bool lowerCase = false)
        {
            std::basic_string<T> retOutput {};

            std::ranges::for_each(source, [&retOutput, &lowerCase](auto ch) {
                switch (ch) {
                    case 0x20:
                    case 0x22:
                    case 0x23:
                    case 0x24:
                    case 0x25:
                    case 0x26:
                    case 0x27:
                    case 0x3c:
                    case 0x3e:
                    case 0x7b:
                    case 0x7d:
                    case 0x2f:
                    case 0x5c:
                    case 0x40:
                    case 0x7e:
                    case 0x7c:
                    case 0x2c:
                    case 0x2b:
                    case 0x3a:
                    case 0x5b:
                    case 0x5d:
                    case 0x3f:
                    case 0x3d:
                    case 0x60:
                        std::format_to(std::back_inserter(retOutput), lowerCase ? _NORW(T, "%{:02x}") : _NORW(T, "%{:02X}"), ch);
                        break;
                    default: std::format_to(std::back_inserter(retOutput), _NORW(T, "{}"), ch);
                };
            });

            return retOutput;
        }
    };
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
