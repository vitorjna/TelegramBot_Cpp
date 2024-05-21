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
