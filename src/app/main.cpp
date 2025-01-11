#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ordermodel.h"
#include "ordertotalvalue.h"
#include "orderviewmodel.h"
#include "staffmodel.h"
#include "usermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    StaffModel staffModel;
    UserModel userModel;
    OrderModel orderModel;
    OrderViewModel proxyModel(&orderModel, &userModel, &staffModel);
    engine.rootContext()->setContextProperty("orderModel", &orderModel);
    engine.rootContext()->setContextProperty("userModel", &userModel);
    engine.rootContext()->setContextProperty("staffModel", &staffModel);
    engine.rootContext()->setContextProperty("proxyModel", &proxyModel);

    OrderTotalValue totalOrder(&orderModel);
    engine.rootContext()->setContextProperty("totalOrder", &totalOrder);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ProxyOrm", "Main");

    return app.exec();
}
