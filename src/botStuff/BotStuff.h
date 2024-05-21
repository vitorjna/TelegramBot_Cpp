#pragma once

#include <QAbstractEventDispatcher>
#include <QDebug>
#include <QObject>
#include <QThread>

#include "botStuff/QtTelegramBot/qttelegrambot.h"

class CPUWatcher;
class MessageProcessing;

class BotStuff : public QObject
{
    Q_OBJECT

public:
    static BotStuff *getInstance();

    static void start();

    static void send(const int nChatId, const QString &szMessage);

protected slots:
    void newMessage(const Telegram::Message &myMessage);

private:
    explicit BotStuff(QObject *parent = nullptr);
    ~BotStuff() override;

    static BotStuff *myBotStuff;

    void setupSignalsAndSlots();

    void startMember();

    void sendMember(const int nChatId, const QString &szMessage);

    Telegram::Bot       *myBotApi;

    MessageProcessing   *myMessageProcessing;
    CPUWatcher          *myWatcher;

signals:
    void newMessageReceived(const Telegram::User &myUser, const QString &szMessage);

};
