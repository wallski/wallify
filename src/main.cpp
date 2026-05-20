#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include "SpotifyMigrator.h"
#include "AppSettings.h"
#include "LocalLibrary.h"
#include "AudioPlayer.h"
#include "CoverImageProvider.h"

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

    LocalLibrary* library = new LocalLibrary(&app);
    engine.rootContext()->setContextProperty("localLibrary", library);

    AudioPlayer* player = new AudioPlayer(library, &app);
    engine.rootContext()->setContextProperty("audioPlayer", player);

    engine.addImageProvider("covers", new CoverImageProvider(library));

    SpotifyMigrator migrator(library);
    engine.rootContext()->setContextProperty("spotifyMigrator", &migrator);

    const QUrl url(u"qrc:/Wallify/resources/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
