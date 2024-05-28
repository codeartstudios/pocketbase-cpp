#ifndef QPOCKETBASE_H
#define QPOCKETBASE_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QUrl>
#include <QDebug>


#include "authstore.h"
#include "adminservice.h"
#include "collectionservice.h"
#include "fileservice.h"
#include "realtimeservice.h"
#include "settingsservice.h"
#include "logservice.h"
#include "healthservice.h"
#include "backupservice.h"
#include "recordservice.h"

class QPocketBase : public QObject
{
    Q_OBJECT
public:
    // explicit QPocketBase();
    QPocketBase(QString& baseUrl, const QString& lang = "en-US", AuthStore* authStore = nullptr, QObject *parent = nullptr);

    /// The PocketBase backend base url address (eg. 'http://127.0.0.1:8090').
    // Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)
    Q_PROPERTY(QString baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged FINAL)

    /// Optional language code (default to `en-US`) that will be sent
    /// with the requests to the server as `Accept-Language` header.
    Q_PROPERTY(QString lang READ lang WRITE setLang NOTIFY langChanged FINAL)

    /// The underlying http client that will be used to send the request.
    /// This is used primarily for the unit tests.
    // late final http.Client Function() httpClientFactory;

    /// Returns the RecordService associated to the specified collection.
    RecordService* collection(const QString& collectionIdOrName);

    /// Constructs a filter expression with placeholders populated from a map.
    ///
    /// Placeholder parameters are defined with the `{:paramName}` notation.
    ///
    /// The following parameter values are supported:
    /// - `String` (_single quotes are autoescaped_)
    /// - `num`
    /// - `bool`
    /// - `DateTime`
    /// - `null`
    /// - everything else is converted to a string using `jsonEncode()`
    ///
    /// Example:
    ///
    /// ```dart
    /// pb.collection("example").getList(filter: pb.filter(
    ///   "title ~ {:title} && created >= {:created}",
    ///   { "title": "example", "created": DateTime.now() },
    /// ));
    /// ```
    QString filter(const QString& expr, const QMap<QString, QVariant>& query);

    /// Legacy alias of `pb.files.getUrl()`.
    // QString getFileUrl( RecordModel record, QString filename, QString thumb="", QString token="", QMap<QString, QVariant> query = {} ) {
    //     return m_files.getUrl( record, filename, thumb, token, query );
    // }

    /// Builds and returns a full request url by safely concatenating
    /// the provided path to the base url.
    QUrl buildUrl(QString path, QMap<QString, QVariant> queryParameters );


    /*
     * class PocketBase {




  /// Sends a single HTTP request built with the current client configuration
  /// and the provided options.
  ///
  /// All response errors are normalized and wrapped in [ClientException].
  Future<dynamic> send(
    String path, {
    String method = "GET",
    Map<String, String> headers = const {},
    Map<String, dynamic> query = const {},
    Map<String, dynamic> body = const {},
    List<http.MultipartFile> files = const [],
  }) async {
    http.BaseRequest request;

    final url = buildUrl(path, query);

    if (files.isEmpty) {
      request = _jsonRequest(method, url, headers: headers, body: body);
    } else {
      request = _multipartRequest(
        method,
        url,
        headers: headers,
        body: body,
        files: files,
      );
    }

    if (!headers.containsKey("Authorization") && authStore.isValid) {
      request.headers["Authorization"] = authStore.token;
    }

    if (!headers.containsKey("Accept-Language")) {
      request.headers["Accept-Language"] = lang;
    }

    // ensures that keepalive on web is disabled for now
    //
    // it is ignored anyway when using the default http.Cient on web
    // and it causing issues with the alternative fetch_client package
    // (see https://github.com/Zekfad/fetch_client/issues/6#issuecomment-1615936365)
    if (isWeb) {
      request.persistentConnection = false;
    }

    final requestClient = httpClientFactory();

    try {
      final response = await requestClient.send(request);
      final responseStr = await response.stream.bytesToString();

      dynamic responseData;
      try {
        responseData = responseStr.isNotEmpty ? jsonDecode(responseStr) : null;
      } catch (_) {
        // custom non-json response
        responseData = responseStr;
      }

      if (response.statusCode >= 400) {
        throw ClientException(
          url: url,
          statusCode: response.statusCode,
          response: responseData is Map<String, dynamic> ? responseData : {},
        );
      }

      return responseData;
    } catch (e) {
      // PocketBase API exception
      if (e is ClientException) {
        rethrow;
      }

      // http client exception (eg. connection abort)
      if (e is http.ClientException) {
        throw ClientException(
          url: e.uri,
          originalError: e,
          // @todo will need to be redefined once cancellation support is added
          isAbort: true,
        );
      }

      // anything else
      throw ClientException(url: url, originalError: e);
    } finally {
      requestClient.close();
    }
  }

  http.Request _jsonRequest(
    String method,
    Uri url, {
    Map<String, String> headers = const {},
    Map<String, dynamic> body = const {},
  }) {
    final request = http.Request(method, url);

    if (body.isNotEmpty) {
      request.body = jsonEncode(body);
    }

    if (headers.isNotEmpty) {
      request.headers.addAll(headers);
    }

    if (!headers.containsKey("Content-Type")) {
      request.headers["Content-Type"] = "application/json";
    }

    return request;
  }

  MultipartRequest _multipartRequest(
    String method,
    Uri url, {
    Map<String, String> headers = const {},
    Map<String, dynamic> body = const {},
    List<http.MultipartFile> files = const [],
  }) {
    final request = MultipartRequest(method, url)
      ..files.addAll(files)
      ..headers.addAll(headers);

    request.fields["@jsonPayload"] = [jsonEncode(body)];

    return request;
  }

  Map<String, dynamic> _normalizeQueryParameters(
    Map<String, dynamic> parameters,
  ) {
    final result = <String, dynamic>{};

    parameters.forEach((key, value) {
      final normalizedValue = <String>[];

      // convert to List to normalize access
      if (value is! Iterable) {
        value = [value];
      }

      for (dynamic v in value) {
        if (v == null) {
          continue; // skip null query params
        }

        normalizedValue.add(v.toString());
      }

      if (normalizedValue.isNotEmpty) {
        result[key] = normalizedValue;
      }
    });

    return result;
  }
}
     */

    QString baseUrl() const;
    void setBaseUrl(const QString &newBaseUrl);

    QString lang() const;
    void setLang(const QString &newLang);

signals:

    void baseUrlChanged();

    void langChanged();

private:
    /// An instance of the local [AuthStore] service.
    AuthStore* m_authStore;

    /// An instance of the service that handles the **Admin APIs**.
    AdminService* m_admins;

    /// An instance of the service that handles the **Collection APIs**.
    CollectionService* m_collections;

    /// An instance of the service that handles the **File APIs**.
    FileService* m_files;

    /// An instance of the service that handles the **Realtime APIs**.
    ///
    /// This service is usually used with custom realtime actions.
    /// For records realtime subscriptions you can use the subscribe/unsubscribe
    /// methods available in the `collection()` RecordService.
    RealtimeService* m_realtime;

    /// An instance of the service that handles the **Settings APIs**.
    SettingsService* m_settings;

    /// An instance of the service that handles the **Log APIs**.
    LogService* m_logs;

    /// An instance of the service that handles the **Health APIs**.
    HealthService* m_health;

    /// The service that handles the **Backup and restore APIs**.
    BackupService* m_backups;

    /// Cache of all created RecordService instances.
    QMap<QString, RecordService*> m_recordServices;

    QString m_baseUrl;
    QString m_lang;
};

#endif // QPOCKETBASE_H
