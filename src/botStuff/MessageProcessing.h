#pragma once

#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QThread>

#include "botStuff/QtTelegramBot/qttelegrambot.h"

class MessageProcessing : public QObject
{
    Q_OBJECT

    enum Tasks {
        LOCK_PC,
        TURN_OFF_BOT,
        GET_CPU_USAGE,
        GET_CPU_CORES,
        COUNT_TASKS
    };

    enum BotKeywords {
        HELLO = 1, //lookup returns 0 when not finding a value, so the first entry has to be 0
        HI,
        QUIT,
        HELP,
        HALP,
        Q,
        QUOTE,
        QUOTES,
        CPU,
        CORE,
        CORES,
        LOCK,
        FUCK_YOU,
        YOU_UP,
        YOU_UP_Q,
        U_UP,
        U_UP_Q,
        DID_YOU_HIT_HER,
        COUNT_BOT_KEYWORDS
    };

public:
    enum Messages {
        ERROR_OCCURRED,
        NO_PERMISSION,
        COUNT_MESSAGES
    };

    explicit MessageProcessing(QObject *parent = nullptr);
    ~MessageProcessing() override;

    qint32 getMyChatId();

    bool isLoggedIn();

    QString getMessage(const Messages eMessage);

private:
    static QHash<QString, BotKeywords> myHashKeywords;

    static QHash<QString, BotKeywords> initHashKeywords();

    bool hasPermissions(const int nChatId, const bool bSendIfNotAllowed = false);

    QString getCpuUsage();

    void doTask(const Tasks eTask, const int nChatId);

    qint32 nMyChatId;

    QString getHelp();

public slots:
    void processNewMessage(const Telegram::User &myUser, const QString &szMessage);

};
