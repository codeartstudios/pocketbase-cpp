#ifndef LISTRESULT_H
#define LISTRESULT_H

#include <QList>
#include "basemodel.h"

namespace pb {
class ListResult {
public:
    ListResult(int page = 1, int perPage = 0,
               int totalItems = 0, int totalPages = 0,
               const QList<BaseModel*>& items = QList<BaseModel*>());

    int page() const;
    int perPage() const;
    int totalItems() const;
    int totalPages() const;
    QList<BaseModel*> items() const;

private:
    int m_page;
    int m_perPage;
    int m_totalItems;
    int m_totalPages;
    QList<BaseModel*> m_items;
};
}

#endif // LISTRESULT_H
