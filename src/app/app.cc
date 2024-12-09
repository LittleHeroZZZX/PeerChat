#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Client.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Client client;
    engine.rootContext()->setContextProperty("client", &client);

    const QUrl url(QStringLiteral("qrc:/qmls/main.qml"));
    // const QUrl url(QStringLiteral(
    //     "file:///C:/Users/ZhouXin/projects/homework/networkProgram/"
    //     "peerchat/qmls/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
