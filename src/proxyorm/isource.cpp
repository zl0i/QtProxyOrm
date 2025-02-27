#include "isource.h"

ProxyOrm::ISource::ISource() {}

QVariant ProxyOrm::ISource::data(int row, int role) const
{
    if (enabledCache && !cache[row][role].isNull()) {
        return cache[row][role];
    }
    QVariant result = templateData(row, role);
    if (enabledCache) {
        cache[row][role] = result;
    }
    return templateData(row, role);
}

void ProxyOrm::ISource::invalidateCache(int row, int role)
{
    if (row == -1 && role == -1) {
        cache.clear();
    } else if (role == -1) {
        cache[row].clear();
    } else {
        cache[row][role].clear();
    }
}
