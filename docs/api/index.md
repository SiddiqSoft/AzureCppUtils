# API Reference Overview

All classes and helper functions in `AzureCppUtils` are located in the `siddiqsoft` namespace.

---

## API Reference Directory

| Struct / Header | Description | API Page |
| :--- | :--- | :--- |
| [`siddiqsoft::DateUtils`](date-utils.md) | Date & time utilities for RFC7231, ISO8601, epoch conversion, and duration arithmetic. | [DateUtils API](date-utils.md) |
| [`siddiqsoft::Base64Utils`](base64-utils.md) | Base64 encoding/decoding and RFC 4648 URL escaping. | [Base64Utils API](base64-utils.md) |
| [`siddiqsoft::EncryptionUtils`](encryption-utils.md) | Cryptographic functions (MD5, HMAC-SHA256, JWT SHA256) and Azure SAS/Cosmos tokens. | [EncryptionUtils API](encryption-utils.md) |
| [`siddiqsoft::UrlUtils`](url-utils.md) | RFC 3986 percent-encoding for HTTP URLs. | [UrlUtils API](url-utils.md) |
| [`siddiqsoft::ConversionUtils`](conversion-utils.md) | Wide/narrow string conversions (`char` <-> `wchar_t`). | [ConversionUtils API](conversion-utils.md) |

---

## Namespace

```cpp
namespace siddiqsoft {
    struct DateUtils;
    struct Base64Utils;
    struct EncryptionUtils;
    struct UrlUtils;
    struct ConversionUtils;
}
```
