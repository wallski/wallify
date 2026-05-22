#include "DiscordRPC.h"
#include <QDebug>

// Discord RPC structs (simplified inline implementation)
struct DiscordRichPresence {
    int type;
    qint64 applicationId;
    const char *state;
    const char *details;
    qint64 startTimestamp;
    qint64 endTimestamp;
    const char *largeImageKey;
    const char *largeImageText;
    const char *smallImageKey;
    const char *smallImageText;
    const char *partyId;
    int partySize;
    int partyMax;
    const char *matchSecret;
    const char *joinSecret;
    const char *spectateSecret;
    int instance;
};

struct DiscordUser {
    const char *userId;
    const char *username;
    const char *discriminator;
    const char *avatar;
};

typedef void (*DiscordReadyCallback)(const DiscordUser *user);
typedef void (*DiscordDisconnectedCallback)(int errcode, const char *message);
typedef void (*DiscordErrorCallback)(int errcode, const char *message);

struct DiscordEventHandlers {
    DiscordReadyCallback ready;
    DiscordDisconnectedCallback disconnected;
    DiscordErrorCallback errored;
    void *joinGame;
    void *spectateGame;
    void *joinRequest;
    void *userData;
};

#ifdef _WIN32
// Dynamic loading for Discord RPC
static HMODULE discordModule = nullptr;
typedef void (*Discord_Initialize_t)(const char *applicationId, DiscordEventHandlers *handlers, int autoRegister, const char *optionalSteamId);
typedef void (*Discord_Shutdown_t)(void);
typedef void (*Discord_UpdatePresence_t)(DiscordRichPresence *presence);
typedef void (*Discord_ClearPresence_t)(void);
typedef void (*Discord_RunCallbacks_t)(void);

static Discord_Initialize_t pDiscord_Initialize = nullptr;
static Discord_Shutdown_t pDiscord_Shutdown = nullptr;
static Discord_UpdatePresence_t pDiscord_UpdatePresence = nullptr;
static Discord_ClearPresence_t pDiscord_ClearPresence = nullptr;
static Discord_RunCallbacks_t pDiscord_RunCallbacks = nullptr;

static bool loadDiscordRPC() {
    if (discordModule) return true;
    discordModule = LoadLibraryA("discord-rpc.dll");
    if (!discordModule) return false;

    pDiscord_Initialize = (Discord_Initialize_t)GetProcAddress(discordModule, "Discord_Initialize");
    pDiscord_Shutdown = (Discord_Shutdown_t)GetProcAddress(discordModule, "Discord_Shutdown");
    pDiscord_UpdatePresence = (Discord_UpdatePresence_t)GetProcAddress(discordModule, "Discord_UpdatePresence");
    pDiscord_ClearPresence = (Discord_ClearPresence_t)GetProcAddress(discordModule, "Discord_ClearPresence");
    pDiscord_RunCallbacks = (Discord_RunCallbacks_t)GetProcAddress(discordModule, "Discord_RunCallbacks");

    return pDiscord_Initialize && pDiscord_Shutdown && pDiscord_UpdatePresence && pDiscord_ClearPresence;
}
#else
static bool loadDiscordRPC() { return false; }
#define pDiscord_Initialize nullptr
#define pDiscord_Shutdown nullptr
#define pDiscord_UpdatePresence nullptr
#define pDiscord_ClearPresence nullptr
#define pDiscord_RunCallbacks nullptr
#endif

DiscordRPC::DiscordRPC(QObject *parent)
    : QObject(parent), m_initialized(false)
{
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, [this]() {
        if (m_initialized && pDiscord_RunCallbacks) {
            pDiscord_RunCallbacks();
        }
    });
}

DiscordRPC::~DiscordRPC()
{
    shutdown();
}

void DiscordRPC::initialize()
{
    if (m_initialized) return;

    if (!loadDiscordRPC()) {
        qWarning() << "DiscordRPC: Failed to load discord-rpc.dll";
        return;
    }

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = [](const DiscordUser *user) {
        Q_UNUSED(user)
        qDebug() << "DiscordRPC: Connected to Discord";
    };
    handlers.disconnected = [](int errcode, const char *message) {
        qDebug() << "DiscordRPC: Disconnected" << errcode << message;
    };
    handlers.errored = [](int errcode, const char *message) {
        qDebug() << "DiscordRPC: Error" << errcode << message;
    };

    pDiscord_Initialize("1234567890123456789", &handlers, 1, nullptr);
    m_initialized = true;
    m_updateTimer->start(1000);

    qDebug() << "DiscordRPC: Initialized successfully";
}

void DiscordRPC::shutdown()
{
    if (!m_initialized) return;

    m_updateTimer->stop();
    if (pDiscord_Shutdown) {
        pDiscord_Shutdown();
    }
    m_initialized = false;

#ifdef _WIN32
    if (discordModule) {
        FreeLibrary(discordModule);
        discordModule = nullptr;
    }
#endif
}

void DiscordRPC::updatePresence(const QString &state, const QString &details,
                                 const QString &largeImageKey, const QString &largeImageText,
                                 qint64 startTimestamp, qint64 endTimestamp)
{
    if (!m_initialized || !pDiscord_UpdatePresence) return;

    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));

    QByteArray stateBytes = state.toUtf8();
    QByteArray detailsBytes = details.toUtf8();
    QByteArray largeImageKeyBytes = largeImageKey.toUtf8();
    QByteArray largeImageTextBytes = largeImageText.toUtf8();

    presence.state = stateBytes.constData();
    presence.details = detailsBytes.constData();
    presence.largeImageKey = largeImageKeyBytes.constData();
    presence.largeImageText = largeImageTextBytes.constData();
    presence.startTimestamp = startTimestamp;
    presence.endTimestamp = endTimestamp;
    presence.instance = 1;

    pDiscord_UpdatePresence(&presence);
}

void DiscordRPC::clearPresence()
{
    if (!m_initialized || !pDiscord_ClearPresence) return;
    pDiscord_ClearPresence();
}
