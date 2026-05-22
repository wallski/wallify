#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include "SpotifyMigrator.h"
#include "YouTubeMigrator.h"
#include "SoundCloudMigrator.h"
#include "AppSettings.h"
#include "LocalLibrary.h"
#include "AudioPlayer.h"
#include "CoverImageProvider.h"
#include "DiscordRPC.h"

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

    SpotifyMigrator* spotifyMigrator = new SpotifyMigrator(library);
    engine.rootContext()->setContextProperty("spotifyMigrator", spotifyMigrator);

    YouTubeMigrator* youtubeMigrator = new YouTubeMigrator(library);
    engine.rootContext()->setContextProperty("youtubeMigrator", youtubeMigrator);

    SoundCloudMigrator* soundcloudMigrator = new SoundCloudMigrator(library);
    engine.rootContext()->setContextProperty("soundcloudMigrator", soundcloudMigrator);

    DiscordRPC* discordRPC = new DiscordRPC(&app);
    engine.rootContext()->setContextProperty("discordRPC", discordRPC);
    discordRPC->initialize();

    QObject::connect(player, &AudioPlayer::trackChanged, discordRPC, [discordRPC](const QVariantMap &track) {
        if (!track.isEmpty()) {
            QString title = track["title"].toString();
            QString artist = track["artist"].toString();
            qint64 duration = track["duration"].toLongLong();
            qint64 now = QDateTime::currentDateTime().toSecsSinceEpoch();
            qint64 endTime = duration > 0 ? now + (duration / 1000) : 0;

            discordRPC->updatePresence(
                "Listening to " + title,
                "by " + artist,
                "wallify_logo",
                "Wallify Music",
                now,
                endTime
            );
        } else {
            discordRPC->clearPresence();
        }
    });

    const QUrl url(u"qrc:/Wallify/resources/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
