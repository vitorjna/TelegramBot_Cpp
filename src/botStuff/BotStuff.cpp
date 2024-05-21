#include "BotStuff.h"
#include "botStuff/MessageProcessing.h"
#include "tools/CPUWatcher.h"
#include "util/MemoryUtils.h"
#include "util/SystemUtils.h"

BotStuff *BotStuff::myBotStuff;

BotStuff *BotStuff::getInstance()
{
    if (myBotStuff == nullptr) {
        myBotStuff = new BotStuff();
    }

    return myBotStuff;
}

void BotStuff::start()
{
    getInstance()->startMember();
}

void BotStuff::send(const int nChatId, const QString &szMessage)
{
    getInstance()->sendMember(nChatId, szMessage);
}

void BotStuff::newMessage(const Telegram::Message &myMessage)
{
    qDebug() << "new message:" << myMessage;

    if (myBotApi != nullptr && myMessage.type == Telegram::Message::TextType) {
        emit newMessageReceived(myMessage.from, myMessage.string);
    }
}

BotStuff::BotStuff(QObject *parent)
    : QObject(parent)
{
    myBotApi = new Telegram::Bot(QStringLiteral(TOKEN), true, 500, 4, this);
    myMessageProcessing = new MessageProcessing();
    myWatcher = new CPUWatcher();

    setupSignalsAndSlots();
}

BotStuff::~BotStuff()
{
    //TODO place utils in common project
    MemoryUtils::deletePointer(myBotApi);
    MemoryUtils::deletePointer(myMessageProcessing);
    MemoryUtils::deletePointer(myWatcher);
}

void BotStuff::setupSignalsAndSlots()
{
    connect(this, &BotStuff::newMessageReceived, myMessageProcessing, &MessageProcessing::processNewMessage);
}

void BotStuff::startMember()
{
    QObject::connect(myBotApi, &Telegram::Bot::message, this, &BotStuff::newMessage);
    myWatcher->startMonitoring();

    qDebug() << "Started Telegram Bot";
}

void BotStuff::sendMember(const int nChatId, const QString &szMessage)
{
    if (nChatId == -1) {
        if (myMessageProcessing->isLoggedIn() == true) {
            myBotApi->sendMessage(myMessageProcessing->getMyChatId(), szMessage);
        }

    } else {
        myBotApi->sendMessage(nChatId, szMessage);
    }
}

