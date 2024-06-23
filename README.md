PocketBase C++ SDK in Qt
======================================================================

NON OFFICIAL & EXPERIMENTAL C++ SDK for interacting with the [PocketBase Web API](https://pocketbase.io/docs).

Work still in progress, many breaking changes without notice as we try to match the official SDKs. Contributions are highly welcome.

So far, 
- [ ] Admin Service
- [ ] Records Service (partially)
- [ ] Collections Service
- [ ] File Service

Segments copied from the Dart SDK, will be updated later ...
- [Installation](#installation)
- [Caveats](#caveats)
    - [File upload](#file-upload)
    - [RecordModel](#recordmodel)
    - [Error handling](#error-handling)
    - [AuthStore](#authstore)
    - [Binding filter parameters](#binding-filter-parameters)
- [Services](#services)
- [Limitations](#limitations)
- [Development](#development)


## Installation

Add the library to your `dependencies`:

```sh
git clone https://github.com/codeartstudios/pocketbase-cpp.git

```

Import it in your C++ code:

```c++
#include "client.h"

auto pb = PocketBase('http://127.0.0.1:8090');

...

// authenticate as regular user
QJsonObject userData = pb.collection('users').authWithPassword('test@example.com', '123456');


// and much more...
```

> More detailed API docs and copy-paste examples could be found in the [API documentation for each service](https://pocketbase.io/docs/api-authentication)
> or in the [Services section](#services) below.
