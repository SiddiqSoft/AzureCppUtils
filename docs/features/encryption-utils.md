# Encryption & Token Utilities Guide

The `EncryptionUtils` component (`siddiqsoft/encryption-utils.hpp`) provides cryptographic signatures and authorization token generation specifically designed for Azure REST services.

---

## Azure Authorization Tokens

### Azure Service Bus / Event Hubs SAS Tokens
Generates a Shared Access Signature (SAS) token for Azure Service Bus queues/topics or Event Hubs.

```cpp
#include <chrono>
#include "siddiqsoft/encryption-utils.hpp"

std::string keyName = "RootManageSharedAccessKey";
std::string key     = "PrimarySecretKey==";
std::string uri     = "mynamespace.servicebus.windows.net/myqueue";

// Option 1: Specify duration (e.g. 60 minutes)
auto sas1 = siddiqsoft::EncryptionUtils::SASToken<char>(
    key, uri, keyName, std::chrono::minutes(60)
);

// Option 2: Specify explicit epoch expiry string
auto sas2 = siddiqsoft::EncryptionUtils::SASToken<char>(
    key, uri, keyName, "1629608276"
);
```

### Azure Cosmos DB Authorization Tokens
Generates master-key authorization signatures for Azure Cosmos DB REST API calls.

```cpp
#include "siddiqsoft/encryption-utils.hpp"

std::string masterKey    = "CosmosMasterKeyBase64==";
std::string verb         = "GET";
std::string resourceType = "docs";
std::string resourceId   = "dbs/mydb/colls/mycoll";
std::string date         = "Tue, 01 Nov 2022 08:12:31 GMT";

auto cosmosToken = siddiqsoft::EncryptionUtils::CosmosToken<char>(
    masterKey, verb, resourceType, resourceId, date
);
```

---

## Cryptographic Hashes & Signatures

### MD5 Hashing
Calculates hex-encoded MD5 hash strings.

```cpp
#include "siddiqsoft/encryption-utils.hpp"

std::string md5Hex = siddiqsoft::EncryptionUtils::MD5("Hello Azure");
```

### HMAC-SHA256 Signatures
Calculates raw binary or Base64 HMAC-SHA256 signatures for a given key and string-to-sign.

```cpp
#include "siddiqsoft/encryption-utils.hpp"

std::string key = "SecretKey";
std::string payload = "StringToSign";

// Compute raw binary HMAC-SHA256 signature
std::string hmacBin = siddiqsoft::EncryptionUtils::HMAC<char>(key, payload);
```

### JWT SHA256 Signatures
Generates HMAC-SHA256 signatures tailored for JSON Web Tokens (JWT).

```cpp
#include "siddiqsoft/encryption-utils.hpp"

std::string key = "JWTSecretKey";
std::string jwtHeaderAndPayload = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIn0";

std::string jwtSignature = siddiqsoft::EncryptionUtils::JWTSHA256<char>(key, jwtHeaderAndPayload);
```

!!! warning "Binary Key Storage"
    Keys in `EncryptionUtils` are passed as binary data contained within a `std::string`. Do not attempt to store or convert binary key strings into `std::wstring`, as character conversion will alter the byte sequence and cause Azure authentication failures.
