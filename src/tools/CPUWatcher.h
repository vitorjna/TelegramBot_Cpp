#pragma once

#include <QDebug>
#include <QThread>

class CPUWatcher : public QObject
{
    Q_OBJECT
    QThread myWorkerThread;

public:
    struct CPUWatcherConfig {
        int HIGH_THRESHOLD;
        int HIGH_DURATION;
        int LOW_THRESHOLD;
        int LOW_DURATION;

        CPUWatcherConfig(int nHighPerc = 80, int nHighSec = 4, int nLowPerc = 20, int nLowSec = 2)
            : HIGH_THRESHOLD(nHighPerc)
            , HIGH_DURATION(nHighSec)
            , LOW_THRESHOLD(nLowPerc)
            , LOW_DURATION(nLowSec) {
        }
    };

    explicit CPUWatcher(const CPUWatcherConfig &myConfig, QObject *parent = nullptr);
    ~CPUWatcher() override;

    void startMonitoring();

    void stopMonitoring();

private:
    void setupSignalsAndSlots();

    void startMonitoringSlot();

    void waitForStop();

    CPUWatcherConfig myConfig;

    QAtomicInt bRun;

signals:
    void startMonitoringSignal();

    void highCpuSignal();
    void lowCpuSignal();
};


