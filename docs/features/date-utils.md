# Date Utilities Guide

The `DateUtils` component (`siddiqsoft/date-utils.hpp`) provides formatting, parsing, and duration calculation functions essential for Azure REST API headers and timestamps.

---

## Supported Formats

### RFC 7231 Date Formatting
Azure REST API HTTP headers (such as `x-ms-date` or `Date`) require RFC 7231 / HTTP-date compliant string representations.

```cpp
#include "siddiqsoft/date-utils.hpp"

// Formats current time as "Tue, 01 Nov 2022 08:12:31 GMT"
std::string rfcDate = siddiqsoft::DateUtils::RFC7231();

// Supports wchar_t container output as well
std::wstring rfcWideDate = siddiqsoft::DateUtils::RFC7231<wchar_t>();
```

### ISO 8601 Date Formatting
Azure Table Storage and Cosmos DB require ISO 8601 formatted timestamps with millisecond precision (`yyyy-mm-ddThh:mm:ss.mmmZ`).

```cpp
#include "siddiqsoft/date-utils.hpp"

// Formats current time as "2022-11-01T08:12:31.456Z"
std::string isoDate = siddiqsoft::DateUtils::ISO8601();
```

---

## Epoch Calculations & Parsing

### `epochPlus`
Calculates an epoch timestamp (in seconds) offset by a given duration from a reference `time_point` (defaults to current time):

```cpp
#include "siddiqsoft/date-utils.hpp"

// Calculate epoch timestamp 3600 seconds (1 hour) into the future
std::chrono::seconds expiryEpoch = siddiqsoft::DateUtils::epochPlus(std::chrono::seconds(3600));
```

### `parseEpoch`
Parses integer or string epoch values (including fractional seconds) back into a `std::chrono::system_clock::time_point`:

```cpp
#include "siddiqsoft/date-utils.hpp"

// Parse epoch string with fractional seconds
auto tp = siddiqsoft::DateUtils::parseEpoch("1629608276.500");
```

### `parseISO8601`
Parses an ISO 8601 string back into a `std::chrono::system_clock::time_point`:

```cpp
#include "siddiqsoft/date-utils.hpp"

auto tp = siddiqsoft::DateUtils::parseISO8601("2022-11-01T08:12:31.456Z");
```

---

## Timespans & Durations

### `toTimespan`
Formats a `std::chrono::seconds` duration into `D.HH:MM:SS`:

```cpp
#include "siddiqsoft/date-utils.hpp"

std::string ts = siddiqsoft::DateUtils::toTimespan(std::chrono::seconds(90061));
// Output: "1.01:01:01"
```

### `durationString`
Produces human-readable duration strings with automatic scaling:

```cpp
#include "siddiqsoft/date-utils.hpp"

std::string durStr = siddiqsoft::DateUtils::durationString(std::chrono::minutes(150));
// Output: "2hours 30minutes 0seconds"
```
