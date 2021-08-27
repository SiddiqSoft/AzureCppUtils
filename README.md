Azure C++ Utils : Azure REST API Helpers for Modern C++
-------------------------------------------
<!-- badges -->
[![CodeQL](https://github.com/SiddiqSoft/azure-cpp-utils/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/azure-cpp-utils/actions/workflows/codeql-analysis.yml)
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.azure-cpp-utils?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=16&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.AzureCppUtils)
![](https://img.shields.io/github/v/tag/SiddiqSoft/azure-cpp-utils)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/16)
![](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/16)
<!-- end badges -->

# Objective

- Collect useful helpers for Azure REST API in a single location.
- Header-only
- Use Win32 functions on Windows

## Requirements
- The build and tests are for Visual Studio 2019 under x64.

## Usage
- Use the nuget [SiddiqSoft.AzureCppUtils](https://www.nuget.org/packages/SiddiqSoft.AzureCppUtils/)
- Copy paste..whatever works.

# Features 

- DateUtils
  - RFC7231 and RFC1123
  - ISO8601
- ConversionUtils
  - utf8fromWide
  - wideFromUtf8
- Bas64Utils
  - encode
  - decode
- EncryptionUtils
  - MD5
  - HMAC
  - JWTSHA256
  - SASToken
  - CosmosToken  

> All of the functions support string and wstring.
> *However*, the EncryptionUtils have disabled the support for wstring as it is 

## API


```cpp
struct ConversionUtils
{
    string  asciiFromWide(const wstring& src);
    string  utf8FromWide( const wstring& src);
    wstring wideFromUtf8( const string&  src);
    wstring wideFromAscii(const string&  src);
}

struct DateUtils
{
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> ISO8601(const chrono::system_clock::time_point& rawtp = chrono::system_clock::now())

    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> RFC7231(const chrono::system_clock::time_point& rawtp = chrono::system_clock::now())
}


struct Base64Utils
{
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> urlEscape(const basic_string<T>& src)

    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> encode(const basic_string<T>& argBin)

    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> decode(const basic_string<T>& argEncoded)

}


struct UrlUtils
{
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> encode(const basic_string<T>& source, bool lowerCase = false)
}


struct EncryptionUtils
{
    // Always returns a "binary" in std::string
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    string MD5(const basic_string<T>& source)

    // Always returns a "binary" in std::string
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    string HMAC(const basic_string<T>& message,
                const string& key)  // "binary" or decoded

    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T>
    JWTHMAC256(const string& key, // "binary" or decoded
               const basic_string<T>& header,
               const basic_string<T>& payload)

    // Returns tokens that can be used in the HTTP request header
    // Note that the key is always in std::string since they are decoded from base64 values from the Azure portal.
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> SASToken(const string&          key, // "binary" or decoded
                             const basic_string<T>& url,
                             const basic_string<T>& keyName,
                             const chrono::seconds& timeout)


    // Returns tokens that can be used in the HTTP request header
    // Note that the key is always in std::string since they are decoded from base64 values from the Azure portal.
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> SASToken(const string&          key, // "binary" or decoded
                             const basic_string<T>& url,
                             const basic_string<T>& keyName,
                             const basic_string<T>& expiry)

    // Returns tokens that can be used in the HTTP request header
    // Note that the key is always in std::string since they are decoded from base64 values from the Azure portal.
    template <typename T = char>
        requires same_as<T, char> || same_as<T, wchar_t>
    basic_string<T> CosmosToken(const string&          key, // "binary" or decoded
                                const basic_string<T>& verb,
                                const basic_string<T>& type,
                                const basic_string<T>& resourceLink,
                                const basic_string<T>& date)
}

```


<p align="right">
&copy; 2021 Siddiq Software LLC. All rights reserved.
</p>
