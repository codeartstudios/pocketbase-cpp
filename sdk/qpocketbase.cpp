#include "qpocketbase.h"

QPocketBase::QPocketBase(QString& baseUrl, QString& lang, AuthStore* authStore, QObject *parent)
    : QObject{parent},
    m_authStore((authStore ? authStore : new AuthStore())),
    m_admins(new AdminService()),
    m_collections(new CollectionService()),
    m_files(new FileService()),
    m_realtime(new RealtimeService()),
    m_settings(new SettingsService()),
    m_logs(new LogService()),
    m_health(new HealthService()),
    m_backups(new BackupService())
{

}
