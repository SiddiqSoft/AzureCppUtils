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
    SERVICES; LOSS OF USE, d_, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#ifndef AZURECPPUTILS_HPP
#define AZURECPPUTILS_HPP


#include <algorithm>
#include <iostream>
#include <chrono>
#include <string>
#include <functional>
#include <memory>
#include <format>

#include <Windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt")
#pragma comment(lib, "crypt32")

#include "siddiqsoft/RunOnEnd.hpp"


namespace siddiqsoft
{
	struct DateUtils
	{
		/// @brief Converts the argument to ISO8601 format
		/// @tparam T Must be string or wstring
		/// @param rawTime time_point representing the time. Defaults to "now"
		/// @return String representing ISO 8601 format with milliseconds empty if fails
		template <class T = std::string>
		static T ISO8601(const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now())
		{
			auto rawtime = std::chrono::system_clock::to_time_t(rawtp);
			tm   timeInfo {};
			// We need to get the fractional milliseconds from the raw time point.
			auto msTime = std::chrono::duration_cast<std::chrono::milliseconds>(rawtp.time_since_epoch()).count() % 1000;
			// Get the UTC time packet.
			auto ec = gmtime_s(&timeInfo, &rawtime);

			if constexpr (std::is_same_v<T, std::string>) {
				// https://en.wikipedia.org/wiki/ISO_8601
				// yyyy-mm-ddThh:mm:ss.mmmZ
				char buff[sizeof "yyyy-mm-ddThh:mm:ss.0000000Z"] {};

				if (ec != EINVAL) strftime(buff, sizeof(buff), "%FT%T", &timeInfo);
				return std::format("{}.{:03}Z", buff, msTime);
			}
			else if constexpr (std::is_same_v<T, std::wstring>) {
				// https://en.wikipedia.org/wiki/ISO_8601
				// yyyy-mm-ddThh:mm:ss.mmmZ
				wchar_t buff[sizeof L"yyyy-mm-ddThh:mm:ss.0000000Z"] {};
				if (ec != EINVAL) wcsftime(buff, sizeof(buff), L"%FT%T", &timeInfo);
				return std::format(L"{}.{:03}Z", buff, msTime);
			}

			return {};
		}


		/// @brief Build a time and date string compliant with the RFC7231
		/// @tparam T Must be either std::string or std::wstring
		/// @param rawtp Optional. Uses current time if not provided
		/// @return Returns a string representation of the form: "Tue, 01 Nov 1994 08:12:31 GMT"
		template <class T = std::string>
		static T RFC7231(const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now())
		{
			auto rawtime = std::chrono::system_clock::to_time_t(rawtp);
			tm   timeInfo {};

			// Get the UTC time packet.
			auto ec = gmtime_s(&timeInfo, &rawtime);

			// HTTP-date as per RFC 7231:  Tue, 01 Nov 1994 08:12:31 GMT
			// Note that since we are getting the UTC time we should not use the %z or %Z in the strftime format
			// as it returns the local timezone and not GMT.
			if constexpr (std::is_same_v<T, std::string>) {
				char buff[sizeof "Tue, 01 Nov 1994 08:12:31 GMT"] {};
				if (ec != EINVAL) strftime(buff, sizeof(buff), "%a, %d %h %Y %T GMT", &timeInfo);

				return buff;
			}

			if constexpr (std::is_same_v<T, std::wstring>) {
				wchar_t buff[sizeof L"Tue, 01 Nov 1994 08:12:31 GMT"] {};
				if (ec != EINVAL) wcsftime(buff, sizeof(buff), L"%a, %d %h %Y %T GMT", &timeInfo);

				return buff;
			}
		}
	};


