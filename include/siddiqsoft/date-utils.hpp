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
            const auto rawtime = std::chrono::system_clock::to_time_t(rawtp);
            tm         timeInfo {};
            // We need to get the fractional milliseconds from the raw time point.
            auto msTime = std::chrono::duration_cast<std::chrono::milliseconds>(rawtp.time_since_epoch()).count() % 1000;
            // Get the UTC time packet.
            const auto ec = gmtime_s(&timeInfo, &rawtime);

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


        /// @brief Returns D.HH:MM:SS ; days.hours:minutes:seconds
        /// @tparam T Must be either std::string or std::wstring
        /// @param arg Number of seconds
        /// @return Returns a string representation of the form: "days.hours:minutes:seconds"
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> toTimespan(const std::chrono::seconds& arg)
        {
            auto asSeconds = arg.count();
            // https://www.epochconverter.com/
            // Human-readable time 	Seconds
            // 1 hour				3600 seconds
            // 1 day				86400 seconds
            // 1 week				604800 seconds
            // 1 month (30.44 days)	2629743 seconds
            // 1 year (365.24 days)	31556926 seconds
            auto hours   = (asSeconds / 3600) % 24;
            auto days    = (asSeconds / 86400);
            auto months  = (asSeconds / 2629743);
            auto years   = asSeconds / 31556926;
            auto weeks   = (asSeconds / 604800);
            auto minutes = (asSeconds / 60) % 60;
            auto seconds = asSeconds % 60;

            if constexpr (std::is_same_v<T, char>)
                return std::format("{}.{:02}:{:02}:{:02}", days, hours, minutes, seconds);
            else if constexpr (std::is_same_v<T, wchar_t>)
                return std::format(L"{}.{:02}:{:02}:{:02}", days, hours, minutes, seconds);
        }


        /// @brief Converts the epoch time into a time_point. The reason for string is due to the permissibility of epoch with
        /// decimals where the portion after the decimal represents milliseconds.
        /// @tparam T Must be either std::string or std::wstring or as uint64_t
        /// @param arg A string or wstring
        /// @return time_point
        template <class T = std::string>
            requires std::same_as<T, std::string> || std::same_as<T, std::wstring> || std::same_as<T, uint64_t>
        static std::chrono::system_clock::time_point parseEpoch(const T& arg)
        {
            tm                                    epoch1tm {};
            uint64_t                              epoch1ntp {0};
            uint64_t                              epoch1millis {0};
            __time64_t                            epoch1 {0};
            std::chrono::system_clock::time_point ret_tp {};

            // Convert the argument to unsigned long; this will drop the decimal portion if persent
            // and yields the number of seconds since epoch.
            if constexpr (std::is_same_v<T, uint64_t>)
                epoch1ntp = arg;
            else if constexpr (std::is_same_v<T, std::string>) {
                epoch1ntp = std::stoull(arg.data());
                // Check if we have high-resolution part.
                if (auto locMilli = arg.find("."); locMilli != std::string::npos) {
                    epoch1millis = std::stoull(arg.substr(locMilli + 1).data());
                    epoch1millis *= 1000000; // offset to allow us to avoid the decimals
                    epoch1millis >>= 32;     // divide by 2^32 => milliseconds.
                }
            }
            else if constexpr (std::is_same_v<T, std::wstring>) {
                epoch1ntp = std::stoull(arg.data());
                // Check if we have high-resolution part.
                if (auto locMilli = arg.find(L"."); locMilli != std::string::npos) {
                    epoch1millis = std::stoull(arg.substr(locMilli + 1).data());
                    epoch1millis *= 1000000; // offset to allow us to avoid the decimals
                    epoch1millis >>= 32;     // divide by 2^32 => milliseconds.
                }
            }

            // Guard against empty argument
            if (epoch1ntp > 0) {
                // Just in case, we should handle the NTP and the epoch case..
                // The EPOC time is from Jan 1 1970 whereas NTP starts from 1/1/1900 which necessitates this subtraction
                epoch1 = (epoch1ntp > 2208988800ULL) ? epoch1ntp - 2208988800ULL : epoch1ntp;

                // Convert to tm structure
                _gmtime64_s(&epoch1tm, &epoch1);

                // Create the timepoint
                ret_tp = std::chrono::system_clock::from_time_t(epoch1);
                // Add the milliseconds
                ret_tp += std::chrono::milliseconds(epoch1millis);
            }

            return ret_tp;
        }


        /// @brief Returns a tuple where the first is the chrono::duration<Z> and the second is std::basic_string<T> as
        /// @tparam T Output type (char or wchar_t)
        /// @param end time_point of end
        /// @param start time_point of start
        /// @return Returns a tuple where the first is the chrono::duration<Z> and the second is std::basic_string<T> as
        /// "HH:MM:SS.mmm"
        template <typename T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::tuple<std::chrono::milliseconds, std::basic_string<T>>
        diff(const std::chrono::time_point<std::chrono::system_clock>& end,
             const std::chrono::time_point<std::chrono::system_clock>& start)
        {
            using namespace std;

            auto     delta              = end - start;
            uint64_t uptimeMilliseconds = chrono::duration_cast<chrono::milliseconds>(delta).count();
            uint64_t uptimeSeconds      = chrono::duration_cast<chrono::seconds>(delta).count();
            uint64_t uptimeMinutes      = chrono::duration_cast<chrono::minutes>(delta).count();
            uint64_t uptimeHours        = chrono::duration_cast<chrono::hours>(delta).count();

            // Account for hours
            uptimeMinutes = uptimeMinutes % 60i64;
            // Account for hours and minutes
            uptimeSeconds = (uptimeSeconds - (uptimeMinutes * 60i64)) % 60i64;
            // Account for hours, minutes and seconds
            uptimeMilliseconds = (uptimeMilliseconds) % 1000i64;

            // Clients would find the following useful:
            // {milliseconds, "HH:MM:SS.mmm"}
            if constexpr (std::is_same_v<T, char>)
                return {std::chrono::duration_cast<std::chrono::milliseconds>(delta),
                        std::format("{:02}:{:02}:{:02}.{:03}", uptimeHours, uptimeMinutes, uptimeSeconds, uptimeMilliseconds)};
            else if constexpr (std::is_same_v<T, wchar_t>)
                return {std::chrono::duration_cast<std::chrono::milliseconds>(delta),
                        std::format(L"{:02}:{:02}:{:02}.{:03}", uptimeHours, uptimeMinutes, uptimeSeconds, uptimeMilliseconds)};
        }


        /// @brief Return string with weeks days hours minutes seconds for the provided duration (default microseconds)
        /// @param arg Expressed as uint64_t so we can relay any resolution: us, ms, ns, s..
        /// @return string years/months/weeks days and hour minute second and millis
        ///         The years/months are in "parallel" to the weeks days hours minutes seconds
        ///         The seconds is rounded up if we end up with 500ms after the split
        ///         years / months / weeks days hours minutes seconds
        ///                 months / weeks days hours minutes seconds
        ///                          weeks days hours minutes seconds
        ///                                days hours minutes seconds
        ///                                     hours minutes seconds
        ///                                           minutes seconds
        ///                                           minutes seconds milliseconds
        template <typename T = char, typename D = std::chrono::microseconds>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::basic_string<T> durationString(const D& arg)
        {
            using namespace std;

            std::chrono::days   days(std::chrono::duration_cast<std::chrono::days>(arg));
            std::chrono::months months(std::chrono::duration_cast<std::chrono::months>(arg));
            std::chrono::years  years(std::chrono::duration_cast<std::chrono::years>(arg));
            std::chrono::weeks  weeks(std::chrono::duration_cast<std::chrono::weeks>(arg));

            // We want the "remainder" hours, minutes
            std::chrono::hours        hours(std::chrono::duration_cast<std::chrono::hours>(arg));
            std::chrono::minutes      minutes((std::chrono::duration_cast<std::chrono::minutes>(arg) / 60s));
            std::chrono::seconds      seconds(std::chrono::duration_cast<std::chrono::seconds>(arg) % 60s);
            std::chrono::milliseconds millis(std::chrono::duration_cast<std::chrono::milliseconds>(arg) % 1000ms);
            hours %= std::chrono::days(1);
            minutes %= std::chrono::hours(1);
            days -= std::chrono::duration_cast<std::chrono::days>(weeks);

            if (years > std::chrono::years(0)) {
                // Round "up" the seconds if we have excess milliseconds
                if (millis > 500ms) seconds += 1s;
                return std::format(_NORW(T, "{}years / {}months / {}weeks {} {} {} {}"),
                                   years.count(),
                                   months.count(),
                                   weeks.count(),
                                   days,
                                   hours,
                                   minutes,
                                   seconds);
            }
            else if (months > std::chrono::months(0)) {
                // Round "up" the seconds if we have excess milliseconds
                if (millis > 500ms) seconds += 1s;
                return std::format(
                        _NORW(T, "{}months / {}weeks {} {} {} {}"), months.count(), weeks.count(), days, hours, minutes, seconds);
            }
            else if (weeks > std::chrono::weeks(0)) {
                // Round "up" the seconds if we have excess milliseconds
                if (millis > 500ms) seconds += 1s;
                return std::format(_NORW(T, "{}weeks {} {} {} {}"), weeks.count(), days, hours, minutes, seconds);
            }
            else if (days > std::chrono::days(0)) {
                // Round "up" the seconds if we have excess milliseconds
                if (millis > 500ms) seconds += 1s;
                return std::format(_NORW(T, "{} {} {} {}"), days, hours, minutes, seconds);
            }
            else if (hours > std::chrono::hours(0)) {
                // Round "up" the seconds if we have excess milliseconds
                if (millis > 500ms) seconds += 1s;
                return std::format(_NORW(T, "{} {} {}"), hours, minutes, seconds);
            }
            else if (millis > std::chrono::milliseconds(0)) {
                return std::format(_NORW(T, "{} {} {}"), minutes, seconds, millis);
            }
            else {
                return std::format(_NORW(T, "{} {}"), minutes, seconds);
            }
        }


        /// @brief Converts from ISO8601 format string into time_point
        /// @tparam T One of the following: std::string, std::string_view, std::wstring or std::wstring_view
        /// @param arg string containing the ISO8601 format time
        /// @return time_point
        template <class T = char>
            requires std::same_as<T, char> || std::same_as<T, wchar_t>
        static std::chrono::system_clock::time_point parseISO8601(const std::basic_string<T>& arg)
        {
            uint32_t yearPart = 0, monthPart = 0, dayPart = 0, hourPart = 0, minutePart = 0, secondPart = 0, millisecondPart = 0;

            if constexpr (std::is_same_v<T, char>) {
                sscanf_s(arg.data(),
                         "%d-%d-%dT%d:%d:%d.%ldZ",
                         &yearPart,
                         &monthPart,
                         &dayPart,
                         &hourPart,
                         &minutePart,
                         &secondPart,
                         &millisecondPart);
            }
            else if constexpr (std::is_same_v<T, wchar_t>) {
                swscanf_s(arg.data(),
                          L"%d-%d-%dT%d:%d:%d.%ldZ",
                          &yearPart,
                          &monthPart,
                          &dayPart,
                          &hourPart,
                          &minutePart,
                          &secondPart,
                          &millisecondPart);
            }
            else {
                throw std::invalid_argument("Type is not supported; must be std::string[_view] or std::wstring[_view]");
            }

            if (yearPart > 0 && monthPart > 0 && dayPart > 0) {
                tm retTime;
                retTime.tm_year = yearPart - 1900; // Year since 1900
                retTime.tm_mon  = monthPart - 1;   // 0-11
                retTime.tm_mday = dayPart;         // 1-31
                retTime.tm_hour = hourPart;        // 0-23
                retTime.tm_min  = minutePart;      // 0-59
                retTime.tm_sec  = (int)secondPart; // 0-61 (0-60 in C++11)

                auto tp         = std::chrono::system_clock::from_time_t(_mkgmtime(&retTime));
                tp += std::chrono::milliseconds(millisecondPart);
                return tp;
            }

            return {};
        }
    };
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
