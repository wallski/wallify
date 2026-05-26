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

    SpotifyMigrator* spotifyMigrator = new SpotifyMigrator(library, &app);
    engine.rootContext()->setContextProperty("spotifyMigrator", spotifyMigrator);

    YouTubeMigrator* youtubeMigrator = new YouTubeMigrator(library, &app);
    engine.rootContext()->setContextProperty("youtubeMigrator", youtubeMigrator);

    SoundCloudMigrator* soundcloudMigrator = new SoundCloudMigrator(library, &app);
    engine.rootContext()->setContextProperty("soundcloudMigrator", soundcloudMigrator);

    // Wire settings → library: when the user changes path in settings, update LocalLibrary
    QObject::connect(&appSettings, &AppSettings::libraryPathChanged, library, [library, &appSettings]() {
        library->setLibraryPath(appSettings.libraryPath());
    });

    DiscordRPC* discordRPC = new DiscordRPC(&app);
    engine.rootContext()->setContextProperty("discordRPC", discordRPC);
    discordRPC->initialize();

    auto updateDiscordPresence = [player, discordRPC]() {
        QVariantMap track = player->currentTrack();
        if (track.isEmpty()) {
            discordRPC->clearPresence();
            return;
        }

        QString title = track["title"].toString();
        QString artist = track["artist"].toString();
        
        if (player->isPlaying()) {
            qint64 duration = track["duration"].toLongLong();
            qint64 position = player->position(); // in ms
            qint64 now = QDateTime::currentDateTime().toSecsSinceEpoch();
            
            qint64 startTimestamp = now - (position / 1000);
            qint64 endTimestamp = duration > 0 ? startTimestamp + (duration / 1000) : 0;

            discordRPC->updatePresence(
                "by " + artist,
                "Listening to " + title,
                "wallify_logo",
                "Wallify Music",
                startTimestamp,
                endTimestamp
            );
        } else {
            discordRPC->updatePresence(
                "by " + artist,
                "Paused - " + title,
                "wallify_logo",
                "Wallify Music",
                0,
                0
            );
        }
    };

    QObject::connect(player, &AudioPlayer::trackChanged, discordRPC, [updateDiscordPresence](const QVariantMap &) {
        updateDiscordPresence();
    });

    QObject::connect(player, &AudioPlayer::isPlayingChanged, discordRPC, [updateDiscordPresence]() {
        updateDiscordPresence();
    });

    const QUrl url(u"qrc:/Wallify/resources/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