	struct Base64Utils
	{
		/// @brief Base64 encode a given "binary" string and optionally url escape
		/// @param argBin The bytes to encode
		/// @param urlEscape Optional. Url Escape the resulting string
		/// @return Base64 encoded string
		static std::string encode(const std::string& argBin, bool urlEscape = false)
		{
			DWORD destSize = 0;


			if (!argBin.empty() && (TRUE == CryptBinaryToStringA((const BYTE*)(argBin.data()),
			                                                     static_cast<DWORD>(argBin.length()),
			                                                     CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
			                                                     NULL,
			                                                     &destSize)))
			{
				auto dest = std::make_unique<std::byte[]>(destSize);
				if (TRUE == CryptBinaryToStringA((const BYTE*)(argBin.data()),
				                                 static_cast<DWORD>(argBin.length()),
				                                 CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
				                                 reinterpret_cast<char*>(dest.get()),
				                                 &destSize))
				{
					auto encodeVal = std::string(reinterpret_cast<char*>(dest.get()), destSize);

					if (urlEscape) {
						// Make the value url-safe per https://tools.ietf.org/html/rfc4648#section-5
						std::for_each(encodeVal.begin(), encodeVal.end(), [](auto& ch) {
							switch (ch) {
								case '+': ch = '-'; break;
								case '/': ch = '_'; break;
							}
						});
						encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '\r'), encodeVal.end());
						encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '\n'), encodeVal.end());
						encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '='), encodeVal.end());
					}

					return encodeVal;
				}
			}

			// Fall-through is failure; return empty string
			return {};
		}

		/// @brief Base64 decode the given encoded string back to the binary value
		/// @param argEncoded Previously encoded value
		/// @return Base64 decoded string
		static std::string decode(const std::string& argEncoded)
		{
			DWORD destSize = 0;

			// Find out the destination size
			if (!argEncoded.empty() && (TRUE == CryptStringToBinaryA(argEncoded.data(),
			                                                         static_cast<DWORD>(argEncoded.length()),
			                                                         CRYPT_STRING_BASE64 | CRYPT_STRING_STRICT,
			                                                         NULL,
			                                                         &destSize,
			                                                         NULL,
			                                                         NULL)))
			{
				// This call will size the buffer and put '\0` to ensure we don't overrun the buffer.
				auto dest = std::make_unique<std::byte[]>(destSize);
				if (TRUE == CryptStringToBinaryA(argEncoded.data(),
				                                 static_cast<DWORD>(argEncoded.length()),
				                                 CRYPT_STRING_BASE64 | CRYPT_STRING_STRICT,
				                                 reinterpret_cast<BYTE*>(dest.get()),
				                                 &destSize,
				                                 NULL,
				                                 NULL))
				{
					return std::string(reinterpret_cast<char*>(dest.get()), destSize);
				}
			}

			// Fall-through is failure; return empty string
			return {};
		}
	};


	struct UrlUtils
	{
		/// @brief Helper to encode the given string in context of the HTTP url
		/// @param source Source string
		/// @param lowerCase Default false --> all conversions to uppercase
		/// @return Encoded string with uppercase replacement
		static std::string encode(const std::string& source, bool lowerCase = false)
		{
			std::string retOutput {};
			std::ranges::for_each(source, [&retOutput, &lowerCase](auto ch) {
				switch (ch) {
					case '%': retOutput += "%25"; break;
					case ' ': retOutput += "%20"; break;
					case '&': retOutput += "%26"; break;
					case '<': retOutput += lowerCase ? "%3c" : "%3C"; break;
					case '>': retOutput += lowerCase ? "%3e" : "%3E"; break;
					case '{': retOutput += lowerCase ? "%7b" : "%7B"; break;
					case '}': retOutput += lowerCase ? "%7d" : "%7D"; break;
					case '\'': retOutput += "%27"; break;
					case '\"': retOutput += "%22"; break;
					case '/': retOutput += lowerCase ? "%2f" : "%2F"; break;
					case '\\': retOutput += lowerCase ? "%5c" : "%5C"; break;
					case '@': retOutput += "%40"; break;
					case '~': retOutput += lowerCase ? "%7e" : "%7E"; break;
					case '|': retOutput += lowerCase ? "%7c" : "%7C"; break;
					case ',': retOutput += lowerCase ? "%2c" : "%2C"; break;
					case '+': retOutput += lowerCase ? "%2b" : "%2B"; break;
					case ':': retOutput += lowerCase ? "%3a" : "%3A"; break;
					case '`': retOutput += "%60"; break;
					case '[': retOutput += lowerCase ? "%5b" : "%5B"; break;
					case ']': retOutput += lowerCase ? "%5d" : "%5D"; break;
					case '?': retOutput += lowerCase ? "%3f" : "%3F"; break;
					case '=': retOutput += lowerCase ? "%3d" : "%3D"; break;
					case '$': retOutput += "%24"; break;
					case '#': retOutput += "%23"; break;
					default: retOutput += ch;
				};
			});
			return retOutput;
		}
	};


	struct EncryptionUtils
	{
		/// @brief Create a MD5 hash for the given source
		/// @param source
		/// @return MD5 of the source argument empty if there is a failure
		static std::string MD5(const std::string& source)
		{
			HCRYPTPROV hProv {NULL};
			HCRYPTHASH hHash {NULL};
			RunOnEnd   cleanUpOnEnd {[&hProv, &hHash] {
                // Cleanup on exit of this function scope
                if (hHash != NULL) CryptDestroyHash(hHash);
                if (hProv != NULL) CryptReleaseContext(hProv, 0);
            }};

			// Get handle to the crypto provider
			if (TRUE == CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
				// Get the hash library, choose MD5..
				if (TRUE == CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
					// Hash the source..
					if (TRUE ==
					    CryptHashData(hHash, reinterpret_cast<const BYTE*>(source.data()), static_cast<DWORD>(source.length()), 0))
					{
						char  rgbDigits[] {"0123456789abcdef"};
						BYTE  rgbHash[sizeof(rgbDigits)] {};
						DWORD rgbHashSize = sizeof(rgbDigits);
						// Fetch the results using the gethashparam call..
						if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &rgbHashSize, 0)) {
							std::string result {};

							for (DWORD i = 0; i < rgbHashSize; i++) {
								std::format_to(std::back_inserter(result),
								               "{}{}",
								               rgbDigits[rgbHash[i] >> 4],
								               rgbDigits[rgbHash[i] & 0xf]);
							}

							return result;
						}
					}
				}
			}

			// Fall-through failure
			return {};
		}


		/// @brief Returns binary HMAC using SHA-256.
		/// https://www.liavaag.org/English/SHA-Generator/HMAC/
		/// @param argSource Source text
		/// @param argKey Source key; MUST NOT be base64 encoded
		/// @return Binary enclosed in string (narrow); you must base64encode to get useful result.
		static std::string HMAC(const std::string& message, const std::string& key)
		{
			BCRYPT_ALG_HANDLE  hAlg {NULL};
			BCRYPT_HASH_HANDLE hHash {NULL};
			NTSTATUS           status {0};
			RunOnEnd           cleanupOnEnd {[&hAlg, &hHash] {
                // All handles we allocate are cleaned up when this function returns to caller
                if (hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
                if (hHash) BCryptDestroyHash(hHash);
            }};


			if (status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
			    status == 0) {
				// Set the key for the hash function..
				// Passing NULL, 0 to the pbHashObject and cbHashObject asks the method to allocate
				// memory on our behalf.
				if (status = BCryptCreateHash(hAlg,
				                              &hHash,
				                              NULL,
				                              0,
				                              reinterpret_cast<UCHAR*>(const_cast<char*>(key.data())),
				                              static_cast<DWORD>(key.length()),
				                              0);
				    status == 0)
				{
					// Let's hash our message!
					if (status = BCryptHashData(hHash,
					                            reinterpret_cast<UCHAR*>(const_cast<char*>(message.data())),
					                            static_cast<DWORD>(message.length()),
					                            0);
					    status == 0)
					{
						// Get the size of the hash so we can fetch it..
						DWORD cbHash {0};
						ULONG cbData {0};
						if (status = BCryptGetProperty(
						            hAlg, BCRYPT_HASH_LENGTH, reinterpret_cast<UCHAR*>(&cbHash), sizeof(DWORD), &cbData, 0);
						    status == 0) {
							auto pbHash = std::make_unique<BYTE[]>(cbHash);
							// Fetch the hash value
							if (status = BCryptFinishHash(hHash, reinterpret_cast<UCHAR*>(pbHash.get()), cbHash, 0); status == 0) {
								// Return the HMAC as a raw binary..client must choose to encode or leave as-is
								return std::string {reinterpret_cast<char*>(pbHash.get()), cbHash};
							}
						}
					}
				}
			}

			// Fall-through is failure
			return {};
		}


		/// @brief Create a JsonWebToken authorization with HMAC 256
		/// @param secret
		/// @param header
		/// @param payload The string wiht json tokens
		/// @return
		static std::string JWTHMAC256(const std::string& secret, const std::string& header, const std::string& payload)
		{
			auto s1        = Base64Utils::encode(header, true);
			auto s2        = Base64Utils::encode(payload, true);
			auto a3        = std::format("{}.{}", s1, s2);
			auto a4        = HMAC(a3, secret);
			auto signature = Base64Utils::encode(a4, true);

			return std::format("{}.{}.{}", s1, s2, signature);
		}


		/// @brief Create a Shared Access Signature for Azure storage
		/// https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token
		/// @param url The url for the session
		/// @param keyName The key name
		/// @param key The key to sign the url with time
		/// @param timeout Amount of ticks past the "now" (number of seconds since 1970-1-1)
		/// @return SAS token
		static std::string
		SASToken(const std::string& url, const std::string& keyName, const std::string& key, const std::chrono::seconds& timeout)
		{
			time_t epoch {};

			time(&epoch); // number of seconds since 1970-1-1
			auto delta  = timeout.count();
			auto expiry = std::to_string(int64_t(epoch) + delta);

			return SASToken(url, keyName, key, expiry);
		}


		/// @brief Create a Shared Access Signature for Azure storage
		/// https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token
		/// @param url The url for the session
		/// @param keyName The key name
		/// @param key The key to sign the url with time
		/// @param expiry The expiry is the number of seconds since 1970-1-1 plus timeout
		/// @return SAS token
		static std::string
		SASToken(const std::string& url, const std::string& keyName, const std::string& key, const std::string& expiry)
		{
			if (url.empty()) throw std::invalid_argument("SASToken: url may not be empty");
			if (keyName.empty()) throw std::invalid_argument("SASToken: keyName may not be empty");
			if (key.empty()) throw std::invalid_argument("SASToken: key may not be empty");
			if (expiry.empty()) throw std::invalid_argument("SASToken: expiry may not be empty");

			auto s1    = UrlUtils::encode(url, true); // lowercase
			auto sig   = HMAC(std::format("{}\n{}", s1, expiry), key);
			auto esign = Base64Utils::encode(sig);
			esign      = UrlUtils::encode(esign, true); // lowercase

			return std::format("SharedAccessSignature sr={}&sig={}&se={}&skn={}", s1, esign, expiry, keyName);
		}


		/// @brief Create the Cosmos Authorization Token using the Key for this connection.
		/// @param key Binary. The key must be decoded from the base64 value in the connection string from the Azure portal
		/// @param verb GET, POST, PUT, DELETE
		/// @param type One of the following: dbs, docs, colls, attachments or empty
		/// @param resourceLink The resource link sub-uri
		/// @param date Date in RFC7231 as string
		/// @return Cosmos Authorization signature as std::string. It is base64 encoded and urlsafe
		static std::string CosmosToken(const std::string  key,
		                               const std::string& verb,
		                               const std::string& type,
		                               const std::string& resourceLink,
		                               const std::string& date)
		{
			if (key.empty()) throw std::invalid_argument("CosmosToken: key may not be empty");
			if (date.empty()) throw std::invalid_argument("CosmosToken: date may not be empty");
			if (verb.empty()) throw std::invalid_argument("CosmosToken: verb may not be empty");
			if (type.empty()) throw std::invalid_argument("CosmosToken: type may not be empty");
			if (resourceLink.empty()) throw std::invalid_argument("CosmosToken: resourceLink may not be empty");

			// The formula is expressed as per
			// https://docs.microsoft.com/en-us/rest/api/documentdb/access-control-on-documentdb-resources?redirectedfrom=MSDN
			std::string strToHash {};

			std::ranges::transform(verb, std::back_inserter(strToHash), [](auto& ch) { return std::tolower(ch); });
			std::format_to(std::back_inserter(strToHash), "\n");
			std::ranges::transform(type, std::back_inserter(strToHash), [](auto& ch) { return std::tolower(ch); });
			std::format_to(std::back_inserter(strToHash), "\n{}\n", resourceLink);
			std::ranges::transform(date, std::back_inserter(strToHash), [](auto& ch) { return std::tolower(ch); });
			std::format_to(std::back_inserter(strToHash), "\n\n");

			if (!strToHash.empty()) {
				// Sign using SHA256 using the master key and base64 encode. force lowercase
				if (auto hmacBase64UrlEscaped = UrlUtils::encode(Base64Utils::encode(EncryptionUtils::HMAC(strToHash, key)), true);
				    !hmacBase64UrlEscaped.empty())
				{
					return std::format("type%3dmaster%26ver%3d1.0%26sig%3d{}", hmacBase64UrlEscaped);
				}
			}

			// Fall-through failure
			return {};
		}
	};
} // namespace siddiqsoft


#endif // !AZURECPPUTILS_HPP
