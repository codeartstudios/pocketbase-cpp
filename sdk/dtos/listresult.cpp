#include "ListResult.h"
#include "BaseModel.h"

ListResult::ListResult(
    int page, int perPage, int totalItems, int totalPages,
    const QList<BaseModel*>& items)
    : m_page(page),
    m_perPage(perPage),
    m_totalItems(totalItems),
    m_totalPages(totalPages),
    m_items(items) {}

int ListResult::page() const {
    return m_page;
}

int ListResult::perPage() const {
    return m_perPage;
}

int ListResult::totalItems() const {
    return m_totalItems;
}

int ListResult::totalPages() const {
    return m_totalPages;
}

QList<BaseModel*> ListResult::items() const {
    return m_items;
}
