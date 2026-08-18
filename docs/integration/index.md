# Integration Overview

`AzureCppUtils` is a modern, header-only C++23 library. Integrating it into your C++ project is fast and straightforward.

---

## Integration Methods

| Method | Best For | Link |
| :--- | :--- | :--- |
| **CPM.cmake** | Modern CMake projects with automatic dependency management | [CMake & CPM Guide](cmake.md#cpmcmake) |
| **FetchContent** | Standard CMake 3.14+ projects without external scripts | [FetchContent Guide](cmake.md#fetchcontent) |
| **NuGet** | Visual Studio & MSBuild C++ projects on Windows | [Package Managers Guide](package-managers.md#nuget) |
| **Direct Copy / Submodule** | Embedded builds or vendored dependencies | [Header Inclusion](package-managers.md#direct-headers) |
| **Dependency Diagram** | Mermaid graph and breakdown of CPM / system dependencies | [Dependencies Page](dependencies.md) |

---

## Quick CMake Snippet

```cmake
include(FetchContent)
FetchContent_Declare(
    AzureCppUtils
    GIT_REPOSITORY https://github.com/SiddiqSoft/AzureCppUtils.git
    GIT_TAG        v3.0.0
)
FetchContent_MakeAvailable(AzureCppUtils)

target_link_libraries(your_target PRIVATE AzureCppUtils::AzureCppUtils)
```

---

## Dependencies Summary

| Dependency | Details |
| :--- | :--- |
| **`RunOnEnd`** | [`gh:SiddiqSoft/RunOnEnd#1.4.5`](https://github.com/SiddiqSoft/RunOnEnd) (Scope guard cleanup) |
| **`StringHelpers`** | [`gh:SiddiqSoft/StringHelpers#1.2.2`](https://github.com/SiddiqSoft/StringHelpers) (Wide/narrow string conversions) |
| **Native Cryptography** | `Win32 Crypt32 / BCrypt` on Windows; `OpenSSL 3.x` (`libcrypto`) on Linux & macOS |
