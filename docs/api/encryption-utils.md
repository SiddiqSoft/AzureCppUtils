# `siddiqsoft::EncryptionUtils` API Reference

Defined in `<siddiqsoft/encryption-utils.hpp>`

```cpp
namespace siddiqsoft {
    struct EncryptionUtils;
}
```

`EncryptionUtils` provides cryptographic hashing, HMAC signatures, and token generators for Azure services.

---

## Static Methods

### `MD5`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::string MD5(const std::basic_string<T>& source);
```

Generates a hex-encoded MD5 hash string for the input payload.

- **Parameters**: `source` data to hash.
- **Returns**: 32-character lowercase hex string.

---

### `HMAC`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> HMAC(
    const std::basic_string<T>& key,
    const std::basic_string<T>& source
);
```

Calculates raw binary HMAC-SHA256 signature for the given key and string-to-sign.

- **Parameters**:
  - `key`: Secret key bytes.
  - `source`: String-to-sign payload.
- **Returns**: Raw HMAC-SHA256 signature string.

---

### `JWTSHA256`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> JWTSHA256(
    const std::basic_string<T>& key,
    const std::basic_string<T>& source
);
```

Generates a URL-safe Base64 HMAC-SHA256 signature formatted for JSON Web Tokens (JWT).

---

### `SASToken`

```cpp
template <typename T = char, typename DurationOrEpoch = std::chrono::minutes>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> SASToken(
    const std::basic_string<T>& key,
    const std::basic_string<T>& url,
    const std::basic_string<T>& keyName,
    const DurationOrEpoch& timeOrDuration = std::chrono::minutes(60)
);
```

Generates a Shared Access Signature (SAS) token for Azure Service Bus / Event Hubs.

- **Parameters**:
  - `key`: Base64 encoded primary/secondary key.
  - `url`: Resource URI.
  - `keyName`: SAS policy key name (e.g., `"RootManageSharedAccessKey"`).
  - `timeOrDuration`: Expiry duration (e.g. `std::chrono::minutes(60)`) or explicit epoch string.
- **Returns**: Complete `"SharedAccessSignature sr=...&sig=...&se=...&skn=..."` string.

---

### `CosmosToken`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> CosmosToken(
    const std::basic_string<T>& masterKey,
    const std::basic_string<T>& verb,
    const std::basic_string<T>& resourceType,
    const std::basic_string<T>& resourceId,
    const std::basic_string<T>& date
);
```

Generates an authorization signature token for Azure Cosmos DB REST requests.

- **Parameters**:
  - `masterKey`: Cosmos DB master key (Base64).
  - `verb`: HTTP verb (`"GET"`, `"POST"`, `"PUT"`, `"DELETE"`).
  - `resourceType`: Resource type (`"dbs"`, `"colls"`, `"docs"`).
  - `resourceId`: Resource ID or link path.
  - `date`: RFC 7231 / HTTP-date string.
- **Returns**: URL-encoded authorization header value `type%3Dmaster%26ver%3D1.0%26sig%3D...`.

---

### `constantTimeCompare`

```cpp
static bool constantTimeCompare(
    std::string_view a,
    std::string_view b
) noexcept;
```

Compares two string slices in constant time to prevent timing side-channel attacks during token or signature verification.

- **Parameters**:
  - `a`: First string view.
  - `b`: Second string view.
- **Returns**: `true` if identical in length and bytes; `false` otherwise.
