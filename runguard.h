#ifndef RUNGUARD_H
#define RUNGUARD_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

#define APPKEY          "VISITOR_LOG_APP"

// https://stackoverflow.com/a/28172162

class RunGuard
{
public:
    RunGuard(const QString &key);
    ~RunGuard();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY(RunGuard)

};

#endif // RUNGUARD_H
