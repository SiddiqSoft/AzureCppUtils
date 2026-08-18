# Azure C++ Utils

**Azure REST API Helpers for Modern C++**

<div class="badge-container">
  <a href="https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=16&branchName=main"><img src="https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.AzureCppUtils?branchName=main" alt="Build Status" /></a>
  <a href="https://www.nuget.org/packages/SiddiqSoft.AzureCppUtils/"><img src="https://img.shields.io/nuget/v/SiddiqSoft.AzureCppUtils" alt="NuGet" /></a>
  <a href="https://github.com/SiddiqSoft/AzureCppUtils/tags"><img src="https://img.shields.io/github/v/tag/SiddiqSoft/AzureCppUtils" alt="GitHub Tag" /></a>
  <img src="https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/16" alt="Tests" />
  <img src="https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/16" alt="Coverage" />
</div>

Azure C++ Utils is a high-performance, header-only Modern C++23 library providing essential utilities for communicating with Azure REST APIs (Service Bus, Cosmos DB, Event Grid, Event Hubs, and Storage).

---

## Key Design Goals

- **Header-Only Architecture**: Zero building or linking step required; just include headers and target C++23.
- **Native OS Security Providers**: Uses native Win32 CryptoAPI/BCrypt on Windows and OpenSSL v3 on Linux/macOS.
- **Azure REST API Optimization**: Tailored specifically for generating authorization signatures (SAS Tokens, Cosmos DB Master Key tokens, HMAC-SHA256, MD5) and formatted date strings (RFC 7231, ISO 8601).
- **Narrow and Wide String Support**: Flexibly supports `std::string` (UTF-8) and `std::wstring` where applicable.

---

## Quick Start Examples

=== "Azure SAS Token"

    Generate a Shared Access Signature (SAS) token for Azure Service Bus or Event Hubs:

    ```cpp
    #include <iostream>
    #include <chrono>
    #include "siddiqsoft/encryption-utils.hpp"

    int main()
    {
        std::string keyName = "RootManageSharedAccessKey";
        std::string key     = "myPrimaryKeySecretBase64String=";
        std::string url     = "mynamespace.servicebus.windows.net/myqueue";

        // Generate token valid for 60 minutes from now
        auto sasToken = siddiqsoft::EncryptionUtils::SASToken<char>(
            key, url, keyName, std::chrono::minutes(60)
        );

        std::cout << "Authorization: " << sasToken << std::endl;
        return 0;
    }
    ```

=== "RFC 7231 & ISO 8601 Dates"

    Format date/time stamps required by Azure REST HTTP headers:

    ```cpp
    #include <iostream>
    #include "siddiqsoft/date-utils.hpp"

    int main()
    {
        // Generate HTTP RFC7231 date string ("Tue, 01 Nov 2022 08:12:31 GMT")
        std::string rfcDate = siddiqsoft::DateUtils::RFC7231();

        // Generate ISO8601 date string ("2022-11-01T08:12:31.123Z")
        std::string isoDate = siddiqsoft::DateUtils::ISO8601();

        std::cout << "x-ms-date: " << rfcDate << std::endl;
        std::cout << "ISO 8601:  " << isoDate << std::endl;
        return 0;
    }
    ```

=== "Base64 & URL Encoding"

    Encode binary buffers or strings into RFC 4648 URL-safe Base64:

    ```cpp
    #include <iostream>
    #include "siddiqsoft/base64-utils.hpp"
    #include "siddiqsoft/url-utils.hpp"

    int main()
    {
        std::string rawData = "Hello Azure REST API!";
        
        // Base64 encode
        std::string b64 = siddiqsoft::Base64Utils::encode(rawData);
        
        // URL-escape Base64 for query strings
        std::string safeB64 = siddiqsoft::Base64Utils::urlEscape(b64);

        std::cout << "Base64:     " << b64 << std::endl;
        std::cout << "URL Escaped:" << safeB64 << std::endl;
        return 0;
    }
    ```

---

## Requirements

| Requirement | Specification |
| :--- | :--- |
| **Language Standard** | Modern C++23 (`-std=c++23` or `/std:c++latest`) |
| **Compilers Supported** | MSVC 2019 (v16.11+), GCC 14+, Clang 18+ |
| **Windows Dependencies** | Win32 Cryptography APIs (`Crypt32.lib`, `Bcrypt.lib`) |
| **Linux / macOS Dependencies** | OpenSSL v3.x (`libcrypto`) |

---

## Documentation Quick Links

<div class="grid">
  <div class="card">
    <h3><a href="features/">Features & Guides</a></h3>
    <p>Explore detailed guides for Date Utilities, Base64 conversion, Encryption, SAS Tokens, and URL encoding.</p>
  </div>
  <div class="card">
    <h3><a href="integration/">Integration Guide</a></h3>
    <p>Integrate AzureCppUtils via CMake (CPM.cmake, FetchContent) or NuGet package manager.</p>
  </div>
  <div class="card">
    <h3><a href="api/">API Reference</a></h3>
    <p>Comprehensive struct definitions, method signatures, parameter descriptions, and return types.</p>
  </div>
</div>
