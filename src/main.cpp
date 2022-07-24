#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>

#include "engineinterface.h"
#include "scoreinterface.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine appEngine;

  int idRegular = QFontDatabase::addApplicationFont(":/datas/font/B612Mono-Regular.ttf");
  if (idRegular == -1) {
    throw std::runtime_error("Unable to load regular font from ressource");
  }
  QString family = QFontDatabase::applicationFontFamilies(idRegular).at(0);

  int idBold = QFontDatabase::addApplicationFont(":/datas/font/B612Mono-Bold.ttf");
  if (idBold == -1) {
    throw std::runtime_error("Unable to load bold font from ressource");
  }
  QFont monospace(family);
  app.setFont(monospace);

  EngineInterface gameBackend4x4(4, 4 * 4);
  appEngine.rootContext()->setContextProperty("gameBackend4x4", &gameBackend4x4);

  EngineInterface gameBackend5x5(5, 5 * 5);
  appEngine.rootContext()->setContextProperty("gameBackend5x5", &gameBackend5x5);

  ScoresInterface scoreBackend(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
  appEngine.rootContext()->setContextProperty("scoreBackend", &scoreBackend);

  const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
  QObject::connect(&appEngine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  appEngine.load(url);

  return app.exec();
}
