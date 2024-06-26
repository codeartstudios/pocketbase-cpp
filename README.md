PocketBase C++ SDK in Qt
======================================================================

NON OFFICIAL & EXPERIMENTAL C++ SDK for interacting with the [PocketBase Web API](https://pocketbase.io/docs).

Work still in progress, many breaking changes without notice as we try to match the official SDKs. Contributions are highly welcome.

# Implemented Features 
- [X] Admin Service
- [ ] Records Service (partially)
  - [X] Auth With Password
  - [ ] Auth with OAuth2 (In Plans)
  - [X] Auth Refresh
  - [X] Request Email Verification
  - [X] Confirm Email Verification
  - [X] Request Password Reset
  - [X] Confirm Password Reset
  - [X] Request Email Change
  - [X] Confirm Email Change
  - [ ] List auth methods (Not planned)
  - [ ] List OAuth2 accounts (Not planned)
  - [ ] Unlink OAuth2 account  (Not planned)
- [X] Crud Service
  - [X] Search/List
    - [X] GetList
    - [X] GetFullList
    - [X] GetFistListItem
  - [X] View (getOne)
  - [X] Create
  - [X] Update
  - [X] Delete
  - [X] Realtime
- [X] Collections Service
- [ ] File Service (In plan)
- [X] Health Service
- [ ] Backup Service (Not planned)
- [X] Log Service
- [X] Realtime Service
- [X] Settings Service
- [ ] Documentation


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
