#ifndef MAZEGENERATIONALGORITHM_H
#define MAZEGENERATIONALGORITHM_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QTimer>
#include <QThread>
#include "global.h"
#include "virtualmap.h"


class MazeGenerationAlgorithm : public QObject
{
    Q_OBJECT

public:
    MazeGenerationAlgorithm(VirtualMap *vm, bool needsThread = false)
        : vm(vm), needsThread(needsThread)
    {
        vm->clear();

        qsrand(uint(time(nullptr)));

        if (needsThread) {
            this->workerThread = new QThread(this);
            connect(workerThread, SIGNAL(started()), this, SLOT(startInNewThread()));
            this->moveToThread(workerThread);
        } else {
            this->timerSteps = new QTimer();
            timerSteps->setInterval(msDelay);
            connect(timerSteps, SIGNAL(timeout()), this, SLOT(step()));
        }
    }

    virtual ~MazeGenerationAlgorithm() {
        if (needsThread) {
            workerThread->exit();
            delete workerThread;
        } else {
            this->timerSteps->stop();
            delete timerSteps;
        }
    }

    void start() {
        isWorking = true;
        if (needsThread) {
            workerThread->start();
        } else if (timerSteps != nullptr)
            timerSteps->start();
    }

    void stop() {
        isWorking = false;
        if (needsThread) {
//            workerThread->exit();
            workerThread->quit();
        } else {
            timerSteps->stop();
        }
    }

    bool IsWorking() {
        return isWorking;
    }

signals:
    void finished();

protected:
    VirtualMap *vm;
    int msDelay = 2;
    bool isWorking = false;

    // call after the last step to stop the algorithm
    void finishWork() {
        isWorking = false;

        if (needsThread) {
            workerThread->quit();
        } else {
            timerSteps->stop();
        }

        emit finished();
    }

protected slots:
    virtual void step() {}
    virtual void startInNewThread() {}


private:
    QTimer *timerSteps = nullptr;
    QThread *workerThread;
    bool needsThread = false;
};

#endif // MAZEGENERATIONALGORITHM_H
