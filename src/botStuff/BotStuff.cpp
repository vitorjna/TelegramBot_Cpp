#include "BotStuff.h"
#include "botStuff/MessageProcessing.h"
#include "tools/CPUWatcher.h"

BotStuff *BotStuff::myBotStuff;
QList<BotStuff::BotCommand> BotStuff::myBotCommands = initBotCommands();

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

QList<BotStuff::BotCommand> BotStuff::initBotCommands()
{
    QList<BotStuff::BotCommand> myBotCommands;

    for (int nIndex = 0; nIndex < MessageProcessing::COUNT_BOT_KEYWORDS; ++nIndex) {
        switch (static_cast<MessageProcessing::BotKeywords>(nIndex)) {
            case MessageProcessing::HELLO:
                // case MessageProcessing::HI:
                myBotCommands.emplaceBack("hello", "Helloes");
                break;

            case MessageProcessing::QUOTE:
                // case MessageProcessing::Q:
                myBotCommands.emplaceBack("quote", "Futurama quote");
                break;

            case MessageProcessing::FUCK_YOU:
                break;

            case MessageProcessing::YOU_UP:
                break;

            case MessageProcessing::YOU_UP_Q:
                break;

            case MessageProcessing::U_UP:
                break;

            case MessageProcessing::U_UP_Q:
                break;

            case MessageProcessing::DID_YOU_HIT_HER:
                break;

            case MessageProcessing::CPU:
                // case MessageProcessing::CORES:
                myBotCommands.emplaceBack("cpu", "CPU usage");
                break;

            case MessageProcessing::LOCK:
                myBotCommands.emplaceBack("lock", "Lock computer running Bot");
                break;

            case MessageProcessing::BLOCK:
                break;

            case MessageProcessing::UNBLOCK:
                break;

            case MessageProcessing::QUIT:
                break;

            case MessageProcessing::COUNT_BOT_KEYWORDS:
            default:
                break;
        }
    }

    myBotCommands.emplaceBack("help_fun", "List of fun keywords");
    myBotCommands.emplaceBack("help_pc", "List of PC commands");

    return myBotCommands;
}

void BotStuff::send(const int nChatId, const QString &szMessage)
{
    getInstance()->sendMember(nChatId, szMessage);
}

void BotStuff::newMessage(const Telegram::Message &myMessage)
{
    qDebug() << "new message:" << myMessage;

    if (myBotApi != nullptr
        && myMessage.type == Telegram::Message::TextType) {

        if (myMessage.string[0] == '/') {
            emit newMessageReceived(myMessage.from, myMessage.string.mid(1, -1));

        } else {
            emit newMessageReceived(myMessage.from, myMessage.string);
        }
    }
}

BotStuff::BotStuff(QObject *parent)
    : QObject(parent)
{
    myBotApi = new Telegram::Bot(QStringLiteral(TOKEN), true, 500, 4, this);
    myMessageProcessing = new MessageProcessing();
    myWatcher = new CPUWatcher(CPUWatcher::CPUWatcherConfig(80, 10, 20, 2));

    setupSignalsAndSlots();
}

BotStuff::~BotStuff()
{
    if (myBotApi != nullptr) {
        delete myBotApi;
    }

    if (myMessageProcessing != nullptr) {
        delete myMessageProcessing;
    }

    if (myWatcher != nullptr) {
        delete myWatcher;
    }
}

void BotStuff::setupSignalsAndSlots()
{
    connect(this, &BotStuff::newMessageReceived, myMessageProcessing, &MessageProcessing::processNewMessage);
    connect(myWatcher, &CPUWatcher::highCpuSignal, this, [ this ] {
        this->send(-1, QStringLiteral("High CPU usage detected"));
    });
    connect(myWatcher, &CPUWatcher::lowCpuSignal, this, [ this ] {
        this->send(-1, QStringLiteral("Low CPU usage restored"));
    });
}

void BotStuff::startMember()
{
    QObject::connect(myBotApi, &Telegram::Bot::message, this, &BotStuff::newMessage);
    myWatcher->startMonitoring();

    if (myBotCommands.size() > 0) {
        const QJsonArray myBotCommandsJson = BotCommand::botCommandListToJsonArray(myBotCommands);
        myBotApi->setCommands(myBotCommandsJson);
    }

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
