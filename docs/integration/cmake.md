# CMake & CPM Integration Guide

## CPM.cmake

[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) is the recommended dependency manager for modern C++ CMake builds.

### Setup

1. Download `CPM.cmake` into your project's `cmake/` folder (or include the standard CPM snippet).
2. Add the following to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyAzureApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include CPM
include(cmake/CPM.cmake)

# Import AzureCppUtils
CPMAddModule("gh:siddiqsoft/AzureCppUtils#3.0.0")

# Link to your target
add_executable(MyAzureApp main.cpp)
target_link_libraries(MyAzureApp PRIVATE AzureCppUtils::AzureCppUtils)
```

---

## FetchContent

If you prefer standard CMake without `CPM.cmake`, use `FetchContent`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyAzureApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    AzureCppUtils
    GIT_REPOSITORY https://github.com/SiddiqSoft/AzureCppUtils.git
    GIT_TAG        v3.0.0
)

FetchContent_MakeAvailable(AzureCppUtils)

add_executable(MyAzureApp main.cpp)
target_link_libraries(MyAzureApp PRIVATE AzureCppUtils::AzureCppUtils)
```

---

## `add_subdirectory`

If you vendor `AzureCppUtils` as a git submodule or directory in `third_party/`:

```cmake
add_subdirectory(third_party/AzureCppUtils)

target_link_libraries(MyAzureApp PRIVATE AzureCppUtils::AzureCppUtils)
```

!!! tip "Target Alias"
    The CMake target defined by this library is `AzureCppUtils::AzureCppUtils`. Linking against this target automatically configures include directories and system libraries (`Crypt32` & `BCrypt` on Windows; `OpenSSL::Crypto` on Linux/macOS).
