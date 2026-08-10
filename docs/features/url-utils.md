# URL Utilities Guide

The `UrlUtils` component (`siddiqsoft/url-utils.hpp`) provides RFC 3986 percent-encoding for HTTP request URLs and query strings.

---

## URL Encoding

The `UrlUtils::encode` method encodes any string into a URL-safe percent-encoded format. Alphanumeric characters (`0-9`, `A-Z`, `a-z`) and unreserved characters (`.`, `-`, `~`, `_`) are preserved while all other characters are encoded.

### Basic Usage

```cpp
#include <iostream>
#include "siddiqsoft/url-utils.hpp"

int main()
{
    std::string rawUrl = "https://myaccount.servicebus.windows.net/my queue?param=value&foo=bar#section";

    // Encode URL using uppercase hexadecimal (e.g. %20)
    std::string encodedUpper = siddiqsoft::UrlUtils::encode(rawUrl, false);

    // Encode URL using lowercase hexadecimal (e.g. %20)
    std::string encodedLower = siddiqsoft::UrlUtils::encode(rawUrl, true);

    std::cout << "Uppercase: " << encodedUpper << std::endl;
    std::cout << "Lowercase: " << encodedLower << std::endl;
    return 0;
}
```

### Wide String Support

`UrlUtils::encode` also transparently supports `std::wstring`. Wide strings are converted to UTF-8 before percent-encoding:

```cpp
#include "siddiqsoft/url-utils.hpp"

std::wstring wideStr = L"my queue/path";
std::wstring encodedWide = siddiqsoft::UrlUtils::encode(wideStr);
```
