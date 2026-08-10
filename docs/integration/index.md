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
