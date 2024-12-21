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


    TEST(Base64Utils, test1a_w)
    {
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
        std::wstring sample {L"hello.world"};
        std::wstring targetSample {L"aABlAGwAbABvAC4AdwBvAHIAbABkAA=="};

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
} // namespace siddiqsoft
