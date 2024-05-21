#pragma once

#include <QDebug>
#include <QThread>

class CPUWatcher : public QObject
{
    Q_OBJECT
    QThread myWorkerThread;

public:
    explicit CPUWatcher(QObject *parent = nullptr);
    ~CPUWatcher() override;

    void startMonitoring();

    void stopMonitoring();

private:
    void setupSignalsAndSlots();

    void startMonitoringSlot();

    void waitForStop();

    QAtomicInt bRun;

signals:
    void startMonitoringSignal();


};


