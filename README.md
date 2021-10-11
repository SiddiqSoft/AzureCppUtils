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

## Usage
- Use the nuget [SiddiqSoft.AzureCppUtils](https://www.nuget.org/packages/SiddiqSoft.AzureCppUtils/)
- Include the individual files of the `azure-cpp-utils.hpp` to bring all headers in your source.
- Copy paste..whatever works.

# Features 

- DateUtils (`date-utils.hpp`)
  - RFC7231 and RFC1123
  - ISO8601
- ConversionUtils (`conversion-utils.hpp`)
  - utf8fromWide, wideFromUtf8, asciiFromWide and wideFromAscii
- Bas64Utils (`base64-utils.hpp`)
  - encode, decode
- EncryptionUtils (`encryption-utils.hpp`)
  - MD5, HMAC, JWTSHA256, SASToken, CosmosToken  

The the documentation [here](https://siddiqsoft.github.io/azure-cpp-utils/html/index.html)

<p align="right">
&copy; 2021 Siddiq Software LLC. All rights reserved.
</p>
