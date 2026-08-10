# `siddiqsoft::UrlUtils` API Reference

Defined in `<siddiqsoft/url-utils.hpp>`

```cpp
namespace siddiqsoft {
    struct UrlUtils;
}
```

`UrlUtils` provides RFC 3986 URL percent-encoding.

---

## Static Methods

### `encode`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> encode(
    const std::basic_string<T>& source,
    bool lowerCase = false
);
```

Percent-encodes a string for HTTP context.

- **Template Parameters**: `T` (`char` or `wchar_t`).
- **Parameters**:
  - `source`: Source string to encode.
  - `lowerCase`: If `true`, hex percent escapes use lowercase (`%20`); if `false`, uppercase (`%20`). Defaults to `false`.
- **Returns**: URL-encoded `std::basic_string<T>`.
