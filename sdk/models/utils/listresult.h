#ifndef LISTRESULT_H
#define LISTRESULT_H

#include <QList>
#include <memory>

class BaseModel;

class ListResult {
public:
    ListResult(int page = 1, int perPage = 0, int totalItems = 0, int totalPages = 0, const QList<std::shared_ptr<BaseModel>>& items = QList<std::shared_ptr<BaseModel>>());

    int page() const;
    int perPage() const;
    int totalItems() const;
    int totalPages() const;
    QList<std::shared_ptr<BaseModel>> items() const;

private:
    int m_page;
    int m_perPage;
    int m_totalItems;
    int m_totalPages;
    QList<std::shared_ptr<BaseModel>> m_items;
};

#endif // LISTRESULT_H
