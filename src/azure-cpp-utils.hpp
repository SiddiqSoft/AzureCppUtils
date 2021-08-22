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
#if USE_OPENSSL
		static std::string createMD5Hash(const std::string& source)
		{
			unsigned char digest[MD5_DIGEST_LENGTH] {};
			std::string   output {};
			MD5((unsigned char*)source.c_str(), source.length(), (unsigned char*)&digest);
			for (auto i = 0; i < 16; i++) {
				std::format_to(std::back_inserter(output), "{:02x}", digest[i]);
			}
			return output;
		}


		/// @brief Returns binary HMAC using SHA-256
		/// @param argSource Source text
		/// @param argKey Source key; MUST NOT be base64 encoded
		/// @return Binary enclosed in string (narrow); you must base64encode to get useful result.
		static std::string createHMAC(const std::string& argSource, const std::string& argKey)
		{
			if (argSource.empty() || argKey.empty()) throw std::invalid_argument("createHMAC: source and key must not be empty.");

			unsigned int  diglen = 0;
			unsigned char result[EVP_MAX_MD_SIZE] {}; // output buffer for openssl lib. 64 char max.

			unsigned char* digest = HMAC(EVP_sha256(),
			                             reinterpret_cast<const unsigned char*>(argKey.c_str()),
			                             argKey.length(),
			                             reinterpret_cast<const unsigned char*>(argSource.c_str()),
			                             argSource.length(),
			                             result,
			                             &diglen);

			return std::string(reinterpret_cast<char*>(digest), diglen);
		}
#else
		static std::string hashMD5(const std::string& source)
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
					if (TRUE == CryptHashData(hHash, reinterpret_cast<const BYTE*>(source.data()), source.length(), 0)) {
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


		/// @brief Returns binary HMAC using SHA-256
		/// @param argSource Source text
		/// @param argKey Source key; MUST NOT be base64 encoded
		/// @return Binary enclosed in string (narrow); you must base64encode to get useful result.
		static std::string HMAC(const std::string& message, const std::string& key)
		{
			BCRYPT_ALG_HANDLE  hAlg {NULL};
			BCRYPT_HASH_HANDLE hHash {NULL};
			NTSTATUS           status {0};
			auto               pbHash = std::make_unique<BYTE[]>(256);
			RunOnEnd           cleanupOnEnd {[&] {
                if (hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
                if (hHash) BCryptDestroyHash(hHash);
            }};

			if (status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
			    status == 0) {
				// Set the key for the hash function..
				// Passing NULL, 0 to the pbHashObject and cbHashObject asks the method to allocate
				// memory on our behalf.
				if (status = BCryptCreateHash(
				            hAlg, &hHash, NULL, 0, reinterpret_cast<UCHAR*>(const_cast<char*>(key.data())), key.length(), 0);
				    status == 0)
				{
					// Let's hash our message!
					if (status = BCryptHashData(
					            hHash, reinterpret_cast<UCHAR*>(const_cast<char*>(message.data())), message.length(), 0);
					    status == 0) {
						DWORD cbHash {0};
						ULONG cbData {0};
						status = BCryptGetProperty(
						        hAlg, BCRYPT_HASH_LENGTH, reinterpret_cast<UCHAR*>(&cbHash), sizeof(DWORD), &cbData, 0);
						// Fetch the hash value
						if (status = BCryptFinishHash(hHash, reinterpret_cast<UCHAR*>(pbHash.get()), cbHash, 0); status == 0) {
							// https://www.liavaag.org/English/SHA-Generator/HMAC/
							return std::string {reinterpret_cast<char*>(pbHash.get()), cbHash};
						}
					}
				}
			}

			// Fall-through is failure
			return {};
		}
#endif


		//
		// createJWTHMAC256
		// Create the signature
		// Caution using nlohman::json to dump()
		// The JSON specification does not mandate ordering of the key-value pairs. However, if you notice, the JWT base64url
		// are very sensitive to ordering.
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
			if (url.empty()) throw std::invalid_argument("createSASToken: url may not be empty");
			if (keyName.empty()) throw std::invalid_argument("createSASToken: keyName may not be empty");
			if (key.empty()) throw std::invalid_argument("createSASToken: key may not be empty");
			if (expiry.empty()) throw std::invalid_argument("createSASToken: expiry may not be empty");

			auto s1    = UrlUtils::encode(url, true); // lowercase
			auto sig   = HMAC(std::format("{}\n{}", s1, expiry), key);
			auto esign = Base64Utils::encode(sig);
			esign      = UrlUtils::encode(esign, true); // lowercase

			return std::format("SharedAccessSignature sr={}&sig={}&se={}&skn={}", s1, esign, expiry, keyName);
		}
	};
} // namespace siddiqsoft


#endif // !AZURECPPUTILS_HPP
