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

#ifndef DATE_UTILS_HPP
#define DATE_UTILS_HPP


#include <iostream>
#include <chrono>
#include <string>
#include <concepts>
#include <format>



/// @brief SiddiqSoft
namespace siddiqsoft
{
    /// @brief Date Time utilities for REST API
    struct DateUtils
    {
        /// @brief Converts the argument to ISO8601 format
        /// @tparam T Must be string or wstring
        /// @param rawtp time_point representing the time. Defaults to "now"
        /// @return String representing ISO 8601 format with milliseconds empty if fails
        /// @see The specification https://en.wikipedia.org/wiki/ISO_8601
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> ISO8601(const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now())
        {
            auto rawtime = std::chrono::system_clock::to_time_t(rawtp);
            tm   timeInfo {};
            // We need to get the fractional milliseconds from the raw time point.
            auto msTime = std::chrono::duration_cast<std::chrono::milliseconds>(rawtp.time_since_epoch()).count() % 1000;
            // Get the UTC time packet.
            auto ec = gmtime_s(&timeInfo, &rawtime);

            if constexpr (std::is_same_v<T, char>) {
                // https://en.wikipedia.org/wiki/ISO_8601
                // yyyy-mm-ddThh:mm:ss.mmmZ
                std::vector<char> buff(32, 0);

                if (ec != EINVAL) strftime(buff.data(), buff.capacity(), "%FT%T", &timeInfo);
                return std::format("{}.{:03}Z", buff.data(), msTime);
            }
            else if constexpr (std::is_same_v<T, wchar_t>) {
                // yyyy-mm-ddThh:mm:ss.mmmZ
                std::vector<wchar_t> buff(32, 0);
                if (ec != EINVAL) wcsftime(buff.data(), buff.capacity(), L"%FT%T", &timeInfo);
                return std::format(L"{}.{:03}Z", buff.data(), msTime);
            }

            return {};
        }


        /// @brief Build a time and date string compliant with the RFC7231
        /// @tparam T Must be either std::string or std::wstring
        /// @param rawtp Optional. Uses current time if not provided
        /// @return Returns a string representation of the form: "Tue, 01 Nov 1994 08:12:31 GMT"
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> RFC7231(const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now())
        {
            auto rawtime = std::chrono::system_clock::to_time_t(rawtp);
            tm   timeInfo {};

            // Get the UTC time packet.
            auto ec = gmtime_s(&timeInfo, &rawtime);

            // HTTP-date as per RFC 7231:  Tue, 01 Nov 1994 08:12:31 GMT
            // Note that since we are getting the UTC time we should not use the %z or %Z in the strftime format
            // as it returns the local timezone and not GMT.
            if constexpr (std::is_same_v<T, char>) {
                std::vector<char> buff(32, 0);
                if (ec != EINVAL) strftime(buff.data(), buff.capacity(), "%a, %d %h %Y %T GMT", &timeInfo);

                return buff.data();
            }

            if constexpr (std::is_same_v<T, wchar_t>) {
                std::vector<wchar_t> buff(32, 0);
                if (ec != EINVAL) wcsftime(buff.data(), buff.capacity(), L"%a, %d %h %Y %T GMT", &timeInfo);

                return buff.data();
            }
        }
    };
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
