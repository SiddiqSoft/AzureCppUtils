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
        std::wstring expected_result = L"%D8%B5%D8%AF%D9%8A%D9%82";

        auto result = UrlUtils::encode<wchar_t>(src);
        EXPECT_EQ(expected_result, result);
    }
} // namespace siddiqsoft
