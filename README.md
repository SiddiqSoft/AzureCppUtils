Azure C++ Utils : Azure REST API Helpers for Modern C++
-------------------------------------------
<!-- badges -->
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.AzureCppUtils?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=16&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.AzureCppUtils)
![](https://img.shields.io/github/v/tag/SiddiqSoft/AzureCppUtils)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/16)
![](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/16)
<!-- end badges -->

# Objective

- Collect useful helpers for Azure REST API in a single location.
- Header-only
  - Use the nuget package or CMakeLists to import into your project.
- Use Win32 functions on Windows
  - No support for Linux/Darwin!

## Usage
- Use the nuget [SiddiqSoft.AzureCppUtils](https://www.nuget.org/packages/SiddiqSoft.AzureCppUtils/)
- Use the CPM in your CMakeLists file.

```cmake
..
.. # import the CPM.cmake module and activate..
..
CPMAddModule("gh:siddiqsoft/AzureCppUtils#1.3.1.1")
..
..
target_link_libraries(your-project PRIVATE AzureCppUtils::AzureCppUtils)
```

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

<p align="right">
&copy; 2021 Siddiq Software LLC. All rights reserved.
</p>
