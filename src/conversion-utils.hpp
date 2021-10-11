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

#ifndef CONVERSION_UTILS_HPP
#define CONVERSION_UTILS_HPP


#include <chrono>
#include <string>
#include <ranges>
#include <concepts>
#include <format>

#if defined(_WIN64) || defined(WIN64) || defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#endif


/// @brief SiddiqSoft
namespace siddiqsoft
{
    /// @brief Conversion Functions for ascii to wide, utf8 to wide and vice-versa
    struct ConversionUtils
    {
        /// @brief Convert given wide string to ascii encoded string
        /// @param src std::wstring input
        /// @return std::string with ascii encoding
        static std::string asciiFromWide(const std::wstring& src)
        {
            if (src.empty()) return {};

            if (auto destSize =
                        WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0, nullptr, nullptr);
                destSize > 0)
            {
                // Allocate appropriate buffer +1 for null-char
                std::vector<char> destBuffer(static_cast<size_t>(destSize) + 1);
                destSize = WideCharToMultiByte(CP_ACP,
                                               0,
                                               src.c_str(),
                                               static_cast<int>(src.length()),
                                               destBuffer.data(),
                                               static_cast<DWORD>(destSize),
                                               nullptr,
                                               nullptr);
                return {destBuffer.data(), static_cast<size_t>(destSize)};
            }

            // Fall-through error -> empty string
            return {};
        }

        /// @brief Convert given wide string to utf8 encoded string
        /// @param src std::wstring input
        /// @return std::string with utf-8 encoding
        static std::string utf8FromWide(const std::wstring& src)
        {
            if (src.empty()) return {};

            if (auto destSize =
                        WideCharToMultiByte(CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0, nullptr, nullptr);
                destSize > 0)
            {
                // Allocate appropriate buffer +1 for null-char
                std::vector<char> destBuffer(static_cast<size_t>(destSize) + 1);
                destSize = WideCharToMultiByte(CP_UTF8,
                                               0,
                                               src.c_str(),
                                               static_cast<int>(src.length()),
                                               destBuffer.data(),
                                               static_cast<DWORD>(destSize),
                                               nullptr,
                                               nullptr);
                return {destBuffer.data(), static_cast<size_t>(destSize)};
            }

            // Fall-through error -> empty string
            return {};
        }

        /// @brief Given a utf-8 encoded string returns a utf-16 in std::wstring
        /// @param src Utf-8 encoded string
        /// @return Utf-16 encoded wstring
        static std::wstring wideFromUtf8(const std::string& src)
        {
            if (src.empty()) return {};

            if (auto destSize = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0);
                destSize > 0) {
                // Allocate appropriate buffer +1 for null-char
                std::vector<wchar_t> destBuffer(static_cast<size_t>(destSize) + 1);
                destSize = MultiByteToWideChar(
                        CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), destBuffer.data(), static_cast<DWORD>(destSize));
                return {destBuffer.data(), static_cast<size_t>(destSize)};
            }

            // Fall-through error -> empty string
            return {};
        }


        /// @brief Given an ascii encoded string returns a utf-16 in std::wstring
        /// @param src ascii encoded string
        /// @return Utf-16 encoded wstring
        static std::wstring wideFromAscii(const std::string& src)
        {
            if (src.empty()) return {};

            if (auto destSize = MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), nullptr, 0);
                destSize > 0) {
                // Allocate appropriate buffer +1 for null-char
                std::vector<wchar_t> destBuffer(static_cast<size_t>(destSize) + 1);
                destSize = MultiByteToWideChar(
                        CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), destBuffer.data(), static_cast<DWORD>(destSize));
                return {destBuffer.data(), static_cast<size_t>(destSize)};
            }

            // Fall-through error -> empty string
            return {};
        }
    };
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
