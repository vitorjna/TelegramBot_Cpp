#include "CPUWatcher.h"
#include "botStuff/BotStuff.h"
#include "util/SystemUtils.h"
#include "util/TimeUtils.h"

CPUWatcher::CPUWatcher(const CPUWatcherConfig &myConfig, QObject *parent)
    : QObject() //no parent, will be moved to another thread
    , myConfig(myConfig)
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
        if (SystemUtils::GetCPULoad() > static_cast<float>(myConfig.HIGH_THRESHOLD / 100.0)) {
            ++nCyclesUp;

        } else {
            nCyclesUp = 0;
        }

        if (nCyclesUp >= myConfig.HIGH_DURATION * 2) {
            bHasStarted = true;
            break;
        }

        QThread::msleep(500);
    }

    if (bHasStarted == true) {
        emit highCpuSignal();
        waitForStop();
    }
}

void CPUWatcher::waitForStop()
{
    int nCyclesDown = 0;

    // TimeUtils::startTimer();

    bool bHasStopped = false;

    while (bRun == 1 && bHasStopped == false) {
        if (SystemUtils::GetCPULoad() < static_cast<float>(myConfig.LOW_THRESHOLD / 100.0)) {
            ++nCyclesDown;

        } else {
            nCyclesDown = 0;
        }

        if (nCyclesDown > myConfig.LOW_DURATION * 2) {
            bHasStopped = true;
            break;
        }

        QThread::msleep(500);
    }

    if (bHasStopped == true) {
        emit lowCpuSignal();
        // int64_t nlTimePassed = TimeUtils::getTimeMilliseconds();

        // if (nlTimePassed > 10 * 1000) { //if less than Xs (+ counters) were spent in high CPU usage, ignore
        // }
    }

    startMonitoringSlot();
}
