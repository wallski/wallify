#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QQmlContext>
#include "SpotifyMigrator.h"
#include "AppSettings.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("Wallski");
    app.setApplicationName("Wallify");

    int robotoId = QFontDatabase::addApplicationFont(":/Wallify/resources/fonts/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/Wallify/resources/fonts/BebasNeue-Regular.ttf");
    
    if (robotoId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(robotoId).at(0);
        app.setFont(QFont(family));
    }

    QQmlApplicationEngine engine;
    
    AppSettings appSettings;
    engine.rootContext()->setContextProperty("appSettings", &appSettings);

    SpotifyMigrator migrator;
    engine.rootContext()->setContextProperty("spotifyMigrator", &migrator);

    const QUrl url(u"qrc:/Wallify/resources/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
