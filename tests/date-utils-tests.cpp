/*
    AzureCppUtils : Azure Utilities for Modern C++

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
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "gtest/gtest.h"
#include <chrono>
#include <iostream>

#include "siddiqsoft/conversion-utils.hpp"
#include "../include/siddiqsoft/date-utils.hpp"

namespace siddiqsoft
{
    TEST(DateUtils, epochPlus_1)
    {
        auto currTime = DateUtils::parseEpoch<>(1629608266);
        // Allows for 10 duration from the currTime listed above
        auto timeoutEpochTicks = DateUtils::epochPlus(std::chrono::seconds(10), currTime);

        EXPECT_EQ(std::chrono::seconds(1629608276), timeoutEpochTicks);
    }

    TEST(DateUtils, ISO8601_1)
    {
        auto now_ts      = std::chrono::system_clock::now();

        auto now_iso8601 = DateUtils::ISO8601(now_ts);
        std::cout << "now_iso8601   : " << now_iso8601 << std::endl;
        std::cout << "now_iso8601   : " << DateUtils::ISO8601() << std::endl;
    }

    TEST(DateUtils, ISO8601_1_w)
    {
        auto now_ts      = std::chrono::system_clock::now();

        auto now_iso8601 = DateUtils::ISO8601<wchar_t>(now_ts);
        std::wcerr << L"now_iso8601   : " << now_iso8601 << std::endl;
        std::wcerr << L"now_iso8601   : " << DateUtils::ISO8601<wchar_t>() << std::endl;
    }

    TEST(DateUtils, ISO8601_2)
    {
        // 1734763627.344906
        std::chrono::system_clock::time_point ts =
                std::chrono::system_clock::from_time_t(1734763627) + std::chrono::microseconds(344906);

        auto ts_iso8601 = DateUtils::ISO8601(ts);
        EXPECT_EQ("2024-12-21T06:47:07.344Z", ts_iso8601);
    }

    TEST(DateUtils, ISO8601_2_w)
    {
        // 1734763627.344906
        std::chrono::system_clock::time_point ts =
                std::chrono::system_clock::from_time_t(1734763627) + std::chrono::microseconds(344906);

        auto ts_iso8601 = DateUtils::ISO8601<wchar_t>(ts);
        EXPECT_EQ(L"2024-12-21T06:47:07.344Z", ts_iso8601);
    }

    TEST(DateUtils, RFC7231_1)
    {
        auto now_ts      = std::chrono::system_clock::now();

        auto now_rfc7231 = DateUtils::RFC7231(now_ts);
        std::cout << "now_rfc7231   : " << now_rfc7231 << std::endl;
        std::cout << "now_rfc7231   : " << DateUtils::RFC7231() << std::endl;
    }

    TEST(DateUtils, RFC7231_1_w)
    {
        auto now_ts      = std::chrono::system_clock::now();

        auto now_rfc7231 = DateUtils::RFC7231<wchar_t>(now_ts);
        std::wcerr << L"now_rfc7231   : " << now_rfc7231 << std::endl;
        std::wcerr << L"now_rfc7231   : " << DateUtils::RFC7231<wchar_t>() << std::endl;
    }

    TEST(DateUtils, RFC7231_2)
    {
        // https://www.epochconverter.com
        auto ts         = std::chrono::system_clock::from_time_t(1734763627);

        auto ts_rfc7231 = DateUtils::RFC7231(ts);
        EXPECT_EQ("Sat, 21 Dec 2024 06:47:07 GMT", ts_rfc7231);
    }

    TEST(DateUtils, RFC7231_2_w)
    {
        // https://www.epochconverter.com
        auto ts         = std::chrono::system_clock::from_time_t(1734763627);

        auto ts_rfc7231 = DateUtils::RFC7231<wchar_t>(ts);
        EXPECT_EQ(L"Sat, 21 Dec 2024 06:47:07 GMT", ts_rfc7231);
    }

    TEST(DateUtils, toTimespan_1)
    {
        auto t3 = uint64_t(71928998);

        // days.hours:minutes:seconds
        EXPECT_EQ("832.12:16:38", DateUtils::toTimespan<>(std::chrono::seconds(t3)));
        EXPECT_EQ("10.18:19:59", DateUtils::toTimespan<>(std::chrono::seconds(929999)));
        EXPECT_EQ(L"0.00:00:40", DateUtils::toTimespan<wchar_t>(std::chrono::seconds(40)));
    }


    TEST(DateUtils, durationString_1)
    {
        using namespace std::chrono;

        auto t3  = std::chrono::microseconds(uint64_t(71928998));

        auto ds1 = siddiqsoft::DateUtils::durationString<char>(std::chrono::seconds(40));
        EXPECT_EQ("0min 40s", ds1);

        std::string ds2 = siddiqsoft::DateUtils::durationString(std::chrono::days(10) + 18h + 19min + 59s);
        EXPECT_EQ("1weeks 3d 18h 19min 59s", ds2);

        auto ds3 = siddiqsoft::DateUtils::durationString<char>(std::chrono::days(832) + 12h + 16min + 38s);
        EXPECT_EQ("2years / 27months / 118weeks 6d 12h 16min 38s", ds3);

        auto ds4 = siddiqsoft::DateUtils::durationString<char>(std::chrono::duration_cast<std::chrono::seconds>(t3));
        EXPECT_EQ("1min 11s", ds4);

        auto ds5 = siddiqsoft::DateUtils::durationString<char>(std::chrono::seconds(99));
        EXPECT_EQ("1min 39s", ds5);

        auto ds6 = siddiqsoft::DateUtils::durationString<char>(1h + 11min + 11s);
        EXPECT_EQ("1h 11min 11s", ds6);

        auto ds7 = siddiqsoft::DateUtils::durationString<char>(59min + 59s + 999ms);
        EXPECT_EQ("59min 59s 999ms", ds7);

        auto ds8 = siddiqsoft::DateUtils::durationString<char>(std::chrono::days(357) + std::chrono::days(6) + 23h + 59min + 59s +
                                                               999ms);
        EXPECT_EQ("11months / 51weeks 6d 23h 59min 60s", ds8);

        auto ds9 = siddiqsoft::DateUtils::durationString<char>(std::chrono::days(17) + 1s);
        EXPECT_EQ("2weeks 3d 0h 0min 1s", ds9);

        auto ds10 = siddiqsoft::DateUtils::durationString<char>(std::chrono::weeks(51) + std::chrono::days(2) + 1s);
        EXPECT_EQ("11months / 51weeks 2d 0h 0min 1s", ds10);
    }


    TEST(DateUtils, durationString_1_w)
    {
        using namespace std::chrono;

        auto t3  = std::chrono::microseconds(uint64_t(71928998));

        auto ds1 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::seconds(40));
        EXPECT_EQ(L"0min 40s", ds1);

        std::wstring ds2 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::days(10) + 18h + 19min + 59s);
        EXPECT_EQ(L"1weeks 3d 18h 19min 59s", ds2);

        auto ds3 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::days(832) + 12h + 16min + 38s);
        EXPECT_EQ(L"2years / 27months / 118weeks 6d 12h 16min 38s", ds3);

        auto ds4 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::duration_cast<std::chrono::seconds>(t3));
        EXPECT_EQ(L"1min 11s", ds4);

        auto ds5 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::seconds(99));
        EXPECT_EQ(L"1min 39s", ds5);

        auto ds6 = siddiqsoft::DateUtils::durationString<wchar_t>(1h + 11min + 11s);
        EXPECT_EQ(L"1h 11min 11s", ds6);

        auto ds7 = siddiqsoft::DateUtils::durationString<wchar_t>(59min + 59s + 999ms);
        EXPECT_EQ(L"59min 59s 999ms", ds7);

        auto ds8 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::days(357) + std::chrono::days(6) + 23h + 59min +
                                                                  59s + 999ms);
        EXPECT_EQ(L"11months / 51weeks 6d 23h 59min 60s", ds8);

        auto ds9 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::days(17) + 1s);
        EXPECT_EQ(L"2weeks 3d 0h 0min 1s", ds9);

        auto ds10 = siddiqsoft::DateUtils::durationString<wchar_t>(std::chrono::weeks(51) + std::chrono::days(2) + 1s);
        EXPECT_EQ(L"11months / 51weeks 2d 0h 0min 1s", ds10);
    }


    TEST(DateUtils, parseEpoch_1)
    {
        using namespace std::chrono_literals;

        auto xCallStartTime = "1563400635.344906"; // Wed 2019-07-17T21:57:15.344Z
        auto xCallEndTime   = "1563404341.603589"; // Wed 2019-07-17T22:59:01.603Z
        auto callStartTime  = DateUtils::parseEpoch<std::string>(xCallStartTime);
        std::cerr << std::format("{}", callStartTime) << std::endl;
        auto callEndTime = DateUtils::parseEpoch<std::string>(xCallEndTime);
        auto [delta, ds] = DateUtils::diff<char>(callEndTime, callStartTime);

        EXPECT_EQ(3706258ms, std::chrono::milliseconds(delta));

        std::cerr << "delta         : " << delta << std::endl;
        std::cerr << "ds            : " << ds << std::endl;
        std::cerr << std::format("ds, delta: {}, {}", ds, delta) << std::endl;
    }


    TEST(DateUtils, parseEpoch_1_w)
    {
        using namespace std::chrono_literals;

        auto xCallStartTime = L"1563400635.344906"; // Wed 2019-07-17T21:57:15.344Z
        auto xCallEndTime   = L"1563404341.603589"; // Wed 2019-07-17T22:59:01.603Z
        auto callStartTime  = DateUtils::parseEpoch<std::wstring>(xCallStartTime);
        std::wcerr << std::format(L"{}", callStartTime) << std::endl;
        auto callEndTime = DateUtils::parseEpoch<std::wstring>(xCallEndTime);
        auto [delta, ds] = DateUtils::diff<wchar_t>(callEndTime, callStartTime);
        EXPECT_EQ(3706258ms, std::chrono::milliseconds(delta));

        std::wcerr << L"delta         : " << delta << std::endl;
        std::wcerr << L"ds            : " << ds << std::endl;
        std::wcerr << std::format(L"ds, delta: {}, {}", ds, delta) << std::endl;
    }


    TEST(DateUtils, RoundTripIso8601)
    {
        auto tsNow     = std::chrono::system_clock::now();

        auto x_iso8601 = DateUtils::ISO8601(tsNow);
        std::cerr << "x_iso8601     : " << x_iso8601 << std::endl;
        std::cerr << std::format("x_iso8601: {}\n", x_iso8601) << std::endl;
        auto y_tp = DateUtils::parseISO8601(x_iso8601);
        std::cerr << "y_tp          : " << y_tp.time_since_epoch().count() << std::endl;
        std::cerr << std::format("y_tp: {}\n", y_tp.time_since_epoch().count()) << std::endl;
        auto x_iso8601_rt = DateUtils::ISO8601(y_tp);
        std::cerr << "x_iso8601_rt  : " << x_iso8601_rt << std::endl;
        std::cerr << std::format("x_iso8601_rt: {}\n", x_iso8601_rt) << std::endl;

        // There must be no difference between the original and the round-trip!
        auto [delta, deltastr] = DateUtils::diff<char>(tsNow, y_tp);
        std::cerr << std::format("deltaMS: {}\n", delta) << std::endl;
        std::cerr << std::format("delta  : {}\n", deltastr) << std::endl;
        EXPECT_EQ(std::chrono::milliseconds(0), delta);

        EXPECT_EQ(x_iso8601, x_iso8601_rt);
    }


    TEST(DateUtils, RoundTripIso8601_w)
    {
        auto tsNow     = std::chrono::system_clock::now();

        auto x_iso8601 = DateUtils::ISO8601<wchar_t>(tsNow);
        std::wcerr << L"x_iso8601     : " << x_iso8601 << std::endl;
        std::wcerr << std::format(L"x_iso8601     : {}\n", x_iso8601) << std::endl;
        auto y_tp = DateUtils::parseISO8601<wchar_t>(x_iso8601);
        std::wcerr << L"tsNow         : " << tsNow << std::endl;
        std::wcerr << L"y_tp          : " << y_tp.time_since_epoch().count() << std::endl;
        std::wcerr << std::format(L"y_tp          : {}\n", y_tp.time_since_epoch().count()) << std::endl;
        auto x_iso8601_rt = DateUtils::ISO8601<wchar_t>(y_tp);
        std::wcerr << L"x_iso8601_rt  : " << x_iso8601_rt << std::endl;
        std::wcerr << std::format(L"x_iso8601_rt  : {}\n", x_iso8601_rt) << std::endl;

        auto [delta, deltastr] = DateUtils::diff<wchar_t>(tsNow, y_tp);
        std::wcerr << std::format(L"deltaMS       : {}\n", delta) << std::endl;
        std::wcerr << std::format(L"delta         : {}\n", deltastr) << std::endl;
        EXPECT_EQ(std::chrono::milliseconds(0), delta);

        EXPECT_EQ(x_iso8601, x_iso8601_rt);
    }


    // ---- Additional Tests ----

    TEST(DateUtils, parseISO8601_known_value)
    {
        // Parse a known ISO8601 string and verify the resulting time_point
        std::string iso {"2024-12-21T06:47:07.344Z"};
        auto        tp = DateUtils::parseISO8601(iso);
        // Convert back and compare
        auto roundTrip = DateUtils::ISO8601(tp);
        EXPECT_EQ(iso, roundTrip);
    }

    TEST(DateUtils, parseISO8601_known_value_w)
    {
        std::wstring iso {L"2024-12-21T06:47:07.344Z"};
        auto         tp = DateUtils::parseISO8601<wchar_t>(iso);
        auto         roundTrip = DateUtils::ISO8601<wchar_t>(tp);
        EXPECT_EQ(iso, roundTrip);
    }

    TEST(DateUtils, parseISO8601_zero_millis)
    {
        // ISO8601 with zero milliseconds
        std::string iso {"2020-01-01T00:00:00.000Z"};
        auto        tp = DateUtils::parseISO8601(iso);
        auto        roundTrip = DateUtils::ISO8601(tp);
        EXPECT_EQ(iso, roundTrip);
    }

    TEST(DateUtils, parseISO8601_invalid_returns_epoch)
    {
        // An invalid/empty-ish string should return the default time_point (epoch)
        std::string bad {"not-a-date"};
        auto        tp = DateUtils::parseISO8601(bad);
        EXPECT_EQ(std::chrono::system_clock::time_point {}, tp);
    }

    TEST(DateUtils, parseEpoch_uint64)
    {
        // Parse epoch as uint64_t
        uint64_t epoch = 1629608266;
        auto     tp    = DateUtils::parseEpoch<uint64_t>(epoch);
        auto     iso   = DateUtils::ISO8601(tp);
        EXPECT_EQ("2021-08-22T04:57:46.000Z", iso);
    }

    TEST(DateUtils, parseEpoch_uint32)
    {
        uint32_t epoch = 1629608266;
        auto     tp    = DateUtils::parseEpoch<uint32_t>(epoch);
        auto     iso   = DateUtils::ISO8601(tp);
        EXPECT_EQ("2021-08-22T04:57:46.000Z", iso);
    }

    TEST(DateUtils, parseEpoch_int)
    {
        int  epoch = 1629608266;
        auto tp    = DateUtils::parseEpoch<int>(epoch);
        auto iso   = DateUtils::ISO8601(tp);
        EXPECT_EQ("2021-08-22T04:57:46.000Z", iso);
    }

    TEST(DateUtils, parseEpoch_zero_returns_epoch)
    {
        // Zero epoch should return the default time_point
        uint64_t epoch = 0;
        auto     tp    = DateUtils::parseEpoch<uint64_t>(epoch);
        EXPECT_EQ(std::chrono::system_clock::time_point {}, tp);
    }

    TEST(DateUtils, parseEpoch_ntp_timestamp)
    {
        // NTP timestamp (> 2208988800) should be adjusted
        // 2208988800 + 1629608266 = 3838597066
        uint64_t ntpEpoch = 3838597066ULL;
        auto     tp       = DateUtils::parseEpoch<uint64_t>(ntpEpoch);
        auto     iso      = DateUtils::ISO8601(tp);
        EXPECT_EQ("2021-08-22T04:57:46.000Z", iso);
    }

    TEST(DateUtils, epochPlus_zero_increment)
    {
        auto baseTime = std::chrono::system_clock::from_time_t(1629608266);
        auto result   = DateUtils::epochPlus(std::chrono::seconds(0), baseTime);
        EXPECT_EQ(std::chrono::seconds(1629608266), result);
    }

    TEST(DateUtils, epochPlus_large_increment)
    {
        auto baseTime = std::chrono::system_clock::from_time_t(1629608266);
        // Add 1 hour = 3600 seconds
        auto result = DateUtils::epochPlus(std::chrono::seconds(3600), baseTime);
        EXPECT_EQ(std::chrono::seconds(1629608266 + 3600), result);
    }

    TEST(DateUtils, diff_same_time)
    {
        auto now           = std::chrono::system_clock::now();
        auto [delta, dstr] = DateUtils::diff<char>(now, now);
        EXPECT_EQ(std::chrono::milliseconds(0), delta);
        EXPECT_EQ("00:00:00.000", dstr);
    }

    TEST(DateUtils, diff_same_time_w)
    {
        auto now           = std::chrono::system_clock::now();
        auto [delta, dstr] = DateUtils::diff<wchar_t>(now, now);
        EXPECT_EQ(std::chrono::milliseconds(0), delta);
        EXPECT_EQ(L"00:00:00.000", dstr);
    }

    TEST(DateUtils, diff_known_delta)
    {
        auto start         = std::chrono::system_clock::from_time_t(1000000);
        auto end           = start + std::chrono::seconds(3661) + std::chrono::milliseconds(500);
        auto [delta, dstr] = DateUtils::diff<char>(end, start);
        EXPECT_EQ(std::chrono::milliseconds(3661500), delta);
        EXPECT_EQ("01:01:01.500", dstr);
    }

    TEST(DateUtils, toTimespan_zero)
    {
        EXPECT_EQ("0.00:00:00", DateUtils::toTimespan<>(std::chrono::seconds(0)));
    }

    TEST(DateUtils, toTimespan_one_day)
    {
        // 86400 seconds = 1 day
        EXPECT_EQ("1.00:00:00", DateUtils::toTimespan<>(std::chrono::seconds(86400)));
    }

    TEST(DateUtils, toTimespan_complex)
    {
        // 90061 seconds = 1 day, 1 hour, 1 minute, 1 second
        EXPECT_EQ("1.01:01:01", DateUtils::toTimespan<>(std::chrono::seconds(90061)));
    }

    TEST(DateUtils, toTimespan_wchar)
    {
        EXPECT_EQ(L"1.01:01:01", DateUtils::toTimespan<wchar_t>(std::chrono::seconds(90061)));
    }

    TEST(DateUtils, RFC7231_known_epoch)
    {
        // Unix epoch: Thu, 01 Jan 1970 00:00:00 GMT
        auto ts = std::chrono::system_clock::from_time_t(0);
        auto r  = DateUtils::RFC7231(ts);
        EXPECT_EQ("Thu, 01 Jan 1970 00:00:00 GMT", r);
    }

    TEST(DateUtils, RFC7231_known_epoch_w)
    {
        auto ts = std::chrono::system_clock::from_time_t(0);
        auto r  = DateUtils::RFC7231<wchar_t>(ts);
        EXPECT_EQ(L"Thu, 01 Jan 1970 00:00:00 GMT", r);
    }

    TEST(DateUtils, ISO8601_epoch)
    {
        auto ts  = std::chrono::system_clock::from_time_t(0);
        auto iso = DateUtils::ISO8601(ts);
        EXPECT_EQ("1970-01-01T00:00:00.000Z", iso);
    }

    TEST(DateUtils, durationString_zero)
    {
        auto ds = DateUtils::durationString<char>(std::chrono::seconds(0));
        EXPECT_EQ("0min 0s", ds);
    }

    TEST(DateUtils, durationString_just_seconds)
    {
        auto ds = DateUtils::durationString<char>(std::chrono::seconds(5));
        EXPECT_EQ("0min 5s", ds);
    }

    TEST(DateUtils, durationString_exactly_one_hour)
    {
        using namespace std::chrono;
        auto ds = DateUtils::durationString<char>(1h);
        EXPECT_EQ("1h 0min 0s", ds);
    }

    TEST(DateUtils, durationString_exactly_one_day)
    {
        using namespace std::chrono;
        auto ds = DateUtils::durationString<char>(std::chrono::days(1));
        EXPECT_EQ("1d 0h 0min 0s", ds);
    }
} // namespace siddiqsoft