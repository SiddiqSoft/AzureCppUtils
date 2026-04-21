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

#include <iostream>
#include <format>
#include <string>
#include "siddiqsoft/conversion-utils.hpp"
#include "../include/siddiqsoft/url-utils.hpp"

namespace siddiqsoft
{
    TEST(UrlUtils, urlescape_1)
    {
        // https://www.urlencoder.io
        std::string src = R"(zaqxswcdevfrbgtnhymju,ki.lo/;p'[]\=-0987654321`)";
        std::string expected_result = R"(zaqxswcdevfrbgtnhymju%2Cki.lo%2F%3Bp%27%5B%5D%5C%3D-0987654321%60)";

        auto result = UrlUtils::encode<char>(src);
        EXPECT_EQ(expected_result, result);
    }

    TEST(UrlUtils, urlescape_1w)
    {
        // https://www.urlencoder.io
        std::wstring src             = LR"(zaqxswcdevfrbgtnhymju,ki.lo/;p'[]\=-0987654321`)";
        std::wstring expected_result = LR"(zaqxswcdevfrbgtnhymju%2Cki.lo%2F%3Bp%27%5B%5D%5C%3D-0987654321%60)";

        auto result                 = UrlUtils::encode<wchar_t>(src);
        EXPECT_EQ(expected_result, result);
    }

    TEST(UrlUtils, urlescape_2)
    {
        std::string src = R"(~!@#$%^&*()_+|}{POIUYTREWQASDFGHJKL:"?><MNBVCXZ)";
        std::string expected_result = "~%21%40%23%24%25%5E%26%2A%28%29_%2B%7C%7D%7BPOIUYTREWQASDFGHJKL%3A%22%3F%3E%3CMNBVCXZ";

        auto result = UrlUtils::encode<char>(src);
        EXPECT_EQ(expected_result, result);
    }

    TEST(UrlUtils, urlescape_2w)
    {
        std::wstring src             = LR"(~!@#$%^&*()_+|}{POIUYTREWQASDFGHJKL:"?><MNBVCXZ)";
        std::wstring expected_result = L"~%21%40%23%24%25%5E%26%2A%28%29_%2B%7C%7D%7BPOIUYTREWQASDFGHJKL%3A%22%3F%3E%3CMNBVCXZ";

        auto result                 = UrlUtils::encode<wchar_t>(src);
        EXPECT_EQ(expected_result, result);
    }

    TEST(UrlUtils, urlescape_3)
    {
        std::string src = R"(صديق)";
        std::string expected_result = "%D8%B5%D8%AF%D9%8A%D9%82";

        auto result = UrlUtils::encode<char>(src);
        EXPECT_EQ(expected_result, result);
    }

    TEST(UrlUtils, urlescape_3w)
    {
        // The library always encodes in UTF-8 despite the container
        std::wstring src = LR"(صديق)";
        #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
        std::wstring expected_result = L"%C3%98%C2%B5%C3%98%C2%AF%C3%99%C5%A0%C3%99%E2%80%9A";
        #else
        std::wstring expected_result = L"%D8%B5%D8%AF%D9%8A%D9%82";
        #endif

        auto result = UrlUtils::encode<wchar_t>(src);
        EXPECT_EQ(expected_result, result);
    }


    // ---- Additional Tests ----

    TEST(UrlUtils, encode_empty_string)
    {
        std::string empty {};
        auto        result = UrlUtils::encode<char>(empty);
        EXPECT_TRUE(result.empty());
    }

    TEST(UrlUtils, encode_empty_string_w)
    {
        std::wstring empty {};
        auto         result = UrlUtils::encode<wchar_t>(empty);
        EXPECT_TRUE(result.empty());
    }

    TEST(UrlUtils, encode_already_safe)
    {
        // Alphanumeric and unreserved characters should pass through unchanged
        std::string src {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-~_"};
        auto        result = UrlUtils::encode<char>(src);
        EXPECT_EQ(src, result);
    }

    TEST(UrlUtils, encode_space)
    {
        // Space should be encoded as %20
        std::string src {"hello world"};
        auto        result = UrlUtils::encode<char>(src);
        EXPECT_EQ("hello%20world", result);
    }

    TEST(UrlUtils, encode_lowercase_mode)
    {
        // In lowercase mode, hex digits should be lowercase
        std::string src {"hello world"};
        auto        result = UrlUtils::encode<char>(src, true);
        EXPECT_EQ("hello%20world", result);

        // Test with a character that has different hex in upper vs lower
        std::string src2 {"@"};
        auto        upper = UrlUtils::encode<char>(src2, false);
        auto        lower = UrlUtils::encode<char>(src2, true);
        EXPECT_EQ("%40", upper);
        EXPECT_EQ("%40", lower);

        // Test with a character where case matters: e.g. '[' = 0x5B
        std::string src3 {"["};
        auto        upperResult = UrlUtils::encode<char>(src3, false);
        auto        lowerResult = UrlUtils::encode<char>(src3, true);
        EXPECT_EQ("%5B", upperResult);
        EXPECT_EQ("%5b", lowerResult);
    }

    TEST(UrlUtils, encode_slash)
    {
        // '/' should be encoded
        std::string src {"a/b"};
        auto        result = UrlUtils::encode<char>(src);
        EXPECT_EQ("a%2Fb", result);
    }

    TEST(UrlUtils, encode_question_mark_and_ampersand)
    {
        std::string src {"key=value&foo=bar"};
        auto        result = UrlUtils::encode<char>(src);
        EXPECT_EQ("key%3Dvalue%26foo%3Dbar", result);
    }

    TEST(UrlUtils, encode_hash)
    {
        std::string src {"page#section"};
        auto        result = UrlUtils::encode<char>(src);
        EXPECT_EQ("page%23section", result);
    }

    TEST(UrlUtils, encode_percent)
    {
        // '%' itself should be encoded as %25
        std::string src {"100%"};
        auto        result = UrlUtils::encode<char>(src);
        EXPECT_EQ("100%25", result);
    }

    TEST(UrlUtils, encode_wchar_space)
    {
        std::wstring src {L"hello world"};
        auto         result = UrlUtils::encode<wchar_t>(src);
        EXPECT_EQ(L"hello%20world", result);
    }

    TEST(UrlUtils, encode_wchar_lowercase)
    {
        std::wstring src {L"["};
        auto         upper = UrlUtils::encode<wchar_t>(src, false);
        auto         lower = UrlUtils::encode<wchar_t>(src, true);
        EXPECT_EQ(L"%5B", upper);
        EXPECT_EQ(L"%5b", lower);
    }

    TEST(UrlUtils, encode_full_url_path)
    {
        // A realistic URL path encoding scenario
        std::string src {"https://example.com/path?q=hello world&lang=en"};
        auto        result = UrlUtils::encode<char>(src);
        EXPECT_EQ("https%3A%2F%2Fexample.com%2Fpath%3Fq%3Dhello%20world%26lang%3Den", result);
    }
} // namespace siddiqsoft
