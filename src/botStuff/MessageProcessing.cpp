#include "MessageProcessing.h"
#include "botStuff/BotStuff.h"
#include "tools/CPUWatcher.h"
#include "util/FunUtils.h"
#include "util/MemoryUtils.h"
#include "util/SystemUtils.h"
#include "util/TimeUtils.h"

/// 10 million iterations, 10 key/values:
/// First key:      Hash: 501 ms / IfElse: 339 ms       hi
/// Middle key:     Hash: 555 ms / IfElse: 605 ms       cpu
/// Non-Existing:   Hash: 526 ms / IfElse: 751 ms       message
/// First key:      Hash: 2033 ms / IfElse: 1809 ms     HI
/// Middle key:     Hash: 2111 ms / IfElse: 2236 ms     CPU
/// Non-Existing:   Hash: 2162 ms / IfElse: 2373 ms     MESSAGE
QHash<QString, MessageProcessing::BotKeywords> MessageProcessing::myHashKeywords = initHashKeywords();

MessageProcessing::MessageProcessing(QObject *parent)
    : QObject(parent)
{
    nMyChatId = -1;
}

MessageProcessing::~MessageProcessing()
{
}

qint32 MessageProcessing::getMyChatId()
{
    return nMyChatId;
}

bool MessageProcessing::isLoggedIn()
{
    return nMyChatId != -1;
}

QString MessageProcessing::getMessage(const MessageProcessing::Messages eMessage)
{
    switch (eMessage) {
        case MessageProcessing::ERROR_OCCURRED:
        default:
            return QStringLiteral("Oh dear, it seems an error has occurred ¯\\_(ツ)_/¯");

        case MessageProcessing::NO_PERMISSION:
            return QStringLiteral("Ah ah ah! You didn't say the magic word! Ah ah ah!");
    }
}

