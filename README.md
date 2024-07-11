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
- [X] File Service 
- [X] Health Service
- [ ] Backup Service (Not planned)
- [X] Log Service
- [X] Realtime Service
- [X] Settings Service
- [X] Documentation


## Installation

Add the library to your `dependencies`:

```sh
git clone https://github.com/codeartstudios/pocketbase-cpp.git

```

Import it in your C++ code:

```c++
#include "client.h"

using namespace pb;
auto client = PocketBase('http://127.0.0.1:8090');
```

> More detailed API docs and copy-paste examples could be found in the [API documentation for each service](https://pocketbase.io/docs/api-authentication)
> or in the [Services section](#services) below.



#### Error handling

All services return a standard response, so the error handling is straightforward:

```cpp
try {
  auto result = client.collection('example')->getList(1, 50);
} catch (ClientResponseError e) {
  qDebug() << e.what(); 
}
```

All response errors are normalized and wrapped as `ClientException` with the following public members that you could use:

```dart
ClientException {
    url            QString      // The address of the failed request
    message        QString      // The string message
    statusCode     int          // The status code of the failed request
    response       QMap         // The JSON API error response
    isAbort        bool         // Indicates whether the request was aborted/cancelled
    originalError  std::exception_ptr // The original response error
}
```

#### AuthStore

The SDK keeps track of the authenticated token and auth model for you via the `pb.authStore()` service.
The default `AuthStore` class has the following public members that you could use:

```dart
AuthStore {
    token:    String                      // Getter for the stored auth token
    model:    RecordModel|AdminModel|null // Getter for the stored auth RecordModel or AdminModel
    isValid   bool                        // Getter to loosely check if the store has an existing and unexpired token
    onTokenChanged  Signal                // Stream that gets triggered on each auth store change

    // methods
    save(token, model)             // update the store with the new auth data
    clear()                        // clears the current auth store state
}
```

To _"logout"_ an authenticated record or admin, you can just call `pb.authStore.clear()`.

To _"listen"_ for changes in the auth store, you can _listen_ to the `onTokenChanged` broadcast stream:
```dart
pb.authStore.onChange.listen((e) {
  print(e.token);
  print(e.model);
});
```


## Services

#### RecordService ([API docs](https://pocketbase.io/docs/api-records))

###### _Crud handlers_

```cpp
// Returns a paginated records list.
🔓 client.collection(collectionIdOrName)->getList(/*page*/ 1, /*perPage*/ 30, /*skipTotal*/skipTotal, /*QJsonObject*/params);

// Returns a list with all records batch fetched at once.
🔓 client.collection(collectionIdOrName)->getFullList(/*batch*/ 100, /*QJsonObject*/params);

// Returns the first found record matching the specified filter.
🔓 client.collection(collectionIdOrName)->getFirstListItem(filter, /*QJsonObject*/params);

// Returns a single record by its id.
🔓 client.collection(collectionIdOrName)->getOne(recordId, /*QJsonObject*/params);

// Creates (aka. register) a new record.
🔓 client.collection(collectionIdOrName)->create(/*QJsonObject*/body, /*QJsonObject*/params);

// Updates an existing record by its id.
🔓 client.collection(collectionIdOrName)->update(recordId, /*QJsonObject*/body, /*QJsonObject*/params);

// Deletes a single record by its id.
🔓 client.collection(collectionIdOrName)->deleteOne(recordId);
```

###### _Realtime handlers_

```cpp
// Subscribe to realtime changes to the specified topic ("*" or recordId).
//
// It is safe to subscribe multiple times to the same topic.
//
// If you want to remove all subscriptions related to the topic use unsubscribe(topic).
🔓 client.collection("collectionIdOrName")->subscribe( [&](const Event& data) { /* Callback Function */ } );

// Unsubscribe from all registered subscriptions to the specified topic ("*" or recordId).
// If topic is not set, then it will remove all registered collection subscriptions.
🔓 client.collection("collectionIdOrName")->unsubscribe();

🔓 client.collection("collectionIdOrName")->unsubscribe("topic");

🔓 client.collection("collectionIdOrName")->unsubscribe(["topic1", "topic2", ...]);
```

###### _Auth handlers_

> Available only for "auth" type collections.

```cpp
// Returns all available application auth methods.
🔓 client.collection(collectionIdOrName)->listAuthMethods(/*QJsonObject*/ params);

// Authenticates a record with their username/email and password.
🔓 client.collection(collectionIdOrName)->authWithPassword(usernameOrEmail, password, /*QJsonObject*/ body, /*QJsonObject*/ query);

// Authenticates a record with OAuth2 code.
🔓 client.collection(collectionIdOrName)->authWithOAuth2(provider, code, codeVerifier, redirectUrl, /*QJsonObject*/ createData, /*QJsonObject*/ body, /*QJsonObject*/ query);

// Refreshes the current authenticated record model and auth token.
🔐 client.collection(collectionIdOrName)->authRefresh(/*QJsonObject*/body, /*QJsonObject*/ query);

// Sends a user password reset email.
🔓 client.collection(collectionIdOrName)->requestPasswordReset(email, /*QJsonObject*/body, /*QJsonObject*/query);

// Confirms a record password reset request.
🔓 client.collection(collectionIdOrName)->confirmPasswordReset(resetToken, newPassword, newPasswordConfirm, {expand?, fields?, query, body, headers});

// Sends a record verification email request.
🔓 client.collection(collectionIdOrName)->requestVerification(email);

// Confirms a record email verification request.
🔓 client.collection(collectionIdOrName)->confirmVerification(verificationToken);

// Sends a record email change request to the provider email.
🔐 client.collection(collectionIdOrName)->requestEmailChange(newEmail);

// Confirms record new email address.
🔓 client.collection(collectionIdOrName)->confirmEmailChange(emailChangeToken, userPassword);

```

---

#### AdminService ([API docs](https://pocketbase.io/docs/api-admins))

```cpp
// Authenticates an admin account by its email and password.
🔓 client.admins()->authWithPassword(email, password, /*QJsonObject*/ params);

// Refreshes the current admin authenticated model and token.
🔐 client.admins()->authRefresh(/*QJsonObject*/ params);

// Sends an admin password reset email.
🔓 client.admins()->requestPasswordReset(email);

// Confirms an admin password reset request.
🔓 client.admins()->confirmPasswordReset(resetToken, newPassword, newPasswordConfirm);

// Returns a paginated admins list.
🔐 client.admins()->getList(/*page*/ 1, /*perPage*/ 30, /*skipTotal*/false, /*QJsonObject*/ params);

// Returns a list with all admins batch fetched at once.
🔐 client.admins()->getFullList(/*batch*/ 100, /*QJsonObject*/ params);

// Returns the first found admin matching the specified filter.
🔐 client.admins()->getFirstListItem(filter, /*QJsonObject*/ params);

// Returns a single admin by their id.
🔐 client.admins()->getOne(id, /*QJsonObject*/ params);

// Creates a new admin.
🔐 client.admins()->create(/*QJsonObject*/ body, /*QJsonObject*/ query);

// Updates an existing admin by their id.
🔐 client.admins()->update(id, /*QJsonObject*/body, /*QJsonObject*/ query);

// Deletes a single admin by their id.
🔐 client.admins()->deleteOne(id);
```

---

#### CollectionService ([API docs](https://pocketbase.io/docs/api-collections))

```cpp
// Returns a paginated collections list.
🔐 client.collections()->getList(/*page*/ 1, /*perPage*/ 30, /*QJsonObject*/ params);

// Returns a list with all collections batch fetched at once.
🔐 client.collections()->getFullList(/*batch*/ 100, /*QJsonObject*/ params);

// Returns the first found collection matching the specified filter.
🔐 client.collections()->getFirstListItem(/*QString*/ filter, /*QJsonObject*/ query);

// Returns a single collection by its id.
🔐 client.collections()->getOne(id, /*QJsonObject*/ params);

// Creates (aka. register) a new collection.
🔐 client.collections()->create(/*QJsonObject*/ body, /*QJsonObject*/ query);

// Updates an existing collection by its id.
🔐 client.collections()->update(id, /*QJsonObject*/ body, /*QJsonObject*/ query);

// Deletes a single collection by its id.
🔐 client.collections()->deleteOne(id);

// Imports the provided collections.
🔐 client.collections()->import(collections, /*deleteMissing*/ false, /*QJsonObject*/ params);
```

---

#### LogService ([API docs](https://pocketbase.io/docs/api-logs))

```cpp
// Returns a paginated logs list.
🔐 client.logs()->getList(/*page*/ 1, /*perPage*/ 30, /*QJsonObject*/ params);

// Returns a single log by its id.
🔐 client.logs()->getOne(id, /*QJsonObject*/ params);

// Returns logs statistics.
🔐 client.logs()->getStats(/*QJsonObject*/ params);
```

---

#### SettingsService ([API docs](https://pocketbase.io/docs/api-settings))

```cpp
// Returns a map with all available app settings.
🔐 client.settings()->getAll(/*QJsonObject*/params);

// Bulk updates app settings.
🔐 client.settings()->update(/*QJsonObject*/params);

// Performs a S3 storage connection test.
🔐 client.settings()->testS3(/*QJsonObject*/params);

// Sends a test email (verification, password-reset, email-change).
🔐 client.settings()->testEmail(toEmail, template);

// Generates a new Apple OAuth2 client secret.
🔐 client.settings()->generateAppleClientSecret(clientId, teamId, keyId, privateKey, duration);
```

---

#### RealtimeService

> This service is usually used with custom realtime actions.
> For records realtime subscriptions you can use the subscribe/unsubscribe
> methods available in the `pb.collection()` RecordService.

```cpp
// Initialize the realtime connection (if not already) and register the subscription.
//
// You can subscribe to the `PB_CONNECT` event if you want to listen to the realtime connection connect/reconnect events.
🔓 client.realtime()->subscribe("topic", [&](const Event& data) { /* Callback Function */ }, /*Optional params QJsonObject() */ );

// Unsubscribe from a subscription (If topic is empty, all subscriptions are removed).
🔓 client.realtime()->unsubscribe("topic");

```


---

##### HealthService

```cpp
  // Health check
  QJsonObject health = client.health()->check(/*Optional QJsonObject{}*/);

```



