# `siddiqsoft::DateUtils` API Reference

Defined in `<siddiqsoft/date-utils.hpp>`

```cpp
namespace siddiqsoft {
    struct DateUtils;
}
```

The `DateUtils` struct contains static methods for date/time formatting, parsing, and duration arithmetic required by Azure REST APIs.

---

## Static Methods

### `epochPlus`

```cpp
static auto epochPlus(
    std::chrono::seconds incrementValue,
    const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now()
) -> const std::chrono::seconds;
```

Calculates the epoch timestamp (in seconds) resulting from adding `incrementValue` to `rawtp`.

- **Parameters**:
  - `incrementValue`: Duration offset in seconds.
  - `rawtp`: Reference time point (defaults to current system time).
- **Returns**: `std::chrono::seconds` count since epoch.

---

### `ISO8601`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> ISO8601(
    const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now()
);
```

Formats a time point into an ISO 8601 string (`yyyy-mm-ddThh:mm:ss.mmmZ`).

- **Template Parameters**: `T` (`char` or `wchar_t`).
- **Parameters**: `rawtp` reference time point.
- **Returns**: Formatted `std::basic_string<T>`.

---

### `RFC7231`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> RFC7231(
    const std::chrono::system_clock::time_point& rawtp = std::chrono::system_clock::now()
);
```

Formats a time point into HTTP RFC 7231 date string (`"Tue, 01 Nov 1994 08:12:31 GMT"`).

- **Template Parameters**: `T` (`char` or `wchar_t`).
- **Parameters**: `rawtp` reference time point.
- **Returns**: Formatted `std::basic_string<T>`.

---

### `toTimespan`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> toTimespan(const std::chrono::seconds& arg);
```

Formats a duration in seconds into `D.HH:MM:SS`.

- **Parameters**: `arg` duration in seconds.
- **Returns**: Formatted `std::basic_string<T>`.

---

### `parseEpoch`

```cpp
template <class T = std::string>
    requires std::same_as<T, std::string> || std::same_as<T, std::wstring> ||
             std::same_as<T, uint64_t> || std::same_as<T, uint32_t> || std::same_as<T, int>
static std::chrono::system_clock::time_point parseEpoch(const T& arg);
```

Parses an epoch representation (integer or string with optional fractional seconds) into a `time_point`.

- **Parameters**: `arg` epoch value.
- **Returns**: `std::chrono::system_clock::time_point`.

---

### `diff`

```cpp
template <typename T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::tuple<std::chrono::milliseconds, std::basic_string<T>>
diff(
    const std::chrono::time_point<std::chrono::system_clock>& end,
    const std::chrono::time_point<std::chrono::system_clock>& start
);
```

Calculates the difference between two time points.

- **Returns**: A tuple of `std::chrono::milliseconds` duration and formatted string `"HH:MM:SS.mmm"`.

---

### `durationString`

```cpp
template <typename T = char, typename D = std::chrono::microseconds>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::basic_string<T> durationString(const D& arg);
```

Formats a duration into human-readable components (years, months, weeks, days, hours, minutes, seconds).

---

### `parseISO8601`

```cpp
template <class T = char>
    requires std::same_as<T, char> || std::same_as<T, wchar_t>
static std::chrono::system_clock::time_point parseISO8601(const std::basic_string<T>& arg);
```

Parses an ISO 8601 formatted string (`yyyy-mm-ddThh:mm:ss.mmmZ`) into a `time_point`.
