#pragma once

#include <QDebug>

#include <windows.h>
#include <Lmcons.h>
#include <winable.h>

class SystemUtils
{
public:
    ///############################ CPU load ############################
//https://stackoverflow.com/questions/23143693/retrieving-cpu-load-percent-total-in-windows-with-c
    static float CalculateCPULoad(uint64_t idleTicks, uint64_t totalTicks);
    static uint64_t FileTimeToInt64(const FILETIME &ft);
    // Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
    // You'll need to call this at regular intervals, since it measures the load between
    // the previous call and the current one.  Returns -1.0 on error.
    static float GetCPULoad();

    static bool lockPC();

//    static bool unlockPC(const QString &szPassword);

    static bool blockInput();
    static bool unblockInput();

    static QString getLastError();

};

