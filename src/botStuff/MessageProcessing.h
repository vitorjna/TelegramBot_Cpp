#pragma once

#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QThread>

#include "botStuff/QtTelegramBot/qttelegrambot.h"

class MessageProcessing : public QObject
{
    Q_OBJECT

public:
    enum Tasks {
        LOCK_PC,
        BLOCK_INPUT,
        UNBLOCK_INPUT,
        TURN_OFF_BOT,
        GET_CPU_USAGE,
        GET_CPU_CORES,
        COUNT_TASKS
    };

    enum BotKeywords {
        HELLO = 1, //lookup returns 0 when not finding a value, so the first entry has to be 0
        HI,
        Q,
        QUOTE,
        FUCK_YOU,
        YOU_UP,
        YOU_UP_Q,
        U_UP,
        U_UP_Q,
        DID_YOU_HIT_HER,

        CPU,
        CORES,
        LOCK,
        BLOCK,
        UNBLOCK,

        QUIT,
        COUNT_BOT_KEYWORDS
    };

    enum BotCommands {
        HALP = 1, //lookup returns 0 when not finding a value, so the first entry has to be 0
        HELP,
        COUNT_BOT_COMMANDS
    };

    enum HelpType {
        HELP_FUN,
        HELP_PC,
        COUNT_HELP_TYPE
    };

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
    static QList<QString> myListCommands;

    static QHash<QString, BotKeywords> initHashKeywords();
    static QList<QString> initListCommands();

    static BotCommands getCommand(const QString &szCommand);

    bool hasPermissions(const int nChatId, const bool bSendIfNotAllowed = false);

    QString getCpuUsage();

    void doTask(const Tasks eTask, const int nChatId);

    qint32 nMyChatId;

    QString getHelp(HelpType eType);

public slots:
    void processNewMessage(const Telegram::User &myUser, const QString &szMessage);

};
