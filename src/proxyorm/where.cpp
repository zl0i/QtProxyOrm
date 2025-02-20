#include "where.h"

ProxyOrm::Where::Where(TypeComparison comparison, QVariant condition)
    : condition(condition)
    , comparison(comparison)
{}

ProxyOrm::Where::Where(const Where &other)
    : condition(other.condition)
    , comparison(other.comparison)
{}

ProxyOrm::Where &ProxyOrm::Where::operator=(const Where &other)
{
    if (this == &other) {
        return *this;
    }
    return *new (this) Where(other);
}

bool ProxyOrm::Where::isAccepted(QVariant value) const
{
    if (condition.typeId() != value.typeId()) {
        qWarning() << "Incompatible types for comparison: condition type =" << condition.typeId()
                   << ", value type =" << value.typeId();
        return false;
    }

    if (comparison == TypeComparison::IsNull && value.isNull()) {
        return true;
    }
    if (comparison == TypeComparison::Equals && value == condition) {
        return true;
    }
    if (comparison == TypeComparison::NotEquals && value != condition) {
        return true;
    }
    if (comparison == TypeComparison::Like) {
        if (condition.typeId() != QMetaType::QString && value.typeId() != QMetaType::QString)
            return false;

        return value.toString().contains(condition.toString());
    }

    if (comparison == TypeComparison::ILike) {
        if (condition.typeId() != QMetaType::QString && value.typeId() != QMetaType::QString)
            return false;

        return !value.toString().contains(condition.toString());
    }

    switch (value.typeId()) {
    case QMetaType::Int:
        return typedComparison(value.toInt(), condition.toInt());
    case QMetaType::Float:
        return typedComparison(value.toFloat(), condition.toFloat());
    case QMetaType::Double:
        return typedComparison(value.toDouble(), condition.toDouble());
    }
    return false;
}

template<typename T>
bool ProxyOrm::Where::typedComparison(T a, T b) const
{
    if (comparison == TypeComparison::LessThen) {
        return a < b;
    }
    if (comparison == TypeComparison::LessEqThen) {
        return a <= b;
    }
    if (comparison == TypeComparison::GreaterThen) {
        return a > b;
    }
    if (comparison == TypeComparison::GreaterEqThen) {
        return a >= b;
    }
    if (comparison == TypeComparison::Equals) {
        return a == b;
    }
    if (comparison == TypeComparison::NotEquals) {
        return a != b;
    }
    return false;
}
