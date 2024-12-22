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
- Requires C++20
  - Clang 18+ or GCC 14+ or MSVC 2019+
- Header-only
  - Use the nuget package or CMakeLists to import into your project.
- Use Win32 functions on Windows
- OpenSSL v3.x required for Linux and MacOS


# Features 
> Not every facility of OpenSSL or Crypto lib is provided here. Our focus is to curate code that is of most use when using the Azure ServiceBus, Cosmos, Events REST API.
> Moreover, the API focusses on the `std::string` and while we've attempted to provide for an API that can be used with `std::wstring` the later is utf-8 conversion from `std::string`.

- DateUtils (`date-utils.hpp`)
  - RFC7231 and RFC1123
  - ISO8601
- Bas64Utils (`base64-utils.hpp`)
  - encode, decode
- EncryptionUtils (`encryption-utils.hpp`)
  - MD5, HMAC, JWTSHA256, SASToken, CosmosToken  

## Usage
- Use the nuget [SiddiqSoft.AzureCppUtils](https://www.nuget.org/packages/SiddiqSoft.AzureCppUtils/)
- Use the CPM in your CMakeLists file.

```cmake
..
.. # import the CPM.cmake module and activate..
..
CPMAddModule("gh:siddiqsoft/AzureCppUtils#3.0.0")
..
..
target_link_libraries(your-project PRIVATE AzureCppUtils::AzureCppUtils)
```

```cpp
// Get a SAS signature for ServiceBus API
std::string keyname {"RootManageSharedAccessKey"};
std::string key {"myPrimaryKey"};
std::string url {"myNamespace.servicebus.windows.net/myEventHub"};
// In this test, we're providing the epoch as the expiry string: 1629608276.
// corresponds to Sunday, August 22, 2021 4:57:56 AM UTC
auto sas = EncryptionUtils::SASToken<char>(key, url, keyname, "1629608276");
// Alternatively, you can specify minutes, hours from "now"
auto sas = EncryptionUtils::SASToken<char>(key, url, keyname, std::chrono::minutes(60));
```

<p align="right">
&copy; 2021 Siddiq Software. All rights reserved.
</p>
