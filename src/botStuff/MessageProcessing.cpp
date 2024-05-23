#include "MessageProcessing.h"
#include "botStuff/BotStuff.h"
#include "util/FunUtils.h"
#include "util/SystemUtils.h"

/// 10 million iterations, 10 key/values:
/// First key:      Hash: 501 ms / IfElse: 339 ms       hi
/// Middle key:     Hash: 555 ms / IfElse: 605 ms       cpu
/// Non-Existing:   Hash: 526 ms / IfElse: 751 ms       message
/// First key:      Hash: 2033 ms / IfElse: 1809 ms     HI
/// Middle key:     Hash: 2111 ms / IfElse: 2236 ms     CPU
/// Non-Existing:   Hash: 2162 ms / IfElse: 2373 ms     MESSAGE
QHash<QString, MessageProcessing::BotKeywords> MessageProcessing::myHashKeywords   = initHashKeywords();
QList<QString>                                 MessageProcessing::myListCommands   = initListCommands();

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
        case MessageProcessing::COUNT_MESSAGES:
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
            //FUN
            case MessageProcessing::HELLO:
                myHash.insert(QStringLiteral("hello"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::HI:
                myHash.insert(QStringLiteral("hi"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::Q:
                myHash.insert(QStringLiteral("q"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::QUOTE:
                myHash.insert(QStringLiteral("quote"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::FUCK_YOU:
                myHash.insert(QStringLiteral("fuck you"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::YOU_UP:
                myHash.insert(QStringLiteral("you up"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::YOU_UP_Q:
                myHash.insert(QStringLiteral("you up?"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::U_UP:
                myHash.insert(QStringLiteral("u up"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::U_UP_Q:
                myHash.insert(QStringLiteral("u up?"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::DID_YOU_HIT_HER:
                myHash.insert(QStringLiteral("did you hit her?"), static_cast<BotKeywords>(nIndex));
                break;


            ///PC
            case MessageProcessing::CPU:
                myHash.insert(QStringLiteral("cpu"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::CORES:
                myHash.insert(QStringLiteral("cores"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::LOCK:
                myHash.insert(QStringLiteral("lock"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::BLOCK:
                myHash.insert(QStringLiteral("block"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::UNBLOCK:
                myHash.insert(QStringLiteral("unblock"), static_cast<BotKeywords>(nIndex));
                break;


            case MessageProcessing::QUIT:
                myHash.insert(QStringLiteral("quit"), static_cast<BotKeywords>(nIndex));
                break;

            case MessageProcessing::COUNT_BOT_KEYWORDS:
                break;
        }
    }

    return myHash;
}

QList<QString> MessageProcessing::initListCommands()
{
    QList<QString> myList;

    for (int nIndex = 0; nIndex < COUNT_BOT_COMMANDS; ++nIndex) {
        switch (static_cast<BotCommands>(nIndex)) {
            case MessageProcessing::HALP:
                myList.push_back(QStringLiteral("halp"));
                break;

            case MessageProcessing::HELP:
                myList.push_back(QStringLiteral("help"));
                break;

            case MessageProcessing::COUNT_BOT_COMMANDS:
                break;
        }
    }

    return myList;
}

MessageProcessing::BotCommands MessageProcessing::getCommand(const QString &szCommand)
{
    for (int nIndex = 0; nIndex < myListCommands.size(); ++nIndex) {
        if (szCommand.startsWith(myListCommands.at(nIndex))) {
            return static_cast<BotCommands>(nIndex);
        }
    }

    return COUNT_BOT_COMMANDS;
}

bool MessageProcessing::hasPermissions(const int nChatId, const bool bSendIfNotAllowed)
{
    const bool bIsAllowed = (isLoggedIn() && (nMyChatId == nChatId));

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
                const bool bResult = SystemUtils::lockPC();

                if (bResult == true) {
                    BotStuff::send(nMyChatId, QStringLiteral("PC has been locked"));

                } else {
                    BotStuff::send(nMyChatId, getMessage(ERROR_OCCURRED));
                }
            }

            break;

        case MessageProcessing::BLOCK_INPUT:
            if (hasPermissions(nChatId, true) == true) {

                bool bResult = SystemUtils::blockInput();

                if (bResult == true) {
                    BotStuff::send(nMyChatId, QStringLiteral("PC input has been blocked"));

                } else {
                    BotStuff::send(nMyChatId, getMessage(ERROR_OCCURRED));
                }
            }

            break;

        case MessageProcessing::UNBLOCK_INPUT:
            if (hasPermissions(nChatId, true) == true) {

                const bool bResult = SystemUtils::unblockInput();

                if (bResult == true) {
                    BotStuff::send(nMyChatId, QStringLiteral("PC input has been unblocked"));

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

        case MessageProcessing::COUNT_TASKS:
            break;
    }

}

QString MessageProcessing::getHelp(HelpType eType)
{
    QStringList szaHelp;

    switch (eType) {
        case MessageProcessing::HELP_FUN:
            szaHelp.push_back(QStringLiteral("Use any of the following commands:"));
            szaHelp.push_back(QStringLiteral("'hello'/'hi'"));
            szaHelp.push_back(QStringLiteral("'q'/'quote'"));
            szaHelp.push_back(QStringLiteral("'fuck you'"));
            szaHelp.push_back(QStringLiteral("'you up'/'u up'"));
            szaHelp.push_back(QStringLiteral("'did you hit her?'"));
            break;

        case MessageProcessing::HELP_PC:
            szaHelp.push_back(QStringLiteral("Use any of the following commands:"));
            szaHelp.push_back(QStringLiteral("'cpu'*"));
            szaHelp.push_back(QStringLiteral("'core'*"));
            szaHelp.push_back(QStringLiteral("'cores'*"));
            szaHelp.push_back(QStringLiteral("'lock'*"));
            szaHelp.push_back(QStringLiteral("'block'*"));
            szaHelp.push_back(QStringLiteral("'unlock'*"));
            szaHelp.push_back(QStringLiteral("'quit'*"));
            szaHelp.push_back(QStringLiteral("* requires permissions"));
            break;

        case MessageProcessing::COUNT_HELP_TYPE:
            szaHelp.push_back(QStringLiteral("Help is on the wae"));
            szaHelp.push_back(QStringLiteral("Use:"));
            szaHelp.push_back(QStringLiteral("'help fun' for help about fun commands"));
            szaHelp.push_back(QStringLiteral("'help pc' for help about PC commands"));
            break;
    }

    return szaHelp.join('\n');
}

void MessageProcessing::processNewMessage(const Telegram::User &myUser, const QString &szMessage)
{
    const QString szMessageLower = szMessage.toLower();

    qDebug() << "Message" << szMessage;

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

    if (eKeywordIndex != 0) {
        switch (eKeywordIndex) {
            ///FUN
            case MessageProcessing::HELLO:
            case MessageProcessing::HI:
                BotStuff::send(myUser.id, QStringLiteral("Hi! \\o"));
                break;

            case MessageProcessing::Q:
            case MessageProcessing::QUOTE:
                BotStuff::send(myUser.id, FunUtils::getQuoteFuturama());
                break;

            case MessageProcessing::FUCK_YOU:
                BotStuff::send(myUser.id, QStringLiteral("No, fuck YOU!"));
                QThread::msleep(1000);
                BotStuff::send(myUser.id, QStringLiteral(".|."));
                break;

            case MessageProcessing::YOU_UP:
            case MessageProcessing::YOU_UP_Q:
            case MessageProcessing::U_UP:
            case MessageProcessing::U_UP_Q:
                BotStuff::send(myUser.id, QStringLiteral("For you, I'm always up ;)"));
                break;

            case MessageProcessing::DID_YOU_HIT_HER:
                BotStuff::send(myUser.id, QStringLiteral("I did not hit her, I did not."));
                QThread::msleep(1500);
                BotStuff::send(myUser.id, "Oh, hi " + myUser.firstname);
                break;

            ///PC
            case MessageProcessing::CPU:
                doTask(GET_CPU_USAGE, myUser.id);
                break;

            case MessageProcessing::CORES:
                doTask(GET_CPU_CORES, myUser.id);
                break;

            case MessageProcessing::LOCK:
                doTask(LOCK_PC, myUser.id);
                break;

            case MessageProcessing::BLOCK:
                doTask(BLOCK_INPUT, myUser.id);
                break;

            case MessageProcessing::UNBLOCK:
                doTask(UNBLOCK_INPUT, myUser.id);
                break;


            case MessageProcessing::QUIT:
                doTask(TURN_OFF_BOT, myUser.id);
                break;

            case MessageProcessing::COUNT_BOT_KEYWORDS:
                BotStuff::send(myUser.id, "I'm sorry " + myUser.firstname + ", I'm afraid I can't do that");
                break;
        }

    } else {
        const BotCommands eCommandIndex = getCommand(szMessageLower);

        switch (eCommandIndex) {
            case MessageProcessing::HALP:
            case MessageProcessing::HELP: {

                const QString &szCommand = myListCommands.at(eCommandIndex);

                const QStringView szrCleanCommand = QStringView(szMessageLower).mid(szCommand.size() + 1, -1); // +1 -> space char

                //TODO these keywords should be in a hash-type container
                if (szrCleanCommand == QStringLiteral("fun")) {
                    BotStuff::send(myUser.id, getHelp(HELP_FUN));

                } else if (szrCleanCommand == QStringLiteral("pc")) {
                    BotStuff::send(myUser.id, getHelp(HELP_PC));

                } else {
                    BotStuff::send(myUser.id, getHelp(COUNT_HELP_TYPE));
                }

                break;
            }

            case MessageProcessing::COUNT_BOT_COMMANDS:
                BotStuff::send(myUser.id, "I'm sorry " + myUser.firstname + ", I'm afraid I can't do that");
                break;
        }
    }
}
