# Feature Overview

`AzureCppUtils` provides a focused collection of header-only C++23 utilities tailored for Azure REST API operations.

---

## Core Feature Collections

| Module | Header | Key Capabilities |
| :--- | :--- | :--- |
| **Date Utilities** | `siddiqsoft/date-utils.hpp` | RFC7231, ISO8601 formatting, epoch parsing, duration strings, timespan math. |
| **Base64 Utilities** | `siddiqsoft/base64-utils.hpp` | Base64 encoding/decoding, RFC 4648 URL escaping. |
| **Encryption Utilities** | `siddiqsoft/encryption-utils.hpp` | MD5, HMAC-SHA256, JWT SHA256, Azure Service Bus SAS Tokens, Cosmos DB Tokens. |
| **URL Utilities** | `siddiqsoft/url-utils.hpp` | RFC 3986 percent-encoding for HTTP URLs. |
| **Conversion Utilities** | `siddiqsoft/conversion-utils.hpp` | Wide/narrow string conversions (`char` <-> `wchar_t`). |

---

## Feature Guides

- [Date Utilities Guide](date-utils.md): Working with HTTP RFC7231 dates and ISO8601 timestamps.
- [Base64 Utilities Guide](base64-utils.md): Encoding binary payloads and handling RFC 4648 URL safety.
- [Encryption & Tokens Guide](encryption-utils.md): Generating Azure SAS tokens, Cosmos DB tokens, and HMAC signatures.
- [URL Utilities Guide](url-utils.md): Percent-encoding URLs for REST API endpoints.
