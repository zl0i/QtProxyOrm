#ifndef WHERE_H
#define WHERE_H

#include <QObject>
#include <QRegularExpression>

namespace ProxyOrm {

class Where
{
public:
    enum TypeComparison {
        Equals,
        NotEquals,
        LessThen,
        LessEqThen,
        MoreThen,
        MoreEqThen,
        IsNull,
        Like,
        ILike
    };

    Where(TypeComparison comparison, QVariant condition);
    Where(const Where &other);
    Where &operator=(const Where &other);

    virtual bool isAccepted(QVariant value) const;

private:
    const QVariant condition;
    const TypeComparison comparison;

    template<typename T>
    bool typedComparison(T a, T b) const;
};
} // namespace ProxyOrm

#endif // WHERE_H
