/*
    azure-cpp-utils : Azure Utilities for Modern C++

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
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "gtest/gtest.h"
#include <iostream>


#include "../src/azure-cpp-utils.hpp"


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


	TEST(EncryptionUtils, MD5HashTest)
	{
		std::string myMessage = "My ^&()=+-_[]{};:\'\"<>?`~ N@me is $0.50! A whole 50 off! #discount./|\\";
		auto        varKey    = EncryptionUtils::MD5(myMessage);
		EXPECT_EQ(varKey, "a2cf7440dab41a41487ec62f40d68cee");

		std::string myMessage1 = "happy";
		auto        varKey1    = EncryptionUtils::MD5(myMessage1);
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
		auto sas = EncryptionUtils::SASToken(url, keyname, secret, "1629608276");

		// clang-format off
		EXPECT_EQ("SharedAccessSignature sr=myNamespace.servicebus.windows.net%2fmyEventHub&sig=%2bl9P4WDuWjGTXrA8PeFKGl0g9Nj5OnynM55TK3huc1M%3d&se=1629608276&skn=RootManageSharedAccessKey",
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


	TEST(EncryptionUtils, HMAC_1)
	{
		std::string myData {"hello world"};
		std::string myKey {"01234567890123456789012345678901"};

		try {
			auto r1 = EncryptionUtils::HMAC(myData, myKey);

			// Using online tool: https://www.liavaag.org/English/SHA-Generator/HMAC/
			EXPECT_STREQ("B8dXpkmWppplo/hAbiHLuXgIEPFErnypOewwhH1+tPQ=", Base64Utils::encode(r1).c_str());
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
		auto auth       = EncryptionUtils::CosmosToken(decodedKey, "GET", "dbs", "dbs/ToDoList", "Thu, 27 Apr 2017 00:51:12 GMT");

		// https://docs.microsoft.com/en-us/rest/api/cosmos-db/access-control-on-cosmosdb-resources?redirectedfrom=MSDN
		EXPECT_EQ("type%3dmaster%26ver%3d1.0%26sig%3dc09PEVJrgp2uQRkr934kFbTqhByc7TVr3OHyqlu%2bc%2bc%3d", auth);
	}


	TEST(DateUtils, ISO8601_1)
	{
		auto now_ts      = std::chrono::system_clock::now();

		auto now_iso8601 = DateUtils::ISO8601(now_ts);
		std::cout << "now_iso8601   : " << now_iso8601 << std::endl;
		std::cout << "now_iso8601   : " << DateUtils::ISO8601() << std::endl;
	}


	TEST(DateUtils, RFC7231_1)
	{
		auto now_ts      = std::chrono::system_clock::now();

		auto now_rfc7231 = DateUtils::RFC7231(now_ts);
		std::cout << "now_rfc7231   : " << now_rfc7231 << std::endl;
		std::cout << "now_rfc7231   : " << DateUtils::RFC7231() << std::endl;
	}
} // namespace siddiqsoft