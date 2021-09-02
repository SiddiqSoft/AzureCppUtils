/*
    azure-cpp-utils : Azure REST API Utilities for Modern C++

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
#include <ranges>
#include <concepts>
#include <format>

#if defined(_WIN64) || defined(WIN64) || defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt")
#pragma comment(lib, "crypt32")
#endif

#include "siddiqsoft/RunOnEnd.hpp"

/// @brief SiddiqSoft
namespace siddiqsoft
{
#if !defined(_NORW)
	/// @brief In support of the macro NORW which allows us to declare/use narrow/wide strings as needed. Plucked from the MS stl
	/// implementation
	template <typename _NorWT>
		requires std::same_as<_NorWT, char> || std::same_as<_NorWT, wchar_t>
	[[nodiscard]] constexpr const _NorWT* NorW_1(const char* const _Str, const wchar_t* const _WStr) noexcept
	{
		if constexpr (std::is_same_v<_NorWT, char>) {
			return _Str;
		}
		else {
			return _WStr;
		}
	}
#define _NORW(_NorWT, _Literal) NorW_1<_NorWT>(_Literal, L##_Literal)
#endif

	/// @brief Conversion Functions for ascii to wide, utf8 to wide and vice-versa
	struct ConversionUtils
	{
		/// @brief Convert given wide string to ascii encoded string
		/// @param src std::wstring input
		/// @return std::string with ascii encoding
		static std::string asciiFromWide(const std::wstring& src)
		{
			if (src.empty()) return {};

			if (auto destSize = WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), NULL, 0, NULL, NULL);
			    destSize > 0) {
				// Allocate appropriate buffer +1 for null-char
				std::vector<char> destBuffer(static_cast<size_t>(destSize) + 1);
				destSize = WideCharToMultiByte(CP_ACP,
				                               0,
				                               src.c_str(),
				                               static_cast<int>(src.length()),
				                               destBuffer.data(),
				                               static_cast<DWORD>(destSize),
				                               NULL,
				                               NULL);
				return {destBuffer.data(), static_cast<size_t>(destSize)};
			}

			// Fall-through error -> empty string
			return {};
		}

		/// @brief Convert given wide string to utf8 encoded string
		/// @param src std::wstring input
		/// @return std::string with utf-8 encoding
		static std::string utf8FromWide(const std::wstring& src)
		{
			if (src.empty()) return {};

			if (auto destSize = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), NULL, 0, NULL, NULL);
			    destSize > 0) {
				// Allocate appropriate buffer +1 for null-char
				std::vector<char> destBuffer(static_cast<size_t>(destSize) + 1);
				destSize = WideCharToMultiByte(CP_UTF8,
				                               0,
				                               src.c_str(),
				                               static_cast<int>(src.length()),
				                               destBuffer.data(),
				                               static_cast<DWORD>(destSize),
				                               NULL,
				                               NULL);
				return {destBuffer.data(), static_cast<size_t>(destSize)};
			}

			// Fall-through error -> empty string
			return {};
		}

		/// @brief Given a utf-8 encoded string returns a utf-16 in std::wstring
		/// @param src Utf-8 encoded string
		/// @return Utf-16 encoded wstring
		static std::wstring wideFromUtf8(const std::string& src)
		{
			if (src.empty()) return {};

			if (auto destSize = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), NULL, 0); destSize > 0)
			{
				// Allocate appropriate buffer +1 for null-char
				std::vector<wchar_t> destBuffer(static_cast<size_t>(destSize) + 1);
				destSize = MultiByteToWideChar(
				        CP_UTF8, 0, src.c_str(), static_cast<int>(src.length()), destBuffer.data(), static_cast<DWORD>(destSize));
				return {destBuffer.data(), static_cast<size_t>(destSize)};
			}

			// Fall-through error -> empty string
			return {};
		}


		/// @brief Given an ascii encoded string returns a utf-16 in std::wstring
		/// @param src ascii encoded string
		/// @return Utf-16 encoded wstring
		static std::wstring wideFromAscii(const std::string& src)
		{
			if (src.empty()) return {};

			if (auto destSize = MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), NULL, 0); destSize > 0)
			{
				// Allocate appropriate buffer +1 for null-char
				std::vector<wchar_t> destBuffer(static_cast<size_t>(destSize) + 1);
				destSize = MultiByteToWideChar(
				        CP_ACP, 0, src.c_str(), static_cast<int>(src.length()), destBuffer.data(), static_cast<DWORD>(destSize));
				return {destBuffer.data(), static_cast<size_t>(destSize)};
			}

			// Fall-through error -> empty string
			return {};
		}
	};


	/// @brief Date Time utilities for REST API
	struct DateUtils
	{
		/// @brief Converts the argument to ISO8601 format
		/// @tparam T Must be string or wstring
		/// @param rawtp time_point representing the time. Defaults to "now"
		/// @return String representing ISO 8601 format with milliseconds empty if fails
		/// @see The specification https://en.wikipedia.org/wiki/ISO_8601
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> ISO8601(const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now())
		{
			auto rawtime = std::chrono::system_clock::to_time_t(rawtp);
			tm   timeInfo {};
			// We need to get the fractional milliseconds from the raw time point.
			auto msTime = std::chrono::duration_cast<std::chrono::milliseconds>(rawtp.time_since_epoch()).count() % 1000;
			// Get the UTC time packet.
			auto ec = gmtime_s(&timeInfo, &rawtime);

			if constexpr (std::is_same_v<T, char>) {
				// https://en.wikipedia.org/wiki/ISO_8601
				// yyyy-mm-ddThh:mm:ss.mmmZ
				char buff[_countof("yyyy-mm-ddThh:mm:ss.0000000Z")] {};

				if (ec != EINVAL) strftime(buff, _countof(buff), "%FT%T", &timeInfo);
				return std::format("{}.{:03}Z", buff, msTime);
			}
			else if constexpr (std::is_same_v<T, wchar_t>) {
				// yyyy-mm-ddThh:mm:ss.mmmZ
				wchar_t buff[256] {};
				if (ec != EINVAL) wcsftime(buff, _countof(buff), L"%FT%T", &timeInfo);
				return std::format(L"{}.{:03}Z", buff, msTime);
			}

			return {};
		}


		/// @brief Build a time and date string compliant with the RFC7231
		/// @tparam T Must be either std::string or std::wstring
		/// @param rawtp Optional. Uses current time if not provided
		/// @return Returns a string representation of the form: "Tue, 01 Nov 1994 08:12:31 GMT"
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> RFC7231(const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now())
		{
			auto rawtime = std::chrono::system_clock::to_time_t(rawtp);
			tm   timeInfo {};

			// Get the UTC time packet.
			auto ec = gmtime_s(&timeInfo, &rawtime);

			// HTTP-date as per RFC 7231:  Tue, 01 Nov 1994 08:12:31 GMT
			// Note that since we are getting the UTC time we should not use the %z or %Z in the strftime format
			// as it returns the local timezone and not GMT.
			if constexpr (std::is_same_v<T, char>) {
				char buff[sizeof "Tue, 01 Nov 1994 08:12:31 GMT"] {};
				if (ec != EINVAL) strftime(buff, _countof(buff), "%a, %d %h %Y %T GMT", &timeInfo);

				return buff;
			}

			if constexpr (std::is_same_v<T, wchar_t>) {
				wchar_t buff[sizeof L"Tue, 01 Nov 1994 08:12:31 GMT"] {};
				if (ec != EINVAL) wcsftime(buff, _countof(buff), L"%a, %d %h %Y %T GMT", &timeInfo);

				return buff;
			}
		}
	};


	/// @brief Base64 encode/decode functions
	struct Base64Utils
	{
		/// @brief URL escape the base64 encoded string
		/// @tparam T std::string or std::wstring
		/// @param src The source MUST be a base64 encoded string
		/// @return url encoded base64 source string
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> urlEscape(const std::basic_string<T>& src)
		{
			if (!src.empty()) {
				auto encodeVal {src};

				// Make the value url-safe per https://tools.ietf.org/html/rfc4648#section-5
				if constexpr (std::is_same_v<T, char>) {
					std::ranges::for_each(encodeVal, [](T& ch) {
						if (ch == '+')
							ch = '-';
						else if (ch == '/')
							ch = '_';
					});
					encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '\r'), encodeVal.end());
					encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '\n'), encodeVal.end());
					encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), '='), encodeVal.end());

					return encodeVal;
				}
				if constexpr (std::is_same_v<T, wchar_t>) {
					std::ranges::for_each(encodeVal, [](T& ch) {
						if (ch == L'+')
							ch = L'-';
						else if (ch == L'/')
							ch = L'_';
					});
					encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), L'\r'), encodeVal.end());
					encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), L'\n'), encodeVal.end());
					encodeVal.erase(std::remove(encodeVal.begin(), encodeVal.end(), L'='), encodeVal.end());

					return encodeVal;
				}
			}

			return {};
		}


		/// @brief Base64 encode a given "binary" string and optionally url escape
		/// @param argBin The bytes to encode
		/// @return Base64 encoded string
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> encode(const std::basic_string<T>& argBin)
		{
			DWORD destSize = 0;

			/*
			 * NOTE: Cannot fold after the std::invoke() or std::apply() as the signature varies between the two functions.
			 * std::invoke( std::is_same_v<T,wchar_t> ? CryptBinaryToStringW : CryptBinaryToStringA, ... )
			 * fails to compile since the member types differ and it cannot cast the return into a single type.
			 */
			if (!argBin.empty() &&
			    (TRUE == std::is_same_v<T, wchar_t> ? CryptBinaryToStringW((const BYTE*)(argBin.data()),
			                                                               static_cast<DWORD>(argBin.length() * sizeof(T)),
			                                                               CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
			                                                               NULL,
			                                                               &destSize)
			                                        : CryptBinaryToStringA((const BYTE*)(argBin.data()),
			                                                               static_cast<DWORD>(argBin.length() * sizeof(T)),
			                                                               CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
			                                                               NULL,
			                                                               &destSize)))
			{
				std::vector<T> dest(static_cast<size_t>(destSize) + 1);

				if (TRUE == std::is_same_v<T, wchar_t> ? CryptBinaryToStringW((const BYTE*)(argBin.data()),
				                                                              static_cast<DWORD>(argBin.length() * sizeof(T)),
				                                                              CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
				                                                              reinterpret_cast<LPWSTR>(dest.data()),
				                                                              &destSize)
				                                       : CryptBinaryToStringA((const BYTE*)(argBin.data()),
				                                                              static_cast<DWORD>(argBin.length() * sizeof(T)),
				                                                              CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
				                                                              reinterpret_cast<LPSTR>(dest.data()),
				                                                              &destSize))
				{
					return std::basic_string<T> {reinterpret_cast<T*>(dest.data()), destSize};
				}
			}

			// Fall-through is failure; return empty string
			return {};
		}


		/// @brief Base64 decode the given encoded string back to the binary value
		/// @param argEncoded Previously encoded value
		/// @return Base64 decoded string
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> decode(const std::basic_string<T>& argEncoded)
		{
			DWORD destSize = 0;

			if constexpr (std::is_same_v<T, char>) {
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
					std::vector<std::byte> dest(destSize * sizeof(T));
					if (TRUE == CryptStringToBinaryA(argEncoded.data(),
					                                 static_cast<DWORD>(argEncoded.length()),
					                                 CRYPT_STRING_BASE64 | CRYPT_STRING_STRICT,
					                                 reinterpret_cast<BYTE*>(dest.data()),
					                                 &destSize,
					                                 NULL,
					                                 NULL))
					{
						return std::basic_string<T>(reinterpret_cast<T*>(dest.data()), destSize);
					}
				}
			}
			else if constexpr (std::is_same_v<T, wchar_t>) {
				// Find out the destination size
				if (!argEncoded.empty() && (TRUE == CryptStringToBinaryW(argEncoded.data(),
				                                                         static_cast<DWORD>(argEncoded.length()),
				                                                         CRYPT_STRING_BASE64 | CRYPT_STRING_STRICT,
				                                                         NULL,
				                                                         &destSize,
				                                                         NULL,
				                                                         NULL)))
				{
					// This call will size the buffer and put '\0` to ensure we don't overrun the buffer.
					std::vector<T> dest(destSize);
					if (TRUE == CryptStringToBinaryW(argEncoded.data(),
					                                 static_cast<DWORD>(argEncoded.length()),
					                                 CRYPT_STRING_BASE64 | CRYPT_STRING_STRICT,
					                                 reinterpret_cast<BYTE*>(dest.data()),
					                                 &destSize,
					                                 NULL,
					                                 NULL))
					{
						// The function returns number of bytes which means we need to divide by the sizeof(T)
						// in order to get the number of actual T-elements in the final string.
						return std::basic_string<T>(reinterpret_cast<T*>(dest.data()), destSize / sizeof(T));
					}
				}
			}

			// Fall-through is failure; return empty string
			return {};
		}
	};


	/// @brief Url encode function
	struct UrlUtils
	{
		/// @brief Helper to encode the given string in context of the HTTP url
		/// @param source Source string
		/// @param lowerCase Default false --> all conversions to uppercase
		/// @return Encoded string with uppercase replacement
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> encode(const std::basic_string<T>& source, bool lowerCase = false)
		{
			std::basic_string<T> retOutput {};

			std::ranges::for_each(source, [&retOutput, &lowerCase](auto ch) {
				switch (ch) {
					case 0x20:
					case 0x22:
					case 0x23:
					case 0x24:
					case 0x25:
					case 0x26:
					case 0x27:
					case 0x3c:
					case 0x3e:
					case 0x7b:
					case 0x7d:
					case 0x2f:
					case 0x5c:
					case 0x40:
					case 0x7e:
					case 0x7c:
					case 0x2c:
					case 0x2b:
					case 0x3a:
					case 0x5b:
					case 0x5d:
					case 0x3f:
					case 0x3d:
					case 0x60:
						std::format_to(std::back_inserter(retOutput), lowerCase ? _NORW(T, "%{:02x}") : _NORW(T, "%{:02X}"), ch);
						break;
					default: std::format_to(std::back_inserter(retOutput), _NORW(T, "{}"), ch);
				};
			});

			return retOutput;
		}
	};


	/// @brief Encryption utility functions for ServiceBus, Cosmos, EventGrid, EventHub
	/// Implementation Note!
	/// The support for wstring is for completeness and typically the use-case is where we
	/// fiddle with utf8 data and not utf16 over the internet and especially json documents!
	struct EncryptionUtils
	{
		/// @brief Create a MD5 hash for the given source as a string
		/// @param source Maybe std::string or std::wstring
		/// @return MD5 of the source argument empty if there is a failure
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::string MD5(const std::basic_string<T>& source)
		{
			if constexpr (std::is_same_v<T, char>) {
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
						    CryptHashData(
						            hHash, reinterpret_cast<const BYTE*>(source.data()), static_cast<DWORD>(source.length()), 0)) {
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
			}
			else {
				// The MD5 result is a "binary" so we must not try and convert.
				return MD5<char>(ConversionUtils::utf8FromWide(source));
			}

			// Fall-through failure
			return {};
		}


		/// @brief Returns binary HMAC using SHA-256.
		/// https://www.liavaag.org/English/SHA-Generator/HMAC/
		/// @param message Source text
		/// @param key Source key; MUST NOT be base64 encoded
		/// @return Binary enclosed in string (narrow); you must base64encode to get useful result.
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::string HMAC(const std::basic_string<T>& message, const std::string& key)
		{
			if constexpr (std::is_same_v<T, char>) {
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
								std::vector<BYTE> pbHash(cbHash);
								// Fetch the hash value
								if (status = BCryptFinishHash(hHash, reinterpret_cast<UCHAR*>(pbHash.data()), cbHash, 0);
								    status == 0) {
									// Return the HMAC as a raw binary..client must choose to encode or leave as-is
									return std::string {reinterpret_cast<char*>(pbHash.data()), cbHash};
								}
							}
						}
					}
				}
			}
			else {
				// The HMAC result is "binary" and must not be treated as wstring
				return HMAC<char>(ConversionUtils::asciiFromWide(message), key);
			}

			// Fall-through is failure
			return {};
		}


		/// @brief Create a JsonWebToken authorization with HMAC 256
		/// @param key Must be std::string as the contents are the "key" and treated as "binary"
		/// @param header The JWT header
		/// @param payload The string wiht json tokens
		/// @return HMAC256 encoded JWT token
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T>
		JWTHMAC256(const std::string& key, const std::basic_string<T>& header, const std::basic_string<T>& payload)
		{
			if constexpr (std::is_same_v<T, char>) {
				auto s1        = Base64Utils::urlEscape<char>(Base64Utils::encode<char>(header));
				auto s2        = Base64Utils::urlEscape<char>(Base64Utils::encode<char>(payload));
				auto a3        = std::format("{}.{}", s1, s2);
				auto a4        = HMAC<char>(a3, key);
				auto signature = Base64Utils::urlEscape<char>(Base64Utils::encode<char>(a4));

				return std::format("{}.{}.{}", s1, s2, signature);
			}
			else {
				// Delegate to the narrow version; conversion at the edge
				return ConversionUtils::wideFromUtf8(
				        JWTHMAC256<char>(key, ConversionUtils::utf8FromWide(header), ConversionUtils::utf8FromWide(payload)));
			}
		}


		/// @brief Create a Shared Access Signature for Azure storage
		/// https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token
		/// @param key The key is "binary" in std::string
		/// @param url The url for the session
		/// @param keyName The key name
		/// @param timeout Amount of ticks past the "now" (number of seconds since 1970-1-1)
		/// @return SAS token
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> SASToken(const std::string&          key,
		                                     const std::basic_string<T>& url,
		                                     const std::basic_string<T>& keyName,
		                                     const std::chrono::seconds& timeout)
		{
			time_t epoch {};

			time(&epoch); // number of seconds since 1970-1-1

			return SASToken<T>(key,
			                   url,
			                   keyName,
			                   std::is_same_v<T, wchar_t> ? std::to_wstring(int64_t(epoch) + timeout.count())
			                                              : std::to_string(int64_t(epoch) + timeout.count()));
		}


		/// @brief Create a Shared Access Signature for Azure storage
		/// https://docs.microsoft.com/en-us/rest/api/eventhub/generate-sas-token
		/// @param key The key to sign the url with time
		/// @param url The url for the session
		/// @param keyName The key name
		/// @param expiry The expiry is the number of seconds since 1970-1-1 plus timeout
		/// @return SAS token
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> SASToken(const std::string&          key,
		                                     const std::basic_string<T>& url,
		                                     const std::basic_string<T>& keyName,
		                                     const std::basic_string<T>& expiry)
		{
			if (url.empty()) throw std::invalid_argument("SASToken: url may not be empty");
			if (keyName.empty()) throw std::invalid_argument("SASToken: keyName may not be empty");
			if (key.empty()) throw std::invalid_argument("SASToken: key may not be empty");
			if (expiry.empty()) throw std::invalid_argument("SASToken: expiry may not be empty");

			if constexpr (std::is_same_v<T, char>) {
				auto s1    = UrlUtils::encode<char>(url, true); // lowercase
				auto sig   = HMAC<char>(std::format("{}\n{}", s1, expiry), key);
				auto esign = Base64Utils::encode<char>(sig);
				esign      = UrlUtils::encode<char>(esign, true); // lowercase

				return std::format("SharedAccessSignature sr={}&sig={}&se={}&skn={}", s1, esign, expiry, keyName);
			}
			else {
				// Delegate to the narrow version and convert at the edges.
				return ConversionUtils::wideFromUtf8(SASToken<char>(key,
				                                                    ConversionUtils::utf8FromWide(url),
				                                                    ConversionUtils::utf8FromWide(keyName),
				                                                    ConversionUtils::utf8FromWide(expiry)));
			}
		}


		/// @brief Create the Cosmos Authorization Token using the Key for this connection.
		/// @param key Binary. The key must be decoded from the base64 value in the connection string from the Azure portal
		/// @param verb GET, POST, PUT, DELETE
		/// @param type One of the following: dbs, docs, colls, attachments or empty
		/// @param resourceLink The resource link sub-uri
		/// @param date Date in RFC7231 as string
		/// @return Cosmos Authorization signature as std::string. It is base64 encoded and urlsafe
		template <typename T = char>
			requires std::same_as<T, char> || std::same_as<T, wchar_t>
		static std::basic_string<T> CosmosToken(const std::string&          key,
		                                        const std::basic_string<T>& verb,
		                                        const std::basic_string<T>& type,
		                                        const std::basic_string<T>& resourceLink,
		                                        const std::basic_string<T>& date)
		{
			if (key.empty()) throw std::invalid_argument("CosmosToken: key may not be empty");
			if (date.empty()) throw std::invalid_argument("CosmosToken: date may not be empty");
			if (verb.empty()) throw std::invalid_argument("CosmosToken: verb may not be empty");

			if constexpr (std::is_same_v<T, char>) {
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
					if (auto hmacBase64UrlEscaped = UrlUtils::encode<char>(
					            Base64Utils::encode<char>(EncryptionUtils::HMAC<char>(strToHash, key)), true);
					    !hmacBase64UrlEscaped.empty())
					{
						return std::format("type%3dmaster%26ver%3d1.0%26sig%3d{}", hmacBase64UrlEscaped);
					}
				}
			}
			else {
				// Delegate to the narrow version, conversion at the edges.
				return ConversionUtils::wideFromAscii(CosmosToken<char>(key,
				                                                        ConversionUtils::asciiFromWide(verb),
				                                                        ConversionUtils::asciiFromWide(type),
				                                                        ConversionUtils::asciiFromWide(resourceLink),
				                                                        ConversionUtils::asciiFromWide(date)));
			}

			// Fall-through failure
			return {};
		}
	};
} // namespace siddiqsoft

#endif // !AZURECPPUTILS_HPP
