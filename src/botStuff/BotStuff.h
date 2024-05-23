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

    struct BotCommand {
        QString szCommand; // 1-32 characters. Can contain only lowercase English letters, digits and underscores
        QString szDescription; // 1-256 characters.

        BotCommand(const QString &szCommand, const QString &szDescription)
            : szCommand(szCommand)
            , szDescription(szDescription) {
        }

        QJsonObject toJsonObject() const {
            return QJsonObject::fromVariantMap({std::pair<QString, QVariant>("command", this->szCommand),
                                                std::pair<QString, QVariant>("description", this->szDescription)});
        }

        static QJsonArray botCommandListToJsonArray(const QList<BotCommand> &myBotCommands) {

            QVariantList myBotCommandsList;
            myBotCommandsList.reserve(myBotCommands.size());

            for (const BotCommand &myBotCommand : myBotCommands) {
                myBotCommandsList.push_back(myBotCommand.toJsonObject());
            }

            return QJsonArray::fromVariantList(myBotCommandsList);
        }
    };

public:
    static BotStuff *getInstance();

    static void start();

    static QList<BotStuff::BotCommand> initBotCommands();

    static void send(const int nChatId, const QString &szMessage);

protected slots:
    void newMessage(const Telegram::Message &myMessage);

private:
    explicit BotStuff(QObject *parent = nullptr);
    ~BotStuff() override;

    static BotStuff *myBotStuff;

    static QList<BotCommand> myBotCommands;

    void setupSignalsAndSlots();

    void startMember();

    void sendMember(const int nChatId, const QString &szMessage);

    Telegram::Bot       *myBotApi;

    MessageProcessing   *myMessageProcessing;
    CPUWatcher          *myWatcher;

signals:
    void newMessageReceived(const Telegram::User &myUser, const QString &szMessage);

};
