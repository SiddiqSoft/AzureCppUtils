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
#if defined(__linux__) || defined(__APPLE__)
    TEST(EncryptionUtils, MD5HashTest_0)
    {
        std::string myMessage = "My ^&()=+-_[]{};:\'\"<>?`~ N@me is $0.50! A whole 50 off! #discount./|\\";
        auto        varKey    = EncryptionUtils::calcDigest("MD5", myMessage);
        EXPECT_EQ(varKey, "a2cf7440dab41a41487ec62f40d68cee");

        std::string myMessage1 = "happy";
        auto        varKey1    = EncryptionUtils::calcDigest("MD5", myMessage1);
        EXPECT_EQ(varKey1, "56ab24c15b72a457069c5ea42fcfc640");
    }
#endif

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
        std::string key {"myPrimaryKey"};
        std::string url = "myNamespace.servicebus.windows.net/myEventHub";
        // In this test, we're providing the epoch as the expiry string.
        // corresponds to Sunday, August 22, 2021 4:57:56 AM UTC
        auto sas = EncryptionUtils::SASToken<char>(key, url, keyname, "1629608276");

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

    TEST(EncryptionUtils, HMAC_0)
    {
        std::string myData {"hello world"};
        std::string myKey {"01234567890123456789012345678901"};

        try {
            auto r1  = EncryptionUtils::HMAC(myData, myKey);
            auto r1e = Base64Utils::encode(r1);
            // Using online tool: https://www.liavaag.org/English/SHA-Generator/HMAC/
            EXPECT_STREQ("B8dXpkmWppplo/hAbiHLuXgIEPFErnypOewwhH1+tPQ=", r1e.c_str());
        }
        catch (const std::exception& e) {
            EXPECT_TRUE(false) << e.what() << std::endl;
        }
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
        std::wstring myData {L"hello صديق"};
        std::string  myKey {"01234567890123456789012345678901"};

        try {
            auto r1  = EncryptionUtils::HMAC<wchar_t>(myData, myKey);
            auto r1e = Base64Utils::encode(r1);
            // Using online tool: https://www.liavaag.org/English/SHA-Generator/HMAC/
            EXPECT_STREQ("Sr4F0Mxb/dmb/Rmx3T7sEgV9CptGkX+xdayR9YHgLJA=", r1e.c_str());
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


    // ---- Additional Tests ----

    TEST(EncryptionUtils, MD5_empty_string)
    {
        // MD5 of empty string is d41d8cd98f00b204e9800998ecf8427e
        std::string empty {};
        auto        result = EncryptionUtils::MD5(empty);
        // Empty source should return empty result (the implementation guards against empty)
        EXPECT_TRUE(result.empty());
    }

    TEST(EncryptionUtils, MD5_known_value)
    {
        // MD5("abc") = 900150983cd24fb0d6963f7d28e17f72
        std::string src {"abc"};
        auto        result = EncryptionUtils::MD5(src);
        EXPECT_EQ("900150983cd24fb0d6963f7d28e17f72", result);
    }

    TEST(EncryptionUtils, MD5_known_value_w)
    {
        // wchar_t version should produce same result as narrow for ASCII content
        std::wstring src {L"abc"};
        auto         result = EncryptionUtils::MD5(src);
        EXPECT_EQ("900150983cd24fb0d6963f7d28e17f72", result);
    }

#if defined(__linux__) || defined(__APPLE__)
    TEST(EncryptionUtils, calcDigest_unsupported_type_returns_empty)
    {
        // calcDigest only supports MD5 and MD4; other types return empty
        auto result = EncryptionUtils::calcDigest("INVALID_DIGEST", "test");
        EXPECT_TRUE(result.empty());
    }

    TEST(EncryptionUtils, calcDigest_empty_source)
    {
        // Empty source should return empty result
        auto result = EncryptionUtils::calcDigest("MD5", "");
        EXPECT_TRUE(result.empty());
    }

    TEST(EncryptionUtils, calcDigest_MD4_legacy_returns_empty)
    {
        // MD4 is a legacy algorithm; in OpenSSL 3.x default provider the digest init may fail
        // silently, resulting in an empty return value
        auto result = EncryptionUtils::calcDigest("MD4", "abc");
        // MD4 may or may not be available depending on OpenSSL configuration
        // If available: "a448017aaf21d8525fc10ae87aa6729d", otherwise empty
        EXPECT_TRUE(result.empty() || result == "a448017aaf21d8525fc10ae87aa6729d");
    }
#endif

    TEST(EncryptionUtils, HMAC_empty_message)
    {
        // Empty message should return empty result
        std::string emptyMsg {};
        std::string key {"somekey"};
        auto        result = EncryptionUtils::HMAC(emptyMsg, key);
        EXPECT_TRUE(result.empty());
    }

    TEST(EncryptionUtils, HMAC_empty_key)
    {
        // Empty key should return empty result
        std::string msg {"hello"};
        std::string emptyKey {};
        auto        result = EncryptionUtils::HMAC(msg, emptyKey);
        EXPECT_TRUE(result.empty());
    }

    TEST(EncryptionUtils, HMAC_known_value)
    {
        // HMAC-SHA256("message", "key") verified with external tool
        std::string msg {"message"};
        std::string key {"key"};
        auto        hmacResult = EncryptionUtils::HMAC(msg, key);
        auto        b64        = Base64Utils::encode(hmacResult);
        // https://www.liavaag.org/English/SHA-Generator/HMAC/
        // HMAC-SHA256("message", "key") = 6e9ef29b75fffc5b7abae527d58fdadb2fe42e7219011976917343065f58ed4a
        // Base64: bp7ym3X//Ft6uuUn1Y/a2y/kLnIZARl2kXNDBl9Y7Uo=
        EXPECT_EQ("bp7ym3X//Ft6uuUn1Y/a2y/kLnIZARl2kXNDBl9Y7Uo=", b64);
    }

    TEST(EncryptionUtils, SASToken_empty_url_throws)
    {
        EXPECT_THROW(EncryptionUtils::SASToken<char>("key", std::string {}, std::string {"name"}, std::string {"12345"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, SASToken_empty_keyName_throws)
    {
        EXPECT_THROW(EncryptionUtils::SASToken<char>("key", std::string {"url"}, std::string {}, std::string {"12345"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, SASToken_empty_key_throws)
    {
        EXPECT_THROW(
                EncryptionUtils::SASToken<char>(std::string {}, std::string {"url"}, std::string {"name"}, std::string {"12345"}),
                std::invalid_argument);
    }

    TEST(EncryptionUtils, SASToken_empty_expiry_throws)
    {
        EXPECT_THROW(EncryptionUtils::SASToken<char>("key", std::string {"url"}, std::string {"name"}, std::string {}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, CosmosToken_empty_key_throws)
    {
        EXPECT_THROW(EncryptionUtils::CosmosToken<char>(
                             std::string {}, std::string {"GET"}, std::string {"dbs"}, std::string {"dbs/db1"}, std::string {"Thu, 27 Apr 2017 00:51:12 GMT"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, CosmosToken_empty_verb_throws)
    {
        EXPECT_THROW(EncryptionUtils::CosmosToken<char>(
                             std::string {"key"}, std::string {}, std::string {"dbs"}, std::string {"dbs/db1"}, std::string {"Thu, 27 Apr 2017 00:51:12 GMT"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, CosmosToken_empty_date_throws)
    {
        EXPECT_THROW(EncryptionUtils::CosmosToken<char>(
                             std::string {"key"}, std::string {"GET"}, std::string {"dbs"}, std::string {"dbs/db1"}, std::string {}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, CosmosToken_POST_verb)
    {
        // Test with POST verb to ensure verb lowercasing works
        std::string Key {"dsZQi3KtZmCv1ljt3VNWNm7sQUF1y5rJfC6kv5JiwvW0EndXdDku/dkKBp8/ufDToSxLzR4y+O/0H/t4bQtVNw=="};
        auto        decodedKey = Base64Utils::decode(Key);
        auto        auth       = EncryptionUtils::CosmosToken<char>(decodedKey, "POST", "docs", "dbs/ToDoList/colls/Items", "Thu, 27 Apr 2017 00:51:12 GMT");
        // Just verify it produces a non-empty result with the expected prefix
        EXPECT_TRUE(auth.starts_with("type%3dmaster%26ver%3d1.0%26sig%3d"));
        EXPECT_FALSE(auth.empty());
    }

    TEST(EncryptionUtils, JWTHMAC256_different_payload)
    {
        std::string header {R"({"alg":"HS256","typ":"JWT"})"};
        std::string payload {R"({"sub":"1234567890","name":"John Doe","iat":1516239022})"};
        std::string secret {"your-256-bit-secret"};

        auto jwt = EncryptionUtils::JWTHMAC256(secret, header, payload);
        // JWT should have exactly 3 parts separated by dots
        auto firstDot  = jwt.find('.');
        auto secondDot = jwt.find('.', firstDot + 1);
        EXPECT_NE(std::string::npos, firstDot);
        EXPECT_NE(std::string::npos, secondDot);
        // No more dots after the second
        EXPECT_EQ(std::string::npos, jwt.find('.', secondDot + 1));
        // All three parts should be non-empty
        EXPECT_GT(firstDot, 0u);
        EXPECT_GT(secondDot - firstDot - 1, 0u);
        EXPECT_GT(jwt.size() - secondDot - 1, 0u);
    }

    // ---- SASToken with chrono::seconds timeout (exercises the time()-based overload) ----

    TEST(EncryptionUtils, SASToken_with_chrono_seconds)
    {
        std::string keyname {"myKeyName"};
        std::string key {"mySecretKey"};
        std::string url {"https://myservice.servicebus.windows.net/myhub"};

        // This exercises the SASToken overload that takes chrono::seconds and calls time()
        auto sas = EncryptionUtils::SASToken<char>(key, url, keyname, std::chrono::seconds(300));
        EXPECT_TRUE(sas.starts_with("SharedAccessSignature sr="));
        EXPECT_NE(std::string::npos, sas.find("&sig="));
        EXPECT_NE(std::string::npos, sas.find("&se="));
        EXPECT_NE(std::string::npos, sas.find("&skn=myKeyName"));
    }

    TEST(EncryptionUtils, SASToken_with_chrono_seconds_wchar)
    {
        std::wstring keyname {L"myKeyName"};
        std::string  key {"mySecretKey"};
        std::wstring url {L"https://myservice.servicebus.windows.net/myhub"};

        // wchar_t version of the chrono::seconds overload
        auto sas = EncryptionUtils::SASToken<wchar_t>(key, url, keyname, std::chrono::seconds(300));
        EXPECT_TRUE(sas.starts_with(L"SharedAccessSignature sr="));
        EXPECT_NE(std::wstring::npos, sas.find(L"&sig="));
        EXPECT_NE(std::wstring::npos, sas.find(L"&se="));
        EXPECT_NE(std::wstring::npos, sas.find(L"&skn=myKeyName"));
    }

    // ---- SASToken wchar_t with string expiry (exercises the wchar_t delegation path) ----

    TEST(EncryptionUtils, SASToken_wchar_empty_url_throws)
    {
        EXPECT_THROW(EncryptionUtils::SASToken<wchar_t>("key", std::wstring {}, std::wstring {L"name"}, std::wstring {L"12345"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, SASToken_wchar_empty_keyName_throws)
    {
        EXPECT_THROW(EncryptionUtils::SASToken<wchar_t>("key", std::wstring {L"url"}, std::wstring {}, std::wstring {L"12345"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, SASToken_wchar_empty_key_throws)
    {
        EXPECT_THROW(EncryptionUtils::SASToken<wchar_t>(
                             std::string {}, std::wstring {L"url"}, std::wstring {L"name"}, std::wstring {L"12345"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, SASToken_wchar_empty_expiry_throws)
    {
        EXPECT_THROW(EncryptionUtils::SASToken<wchar_t>("key", std::wstring {L"url"}, std::wstring {L"name"}, std::wstring {}),
                     std::invalid_argument);
    }

    // ---- CosmosToken wchar_t throws ----

    TEST(EncryptionUtils, CosmosToken_wchar_empty_key_throws)
    {
        EXPECT_THROW(EncryptionUtils::CosmosToken<wchar_t>(
                             std::string {}, std::wstring {L"GET"}, std::wstring {L"dbs"}, std::wstring {L"dbs/db1"}, std::wstring {L"Thu, 27 Apr 2017 00:51:12 GMT"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, CosmosToken_wchar_empty_verb_throws)
    {
        EXPECT_THROW(EncryptionUtils::CosmosToken<wchar_t>(
                             std::string {"key"}, std::wstring {}, std::wstring {L"dbs"}, std::wstring {L"dbs/db1"}, std::wstring {L"Thu, 27 Apr 2017 00:51:12 GMT"}),
                     std::invalid_argument);
    }

    TEST(EncryptionUtils, CosmosToken_wchar_empty_date_throws)
    {
        EXPECT_THROW(EncryptionUtils::CosmosToken<wchar_t>(
                             std::string {"key"}, std::wstring {L"GET"}, std::wstring {L"dbs"}, std::wstring {L"dbs/db1"}, std::wstring {}),
                     std::invalid_argument);
    }

    // ---- CosmosToken with empty type (allowed — no validation on type) ----

    TEST(EncryptionUtils, CosmosToken_empty_type)
    {
        std::string Key {"dsZQi3KtZmCv1ljt3VNWNm7sQUF1y5rJfC6kv5JiwvW0EndXdDku/dkKBp8/ufDToSxLzR4y+O/0H/t4bQtVNw=="};
        auto        decodedKey = Base64Utils::decode(Key);
        // Empty type is allowed (e.g., for root-level operations)
        auto auth = EncryptionUtils::CosmosToken<char>(decodedKey, "GET", "", "", "Thu, 27 Apr 2017 00:51:12 GMT");
        EXPECT_TRUE(auth.starts_with("type%3dmaster%26ver%3d1.0%26sig%3d"));
        EXPECT_FALSE(auth.empty());
    }

    TEST(EncryptionUtils, CosmosToken_DELETE_verb)
    {
        std::string Key {"dsZQi3KtZmCv1ljt3VNWNm7sQUF1y5rJfC6kv5JiwvW0EndXdDku/dkKBp8/ufDToSxLzR4y+O/0H/t4bQtVNw=="};
        auto        decodedKey = Base64Utils::decode(Key);
        auto        auth       = EncryptionUtils::CosmosToken<char>(decodedKey, "DELETE", "docs", "dbs/ToDoList/colls/Items/docs/doc1", "Thu, 27 Apr 2017 00:51:12 GMT");
        EXPECT_TRUE(auth.starts_with("type%3dmaster%26ver%3d1.0%26sig%3d"));
    }

    TEST(EncryptionUtils, CosmosToken_PUT_verb)
    {
        std::string Key {"dsZQi3KtZmCv1ljt3VNWNm7sQUF1y5rJfC6kv5JiwvW0EndXdDku/dkKBp8/ufDToSxLzR4y+O/0H/t4bQtVNw=="};
        auto        decodedKey = Base64Utils::decode(Key);
        auto        auth       = EncryptionUtils::CosmosToken<char>(decodedKey, "PUT", "docs", "dbs/ToDoList/colls/Items/docs/doc1", "Thu, 27 Apr 2017 00:51:12 GMT");
        EXPECT_TRUE(auth.starts_with("type%3dmaster%26ver%3d1.0%26sig%3d"));
    }

#if defined(__linux__) || defined(__APPLE__)
    TEST(EncryptionUtils, calcDigest_MD5_known_abc)
    {
        // Directly test calcDigest with MD5 for "abc"
        auto result = EncryptionUtils::calcDigest("MD5", "abc");
        EXPECT_EQ("900150983cd24fb0d6963f7d28e17f72", result);
    }

    TEST(EncryptionUtils, calcDigest_MD5_long_string)
    {
        // Test calcDigest with a longer string
        auto result = EncryptionUtils::calcDigest("MD5", "The quick brown fox jumps over the lazy dog");
        EXPECT_EQ("9e107d9d372bb6826bd81d3542a419d6", result);
    }

    TEST(EncryptionUtils, calcDigest_empty_digestType)
    {
        // Empty digest type should return empty (doesn't match MD5 or MD4)
        auto result = EncryptionUtils::calcDigest("", "test");
        EXPECT_TRUE(result.empty());
    }

    TEST(EncryptionUtils, calcDigest_SHA256_not_supported)
    {
        // SHA256 is not in the MD5/MD4 check, so it should return empty
        auto result = EncryptionUtils::calcDigest("SHA256", "test");
        EXPECT_TRUE(result.empty());
    }
#endif

    // ---- HMAC wchar_t with empty inputs ----

    TEST(EncryptionUtils, HMAC_wchar_empty_message)
    {
        std::wstring emptyMsg {};
        std::string  key {"somekey"};
        auto         result = EncryptionUtils::HMAC<wchar_t>(emptyMsg, key);
        EXPECT_TRUE(result.empty());
    }

    TEST(EncryptionUtils, HMAC_wchar_empty_key)
    {
        std::wstring msg {L"hello"};
        std::string  emptyKey {};
        auto         result = EncryptionUtils::HMAC<wchar_t>(msg, emptyKey);
        EXPECT_TRUE(result.empty());
    }

    // ---- JWTHMAC256 wchar_t with different payload ----

    TEST(EncryptionUtils, JWTHMAC256_wchar_different_payload)
    {
        std::wstring header {LR"({"alg":"HS256","typ":"JWT"})"};
        std::wstring payload {LR"({"sub":"1234567890","name":"John Doe","iat":1516239022})"};
        std::string  secret {"your-256-bit-secret"};

        auto jwt = EncryptionUtils::JWTHMAC256(secret, header, payload);
        // JWT should have exactly 3 parts separated by dots
        auto firstDot  = jwt.find(L'.');
        auto secondDot = jwt.find(L'.', firstDot + 1);
        EXPECT_NE(std::wstring::npos, firstDot);
        EXPECT_NE(std::wstring::npos, secondDot);
        EXPECT_EQ(std::wstring::npos, jwt.find(L'.', secondDot + 1));
    }
} // namespace siddiqsoft
