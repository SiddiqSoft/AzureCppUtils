# `siddiqsoft::ConversionUtils` API Reference

Defined in `<siddiqsoft/conversion-utils.hpp>`

```cpp
namespace siddiqsoft {
    struct ConversionUtils;
}
```

`ConversionUtils` provides string conversion utilities between narrow (`char` / UTF-8) and wide (`wchar_t` / UTF-16) string types.

---

## Static Methods

### `convert_to`

```cpp
template <typename From, typename To>
static std::basic_string<To> convert_to(const std::basic_string<From>& src);
```

Converts a string from type `From` (`char` or `wchar_t`) to type `To` (`char` or `wchar_t`).

- **Parameters**: `src` input string.
- **Returns**: Converted `std::basic_string<To>`.

---

### `wideFromUtf8`

```cpp
static std::wstring wideFromUtf8(const std::string& src);
```

Converts a UTF-8 encoded `std::string` into a `std::wstring`.
