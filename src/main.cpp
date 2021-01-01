#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "enigneinterface.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine appEngine;

    EnigneInterface* gameBackend = new EnigneInterface(&appEngine);
    appEngine.rootContext()->setContextProperty("Engine", gameBackend);

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&appEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    appEngine.load(url);

    return app.exec();
}
