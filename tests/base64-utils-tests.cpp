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
#include <ratio>

#include "siddiqsoft/conversion-utils.hpp"
#include "../include/siddiqsoft/base64-utils.hpp"
#include "../include/siddiqsoft/date-utils.hpp"
#include "../include/siddiqsoft/encryption-utils.hpp"
#include "../include/siddiqsoft/url-utils.hpp"

namespace siddiqsoft
{
    TEST(Base64Utils, test1a)
    {
        std::string sample {"@#$%67yhgbjko)(*&^%EsdfghjGFR%^Yghji876tY7<>:{}\">+_(*^$#~jgHJ"};
        std::string targetSample {"QCMkJTY3eWhnYmprbykoKiZeJUVzZGZnaGpHRlIlXllnaGppODc2dFk3PD46e30iPitfKCpeJCN+amdISg=="};

        try {
            auto result = Base64Utils::encode(sample);
            EXPECT_EQ(result, targetSample) << "Not Valid encode";

            auto result2 = Base64Utils::decode(result);
            EXPECT_EQ(sample, result2) << "Not valid decode";

            auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
            EXPECT_EQ(roundTrip, sample);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }


    TEST(Base64Utils, test1b)
    {
        std::string sample {"hello.world"};
        std::string targetSample {"aGVsbG8ud29ybGQ="};

        try {
            auto result = Base64Utils::encode(sample);

            EXPECT_STREQ(result.c_str(), targetSample.c_str()) << "Not Valid encode";

            auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
            EXPECT_EQ(roundTrip, sample);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
    TEST(Base64Utils, test1a_w)
    {
        // This test works only for Windows; the native functions support UTF-16 encoding/decoding
        // whereas the OpenSSL version only supports utf-8
        std::wstring sample {L"@#$%67yhgbjko)(*&^%EsdfghjGFR%^Yghji876tY7<>:{}\">+_(*^$#~jgHJ"};
        std::wstring targetSample {L"QAAjACQAJQA2ADcAeQBoAGcAYgBqAGsAbwApACgAKgAmAF4AJQBFAHMAZABmAGcAaABqAEcARgBSACUAXgBZAGcAaABqAG"
                                   L"kAOAA3ADYAdABZADcAPAA+ADoAewB9ACIAPgArAF8AKAAqAF4AJAAjAH4AagBnAEgASgA="}; // UTF-16 decode

        try {
            auto result = Base64Utils::encode(sample);
            EXPECT_EQ(targetSample, result) << "Not Valid encode";

            auto result2 = Base64Utils::decode(result);
            EXPECT_EQ(sample, result2) << "Not valid decode";

            auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }


    TEST(Base64Utils, test1b_w)
    {
        // This test works only for Windows; the native functions support UTF-16 encoding/decoding
        // whereas the OpenSSL version only supports utf-8
        std::wstring sample {L"hello.world"};
        std::wstring targetSample {L"aABlAGwAbABvAC4AdwBvAHIAbABkAA=="}; // utf-16; will not work for unix

        try {
            auto result = Base64Utils::encode(sample);

            EXPECT_EQ(targetSample, result) << "Not Valid encode";

            auto result2 = Base64Utils::decode(result);
            EXPECT_EQ(sample, result2) << "Not Valid decode";

            auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }
#endif

    TEST(Base64Utils, test1c_w)
    {
        std::wstring sample {L"hello."};

        try {
            auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }


    TEST(Base64Utils, test1d)
    {
        std::string sample {"صديق"};

        try {
            auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }

    TEST(Base64Utils, test1d_w)
    {
        std::wstring sample {L"صديق"};

        try {
            auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
            EXPECT_EQ(sample, roundTrip);
        }
        catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            EXPECT_FALSE("No match.");
        }
    }


    // ---- Additional Tests ----

    TEST(Base64Utils, encode_empty_string)
    {
        // Encoding an empty string should return an empty string
        std::string empty {};
        auto        result = Base64Utils::encode(empty);
        EXPECT_TRUE(result.empty()) << "Encoding empty string should yield empty result";
    }

    TEST(Base64Utils, decode_empty_string)
    {
        // Decoding an empty string should return an empty string
        std::string empty {};
        auto        result = Base64Utils::decode(empty);
        EXPECT_TRUE(result.empty()) << "Decoding empty string should yield empty result";
    }

    TEST(Base64Utils, encode_single_char)
    {
        // Single character: 'A' -> base64 "QQ=="
        std::string sample {"A"};
        auto        encoded = Base64Utils::encode(sample);
        EXPECT_EQ("QQ==", encoded);
        auto decoded = Base64Utils::decode(encoded);
        EXPECT_EQ(sample, decoded);
    }

    TEST(Base64Utils, encode_two_chars)
    {
        // Two characters: 'AB' -> base64 "QUI="
        std::string sample {"AB"};
        auto        encoded = Base64Utils::encode(sample);
        EXPECT_EQ("QUI=", encoded);
        auto decoded = Base64Utils::decode(encoded);
        EXPECT_EQ(sample, decoded);
    }

    TEST(Base64Utils, encode_three_chars_no_padding)
    {
        // Three characters: 'ABC' -> base64 "QUJD" (no padding)
        std::string sample {"ABC"};
        auto        encoded = Base64Utils::encode(sample);
        EXPECT_EQ("QUJD", encoded);
        auto decoded = Base64Utils::decode(encoded);
        EXPECT_EQ(sample, decoded);
    }

    TEST(Base64Utils, roundtrip_binary_like_data)
    {
        // Test with data containing null-adjacent bytes and special characters
        std::string sample {"\x01\x02\x03\xFF\xFE\xFD"};
        auto        roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
        EXPECT_EQ(sample, roundTrip);
    }

    TEST(Base64Utils, urlEscape_replaces_plus_slash_equals)
    {
        // '+' -> '-', '/' -> '_', '=' removed
        std::string src {"abc+def/ghi=="};
        auto        result = Base64Utils::urlEscape(src);
        EXPECT_EQ("abc-def_ghi", result);
    }

    TEST(Base64Utils, urlEscape_empty_string)
    {
        std::string empty {};
        auto        result = Base64Utils::urlEscape(empty);
        EXPECT_TRUE(result.empty());
    }

    TEST(Base64Utils, urlEscape_no_special_chars)
    {
        // String without +, /, = should remain unchanged
        std::string src {"abcdefghij0123456789"};
        auto        result = Base64Utils::urlEscape(src);
        EXPECT_EQ(src, result);
    }

    TEST(Base64Utils, urlEscape_wchar)
    {
        std::wstring src {L"abc+def/ghi=="};
        auto         result = Base64Utils::urlEscape(src);
        EXPECT_EQ(L"abc-def_ghi", result);
    }

    TEST(Base64Utils, roundtrip_long_string)
    {
        // Test with a longer string to exercise multi-block encoding
        std::string sample(256, 'X');
        auto        roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
        EXPECT_EQ(sample, roundTrip);
    }

    TEST(Base64Utils, roundtrip_all_byte_values)
    {
        // Build a string with all byte values 1-255 (skip 0 since std::string treats it as content but
        // the null-trimming in decode could interfere)
        std::string sample;
        for (int i = 1; i < 256; i++) {
            sample.push_back(static_cast<char>(i));
        }
        auto roundTrip = Base64Utils::decode(Base64Utils::encode(sample));
        EXPECT_EQ(sample, roundTrip);
    }

    TEST(Base64Utils, decode_known_value)
    {
        // "SGVsbG8gV29ybGQ=" decodes to "Hello World"
        std::string encoded {"SGVsbG8gV29ybGQ="};
        auto        decoded = Base64Utils::decode(encoded);
        EXPECT_EQ("Hello World", decoded);
    }

    TEST(Base64Utils, encode_known_value)
    {
        // "Hello World" encodes to "SGVsbG8gV29ybGQ="
        std::string source {"Hello World"};
        auto        encoded = Base64Utils::encode(source);
        EXPECT_EQ("SGVsbG8gV29ybGQ=", encoded);
    }
} // namespace siddiqsoft
