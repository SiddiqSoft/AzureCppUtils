# Package Managers & Direct Headers Guide

## NuGet

For Visual Studio or MSBuild projects on Windows, `AzureCppUtils` is available on nuget.org as [`SiddiqSoft.AzureCppUtils`](https://www.nuget.org/packages/SiddiqSoft.AzureCppUtils/).

### Installation

=== "Package Manager Console"

    ```powershell
    Install-Package SiddiqSoft.AzureCppUtils
    ```

=== "dotnet CLI"

    ```bash
    dotnet add package SiddiqSoft.AzureCppUtils
    ```

=== "PackageReference (vcxproj)"

    ```xml
    <ItemGroup>
      <PackageReference Include="SiddiqSoft.AzureCppUtils" Version="3.0.0" />
    </ItemGroup>
    ```

---

## Direct Headers

Because `AzureCppUtils` is header-only, you can directly include the `include/` directory in your build system's include path:

```text
include/
└── siddiqsoft/
    ├── base64-utils.hpp
    ├── base64-utils-unix.hpp
    ├── base64-utils-win.hpp
    ├── conversion-utils.hpp
    ├── date-utils.hpp
    ├── encryption-utils.hpp
    ├── encryption-utils-unix.hpp
    ├── encryption-utils-win.hpp
    ├── url-utils.hpp
    └── RunOnEnd.hpp
```

### Compiler Requirements

- **C++ Standard**: Pass `-std=c++23` (GCC/Clang) or `/std:c++latest` (MSVC).
- **Windows System Libraries**: Ensure your build links against `crypt32.lib` and `bcrypt.lib`.
- **Linux/macOS System Libraries**: Ensure your build links against OpenSSL (`libcrypto`).
