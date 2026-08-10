# `siddiqsoft::Base64Utils` API Reference

Defined in `<siddiqsoft/base64-utils.hpp>`

```cpp
namespace siddiqsoft {
    struct Base64Utils;
}
```

`Base64Utils` provides static methods for Base64 encoding, decoding, and RFC 4648 URL escaping.

---

## Static Methods

### `encode`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> encode(const std::basic_string<T>& argBin);
```

Base64 encodes the input binary string.

- **Template Parameters**: `T` (`char` or `wchar_t`).
- **Parameters**: `argBin` raw bytes/string to encode.
- **Returns**: Base64 encoded `std::basic_string<T>`.

---

### `decode`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> decode(const std::basic_string<T>& textuallyEncoded);
```

Base64 decodes a previously encoded string back to binary.

- **Template Parameters**: `T` (`char` or `wchar_t`).
- **Parameters**: `textuallyEncoded` Base64 string.
- **Returns**: Decoded binary `std::basic_string<T>`.
- **Throws**: `std::runtime_error` if decoding fails.

---

### `urlEscape`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> urlEscape(const std::basic_string<T>& src);
```

Escapes a Base64 string per RFC 4648 Section 5 for URL safety:
- Replaces `+` with `-`
- Replaces `/` with `_`
- Strips `\r`, `\n`, and `=` padding characters.

- **Parameters**: `src` Base64 encoded string.
- **Returns**: URL-safe Base64 string.
