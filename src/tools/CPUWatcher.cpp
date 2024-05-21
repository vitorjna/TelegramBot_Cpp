#include "CPUWatcher.h"
#include "botStuff/BotStuff.h"
#include "util/SystemUtils.h"
#include "util/TimeUtils.h"

CPUWatcher::CPUWatcher(QObject *parent)
    : QObject() //no parent, will be moved to another thread
    , bRun(0)
{
    Q_UNUSED(parent)

    this->moveToThread(&myWorkerThread);
    QObject::connect(&myWorkerThread, SIGNAL(finished()), this, SLOT(deleteLater()));
    myWorkerThread.start();

    setupSignalsAndSlots();
}

CPUWatcher::~CPUWatcher()
{
    stopMonitoring();

    myWorkerThread.quit();
    myWorkerThread.wait(200);
}

void CPUWatcher::startMonitoring()
{
    if (bRun == 0) {
        bRun = 1;
        emit startMonitoringSignal();
    }
}

void CPUWatcher::stopMonitoring()
{
    bRun = 0;
}

void CPUWatcher::setupSignalsAndSlots()
{
    connect(this, &CPUWatcher::startMonitoringSignal, this, &CPUWatcher::startMonitoringSlot);
}

void CPUWatcher::startMonitoringSlot()
{
    int nCyclesUp = 0;

    bool bHasStarted = false;

    while (bRun == 1 && bHasStarted == false) {
        if (SystemUtils::GetCPULoad() > 0.8f) {
            ++nCyclesUp;

        } else {
            nCyclesUp = 0;
        }

        if (nCyclesUp >= 4) { //at least 2s at more than 80% usage
            bHasStarted = true;
            break;
        }

        QThread::msleep(500);
    }

    if (bHasStarted == true) {
        waitForStop();
    }
}

void CPUWatcher::waitForStop()
{
    int nCyclesDown = 0;

    TimeUtils::startTimer();

    bool bHasStopped = false;

    while (bRun == 1 && bHasStopped == false) {
        if (SystemUtils::GetCPULoad() < 0.2f) {
            ++nCyclesDown;

        } else {
            nCyclesDown = 0;
        }

        if (nCyclesDown > 4) { //at least 2s at less than 20% usage
            bHasStopped = true;
            break;
        }

        QThread::msleep(500);
    }

    if (bHasStopped == true) {
        int64_t dTimePassed = TimeUtils::getTimeMilliseconds();

        if (dTimePassed > 60 * 1000) { //if less than 30s (+ counters) were spent in high CPU usage, ignore
            BotStuff::send(-1, QStringLiteral("Compilation has stopped"));
        }
    }

    startMonitoringSlot();

}
