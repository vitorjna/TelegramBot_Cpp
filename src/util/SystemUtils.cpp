#include "SystemUtils.h"

float SystemUtils::CalculateCPULoad(uint64_t idleTicks, uint64_t totalTicks)
{
    static uint64_t _previousTotalTicks = 0;
    static uint64_t _previousIdleTicks = 0;

    uint64_t totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    uint64_t idleTicksSinceLastTime  = idleTicks - _previousIdleTicks;

    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? (static_cast<float>(idleTicksSinceLastTime)) / totalTicksSinceLastTime : 0);

    _previousTotalTicks = totalTicks;
    _previousIdleTicks  = idleTicks;
    return ret;
}

uint64_t SystemUtils::FileTimeToInt64(const FILETIME &ft)
{
    return ((static_cast<uint64_t>(ft.dwHighDateTime)) << 32) | (static_cast<uint64_t>(ft.dwLowDateTime));
}

float SystemUtils::GetCPULoad()
{
    FILETIME idleTime, kernelTime, userTime;
    return GetSystemTimes(&idleTime, &kernelTime, &userTime) != 0 ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}

bool SystemUtils::lockPC()
{
    int nResult = LockWorkStation();
    return (nResult != 0);
}

bool SystemUtils::blockInput()
{
    return BlockInput(true) != 0;
}

bool SystemUtils::unblockInput()
{
    return BlockInput(false) != 0;
}

//bool SystemUtils::unlockPC(const QString &szPassword)
//{
//    char username[UNLEN + 1];
//    DWORD username_len = UNLEN + 1;
//    GetUserName(username, &username_len);

//    HANDLE sessToken = nullptr;
//    int nResult = LogonUserA(username, ".", szPassword.toStdString().c_str(), LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &sessToken);
//    return (nResult != 0);
//}

QString SystemUtils::getLastError()
{
    //Get the error message, if any.
    DWORD errorMessageID = GetLastError();

    if (errorMessageID == 0) {
        return "";
    }

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, nullptr);

    std::string message(static_cast<char *>(messageBuffer), size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return QString::fromStdString(message);
}