QHash<QString, MessageProcessing::BotKeywords> MessageProcessing::initHashKeywords()
{
    QHash<QString, MessageProcessing::BotKeywords> myHash;

    for (int nIndex = 0; nIndex < COUNT_BOT_KEYWORDS; ++nIndex) {
        switch (static_cast<BotKeywords>(nIndex)) {
            case MessageProcessing::DID_YOU_HIT_HER:
                myHash.insert(QStringLiteral("did you hit her?"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::YOU_UP_Q:
                myHash.insert(QStringLiteral("you up?"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::U_UP_Q:
                myHash.insert(QStringLiteral("u up?"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::YOU_UP:
                myHash.insert(QStringLiteral("you up"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::U_UP:
                myHash.insert(QStringLiteral("u up"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::HELP:
                myHash.insert(QStringLiteral("help"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::HALP:
                myHash.insert(QStringLiteral("halp"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::HELLO:
                myHash.insert(QStringLiteral("hello"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::HI:
                myHash.insert(QStringLiteral("hi"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::QUIT:
                myHash.insert(QStringLiteral("quit"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::Q:
                myHash.insert(QStringLiteral("q"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::QUOTE:
                myHash.insert(QStringLiteral("quote"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::QUOTES:
                myHash.insert(QStringLiteral("quotes"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::CPU:
                myHash.insert(QStringLiteral("cpu"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::CORE:
                myHash.insert(QStringLiteral("core"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::CORES:
                myHash.insert(QStringLiteral("cores"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::LOCK:
                myHash.insert(QStringLiteral("lock"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::FUCK_YOU:
                myHash.insert(QStringLiteral("fuck you"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::COUNT_BOT_KEYWORDS:
                break;
        }
    }

    return myHash;
}

bool MessageProcessing::hasPermissions(const int nChatId, const bool bSendIfNotAllowed)
{
    bool bIsAllowed = (isLoggedIn() && (nMyChatId == nChatId));

    if (bSendIfNotAllowed == true
        && bIsAllowed == false) {
        BotStuff::send(nChatId, getMessage(NO_PERMISSION));
    }

    return bIsAllowed;
}

QString MessageProcessing::getCpuUsage()
{
    return QString::number(static_cast<int>(SystemUtils::GetCPULoad() * 100));
}

void MessageProcessing::doTask(const MessageProcessing::Tasks eTask, const int nChatId)
{
    switch (eTask) {
        case MessageProcessing::TURN_OFF_BOT:
            if (hasPermissions(nChatId, true) == true) {
                BotStuff::send(nMyChatId, QStringLiteral("Bye bye! ヾ(°∇°*)"));
                exit(0);
            }

            break;

        case MessageProcessing::LOCK_PC:
            if (hasPermissions(nChatId, true) == true) {
                bool bResult = SystemUtils::lockPC();

                if (bResult == true) {
                    BotStuff::send(nMyChatId, QStringLiteral("PC has been locked"));

                } else {
                    BotStuff::send(nMyChatId, getMessage(ERROR_OCCURRED));
                }
            }

            break;

        case MessageProcessing::GET_CPU_USAGE:
            if (hasPermissions(nChatId, true) == true) {
                BotStuff::send(nMyChatId, "CPU usage is " + getCpuUsage() + '%');
            }

            break;

        case MessageProcessing::GET_CPU_CORES:
            if (hasPermissions(nChatId, true) == true) {
                BotStuff::send(nMyChatId, "CPU has " + QString::number(QThread::idealThreadCount()) + " logical cores");
            }

            break;

        default:
            break;
    }

}

QString MessageProcessing::getHelp()
{
    QString szHelp;

    //TODO build help list
    szHelp.append("Help is on the wae");

    return szHelp;
}

void MessageProcessing::processNewMessage(const Telegram::User &myUser, const QString &szMessage)
{
    QString szMessageLower = szMessage.toLower();

    if (nMyChatId == -1) {
        if (myUser.username == QStringLiteral("vitorjna")) {
            BotStuff::send(myUser.id, "Oh, hi " + myUser.firstname);
            nMyChatId = myUser.id;

            if (szMessageLower == QStringLiteral("hello")
                || szMessageLower == QStringLiteral("hi")) {
                return;
            }
        }
    }

    BotKeywords eKeywordIndex = myHashKeywords.value(szMessageLower);

    switch (eKeywordIndex) {
        case MessageProcessing::DID_YOU_HIT_HER:
            BotStuff::send(myUser.id, QStringLiteral("I did not hit her, I did not."));
            QThread::msleep(1500);
            BotStuff::send(myUser.id, "Oh, hi " + myUser.firstname);
            break;

        case MessageProcessing::YOU_UP:
        case MessageProcessing::YOU_UP_Q:
        case MessageProcessing::U_UP:
        case MessageProcessing::U_UP_Q:
            BotStuff::send(myUser.id, QStringLiteral("For you, I'm always up ;)"));
            break;

        case MessageProcessing::HELP:
        case MessageProcessing::HALP:
            BotStuff::send(myUser.id, this->getHelp());
            break;

        case MessageProcessing::HELLO:
        case MessageProcessing::HI:
            BotStuff::send(myUser.id, QStringLiteral("Hi! \\o"));
            break;

        case MessageProcessing::QUIT:
            doTask(TURN_OFF_BOT, myUser.id);
            break;

        case MessageProcessing::Q:
        case MessageProcessing::QUOTE:
        case MessageProcessing::QUOTES:
            BotStuff::send(myUser.id, FunUtils::getQuoteFuturama());
            break;

        case MessageProcessing::CPU:
            doTask(GET_CPU_USAGE, myUser.id);
            break;

        case MessageProcessing::CORE:
        case MessageProcessing::CORES:
            doTask(GET_CPU_CORES, myUser.id);
            break;

        case MessageProcessing::LOCK:
            doTask(LOCK_PC, myUser.id);
            break;

        case MessageProcessing::FUCK_YOU:
            BotStuff::send(myUser.id, QStringLiteral("No, fuck YOU!"));
            QThread::msleep(1000);
            BotStuff::send(myUser.id, QStringLiteral(".|."));
            break;

        case MessageProcessing::COUNT_BOT_KEYWORDS:
        default:
            BotStuff::send(myUser.id, "I'm sorry " + myUser.firstname + ", I'm afraid I can't do that");
            break;
    }

}

