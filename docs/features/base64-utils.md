# Base64 Utilities Guide

The `Base64Utils` component (`siddiqsoft/base64-utils.hpp`) provides fast, safe Base64 encoding and decoding as well as RFC 4648 URL escaping.

---

## Core Operations

### Base64 Encoding
Encodes raw binary data or strings into Base64 format.

```cpp
#include "siddiqsoft/base64-utils.hpp"

std::string raw = "Azure C++ Utilities";
std::string encoded = siddiqsoft::Base64Utils::encode(raw);
```

### Base64 Decoding
Decodes a Base64-encoded string back into raw binary bytes.

```cpp
#include "siddiqsoft/base64-utils.hpp"

std::string b64 = "QXp1cmUgQysrIFV0aWxpdGllcw==";
std::string decoded = siddiqsoft::Base64Utils::decode(b64);
```

---

## RFC 4648 URL Escaping

When sending Base64 strings in URL query parameters, special characters like `+`, `/`, and `=` must be converted per [RFC 4648 Section 5](https://tools.ietf.org/html/rfc4648#section-5).

```cpp
#include "siddiqsoft/base64-utils.hpp"

std::string b64 = "abc+def/ghi==";

// Replaces '+' with '-', '/' with '_', and strips '\r', '\n', '='
std::string urlSafeB64 = siddiqsoft::Base64Utils::urlEscape(b64);
// Result: "abc-def_ghi"
```

!!! note "Platform Native Implementations"
    - **Windows**: Uses Win32 CryptoAPI functions (`CryptBinaryToStringA`/`CryptBinaryToStringW` and `CryptStringToBinaryA`/`CryptStringToBinaryW`).
    - **Linux/macOS**: Uses OpenSSL `EVP_EncodeBlock` and `EVP_DecodeBlock`.
