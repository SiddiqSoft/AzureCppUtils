@mainpage Azure C++ Utils : Azure REST API Helpers for Modern C++
@copyright &copy;2021 Siddiq Software LLC.
@author siddiqsoft


<!-- badges -->
[![CodeQL](https://github.com/SiddiqSoft/AzureCppUtils/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/AzureCppUtils/actions/workflows/codeql-analysis.yml)
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.AzureCppUtils?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=16&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.AzureCppUtils)
![](https://img.shields.io/github/v/tag/SiddiqSoft/AzureCppUtils)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/16)
![](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/16)
<!-- end badges -->

# Objective

- Collect useful helpers for Azure REST API in a single location.
- Header-only
- Use Win32 functions on Windows

## Requirements
- The build and tests are for Visual Studio 2019 v16.11 under x64.
- C++20 is required


# API 

- Use the nuget [SiddiqSoft.AzureCppUtils](https://www.nuget.org/packages/SiddiqSoft.AzureCppUtils/)
- Copy paste..whatever works.

 Collection | Description
-----------------|--------------
`siddiqsoft::ConversionUtils` | Conversion helpers (via the Win32 WideFromMultiByte methods.):<br/>convert_to<char,wchar_t>, convert_to<char,wchar_t>, convert_to<char,wchar_t>, wideFromUtf8.
`siddiqsoft::DateUtils` | Helpers for RFC7231 and RFC1123 as well as ISO8601 formatd time strings.<br/>Supports `std::string` and `std::wstring` output.
`siddiqsoft::Base64Utils` | `encode` and `decode` support for `std::string` only.
`siddiqsoft::EncryptionUtils` | Encryption helpers for:<br/>MD5, HMAC, JWTSHA256, SASToken and  CosmosToken<br/>Supports output to `std::string` and `std::wstring` containers.

> **NOTE**
> The "keys" are binary contained within a `std::string`. Do not attempt to store them by conversion to `std::wstring` as they will produce different values and will fail against Azure.
> You can take the resulting "signed" string and convert it to `std::wstring` as they are url-safe utf-8.

Project is on [GitHub](https://github.com/siddiqsoft/AzureCppUtils/)
