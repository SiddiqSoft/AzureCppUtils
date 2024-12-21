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
    /* Encryption Tests */
    TEST(EncryptionUtils, MD5HashTest)
    {
        std::string myMessage = "My ^&()=+-_[]{};:\'\"<>?`~ N@me is $0.50! A whole 50 off! #discount./|\\";
        auto        varKey    = EncryptionUtils::MD5(myMessage);
        EXPECT_EQ(varKey, "a2cf7440dab41a41487ec62f40d68cee");

        std::string myMessage1 = "happy";
        auto        varKey1    = EncryptionUtils::MD5(myMessage1);
        EXPECT_EQ(varKey1, "56ab24c15b72a457069c5ea42fcfc640");
    }

    TEST(EncryptionUtils, MD5HashTest_w)
    {
        std::wstring myMessage = L"My ^&()=+-_[]{};:\'\"<>?`~ N@me is $0.50! A whole 50 off! #discount./|\\";
        auto         varKey    = EncryptionUtils::MD5(myMessage);
        EXPECT_EQ(varKey, "a2cf7440dab41a41487ec62f40d68cee");

        std::wstring myMessage1 = L"happy";
        auto         varKey1    = EncryptionUtils::MD5(myMessage1);
        EXPECT_EQ(varKey1, "56ab24c15b72a457069c5ea42fcfc640");
    }

    TEST(EncryptionUtils, SASToken_1)
    {
        // https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token#code-try-4
        /*
            Now: 1629607976 --> 1629608276
            SASToken: SharedAccessSignature
           sr=myNamespace.servicebus.windows.net%2fmyEventHub&sig=%2bl9P4WDuWjGTXrA8PeFKGl0g9Nj5OnynM55TK3huc1M%3d&se=1629608276&skn=RootManageSharedAccessKey
        */
        std::string keyname {"RootManageSharedAccessKey"};
        std::string secret {"myPrimaryKey"};

        auto url = "myNamespace.servicebus.windows.net/myEventHub";
        auto sas = EncryptionUtils::SASToken<char>(secret, url, keyname, "1629608276");

        // clang-format off
        EXPECT_EQ("SharedAccessSignature sr=myNamespace.servicebus.windows.net%2fmyEventHub&sig=%2bl9P4WDuWjGTXrA8PeFKGl0g9Nj5OnynM55TK3huc1M%3d&se=1629608276&skn=RootManageSharedAccessKey",
                  sas);
        // clang-format on
    }

    TEST(EncryptionUtils, SASToken_2)
    {
        std::string keyname {"sender"};
        std::string secret {"kJZN1+Im0GrRYCaZKNUj2YdA3WzP9MIqUKtojbouQeY="};

        auto sas = siddiqsoft::EncryptionUtils::SASToken<char>(
                secret, "https://helloworldeventhub.servicebus.windows.net/myhub", keyname, std::chrono::hours(24));
    }

    TEST(EncryptionUtils, SASToken_2_w)
    {
        std::wstring keyname {L"sender"};
        std::string  secret {"kJZN1+Im0GrRYCaZKNUj2YdA3WzP9MIqUKtojbouQeY="};

        auto sas = siddiqsoft::EncryptionUtils::SASToken<wchar_t>(
                secret, L"https://helloworldeventhub.servicebus.windows.net/myhub", keyname, std::chrono::hours(24));
    }


    TEST(EncryptionUtils, SASToken_1_w)
    {
        // https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token#code-try-4
        /*
            Now: 1629607976 --> 1629608276
            SASToken: SharedAccessSignature
           sr=myNamespace.servicebus.windows.net%2fmyEventHub&sig=%2bl9P4WDuWjGTXrA8PeFKGl0g9Nj5OnynM55TK3huc1M%3d&se=1629608276&skn=RootManageSharedAccessKey
        */
        std::wstring keyname {L"RootManageSharedAccessKey"};
        std::string  secret {"myPrimaryKey"};
        std::wstring url {L"myNamespace.servicebus.windows.net/myEventHub"};

        auto sas = EncryptionUtils::SASToken(secret, url, keyname, std::wstring {L"1629608276"});

        // clang-format off
        EXPECT_EQ(L"SharedAccessSignature sr=myNamespace.servicebus.windows.net%2fmyEventHub&sig=%2bl9P4WDuWjGTXrA8PeFKGl0g9Nj5OnynM55TK3huc1M%3d&se=1629608276&skn=RootManageSharedAccessKey",
                  sas);
        // clang-format on
    }

    TEST(EncryptionUtils, JWTHMAC256_1)
    {
        std::string header {"{\"alg\":\"HS256\",\"typ\":\"JWT\"}"};
        std::string payload {"{\"loggedInAs\":\"admin\",\"iat\":1422779638}"};
        std::string secret {"secretkey"};

        // Caution using nlohman::json to dump()
        // The JSON specification does not mandate ordering of the key-value pairs. However, if you notice, the JWT base64url are
        // very sensitive to ordering.
        auto jwt = EncryptionUtils::JWTHMAC256(secret, header, payload);

        // clang-format off
        EXPECT_STREQ(jwt.c_str(),
                     "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJsb2dnZWRJbkFzIjoiYWRtaW4iLCJpYXQiOjE0MjI3Nzk2Mzh9.gzSraSYS8EXBxLN_oWnFSRgCzcmJmMjLiuyu5CSpyHI")
                << "Not Valid JWT encode";
        // clang-format on
    }

    TEST(EncryptionUtils, JWTHMAC256_1_w)
    {
        std::wstring header {L"{\"alg\":\"HS256\",\"typ\":\"JWT\"}"};
        std::wstring payload {L"{\"loggedInAs\":\"admin\",\"iat\":1422779638}"};
        std::string  secret {"secretkey"};

        // Caution using nlohman::json to dump()
        // The JSON specification does not mandate ordering of the key-value pairs. However, if you notice, the JWT base64url are
        // very sensitive to ordering.
        auto jwt = EncryptionUtils::JWTHMAC256(secret, header, payload);

        // clang-format off
        EXPECT_STREQ(jwt.c_str(),
                     L"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJsb2dnZWRJbkFzIjoiYWRtaW4iLCJpYXQiOjE0MjI3Nzk2Mzh9.gzSraSYS8EXBxLN_oWnFSRgCzcmJmMjLiuyu5CSpyHI")
                << "Not Valid JWT encode";
        // clang-format on
    }

    TEST(EncryptionUtils, HMAC_1)
    {
        std::string myData {"hello \xF0\x9F\x8C\x8E"};
        std::string myKey {"01234567890123456789012345678901"};

        try {
            auto r1 = EncryptionUtils::HMAC(myData, myKey);

            // Using online tool: https://www.liavaag.org/English/SHA-Generator/HMAC/
            EXPECT_STREQ("f+TQQp/dU+OEWEwY6sRpfVNgN2hKSIklnVOh6iBEPdE=", Base64Utils::encode(r1).c_str());
        }
        catch (const std::exception& e) {
            EXPECT_TRUE(false) << e.what() << std::endl;
        }
    }

    TEST(EncryptionUtils, HMAC_1_w)
    {
        std::wstring myData {L"hello \xD83C\xDF0E"};
        std::string  myKey {"01234567890123456789012345678901"};

        try {
            auto r1 = EncryptionUtils::HMAC<wchar_t>(myData, myKey);

            // Using online tool: https://www.liavaag.org/English/SHA-Generator/HMAC/
            EXPECT_STREQ("6zcwPNrzCM1GM/yjRQAvuNOfq35X5b0j0H+BbyfPNx8=", Base64Utils::encode(r1).c_str());
        }
        catch (const std::exception& e) {
            EXPECT_TRUE(false) << e.what() << std::endl;
        }
    }

    TEST(EncryptionUtils, CosmosToken)
    {
        std::string Key = "dsZQi3KtZmCv1ljt3VNWNm7sQUF1y5rJfC6kv5JiwvW0EndXdDku/dkKBp8/ufDToSxLzR4y+O/0H/t4bQtVNw==";

        EXPECT_EQ(Key, Base64Utils::encode(Base64Utils::decode(Key)));

        auto decodedKey = siddiqsoft::Base64Utils::decode(Key);
        auto auth = EncryptionUtils::CosmosToken<char>(decodedKey, "GET", "dbs", "dbs/ToDoList", "Thu, 27 Apr 2017 00:51:12 GMT");

        // https://docs.microsoft.com/en-us/rest/api/cosmos-db/access-control-on-cosmosdb-resources?redirectedfrom=MSDN
        EXPECT_EQ("type%3dmaster%26ver%3d1.0%26sig%3dc09PEVJrgp2uQRkr934kFbTqhByc7TVr3OHyqlu%2bc%2bc%3d", auth);
    }

    TEST(EncryptionUtils, CosmosToken_w)
    {
        std::string Key {"dsZQi3KtZmCv1ljt3VNWNm7sQUF1y5rJfC6kv5JiwvW0EndXdDku/dkKBp8/ufDToSxLzR4y+O/0H/t4bQtVNw=="};

        EXPECT_EQ(Key, Base64Utils::encode(Base64Utils::decode(Key)));

        auto decodedKey = siddiqsoft::Base64Utils::decode(Key);
        auto auth       = EncryptionUtils::CosmosToken<wchar_t>(
                decodedKey, L"GET", L"dbs", L"dbs/ToDoList", L"Thu, 27 Apr 2017 00:51:12 GMT");

        // https://docs.microsoft.com/en-us/rest/api/cosmos-db/access-control-on-cosmosdb-resources?redirectedfrom=MSDN
        EXPECT_EQ(L"type%3dmaster%26ver%3d1.0%26sig%3dc09PEVJrgp2uQRkr934kFbTqhByc7TVr3OHyqlu%2bc%2bc%3d", auth);
    }
} // namespace siddiqsoft
