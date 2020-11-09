#ifndef PATHFINDINGALGORITHM_H
#define PATHFINDINGALGORITHM_H

#include <QVector>
#include <QObject>
#include <QTimer>
#include "virtualmap.h"
#include "global.h"

class PathfindingAlgorithm : public QObject
{
    Q_OBJECT

public:
    // this constructor must be called explicitly in the subclass' constructor to initialize vm.
    PathfindingAlgorithm(VirtualMap *vm) {
        timerStep = new QTimer(this);
        timerStep->setInterval(20);
        connect(timerStep, SIGNAL(timeout()), this, SLOT(makeSteps()));

        timerPath = new QTimer(this);
        timerPath->setInterval(10);
        connect(timerPath, SIGNAL(timeout()), this, SLOT(PathStep()));

        this->vm = vm;
        connect(vm, SIGNAL(pathChanged()), this, SLOT(FindPathInstantly()));
    }

    virtual ~PathfindingAlgorithm() {
        delete timerPath;
        delete timerStep;
    }

    bool IsInProcess() {
        return isInProcess;
    }

    // Sets the speed of adding nodes. For the path we just set the timer interval
    // The step() function actualy does several steps (stepsPerTime) when called.
    // that is because timer delay cannot be less than 1ms as I want.
    void SetSpeed(int speed) {
        int min = 1;
        int max = 20;
        if (speed < min) speed = min;
        if (speed > max) speed = max;

        if (speed >= 10)
            this->stepsPerTime = speed - 9;
        else {
            stepsPerTime = 1;
            timerStep->setInterval((10 - speed) * 10);
        }

        timerPath->setInterval(50 - ((speed-1) * 50 / max) + 1);
    }

    void Start() {
        onStart();

        isInProcess = true;

        isFinihsed = false;
        isInstantUpdateMode = false;

        timerStep->start();
    }

    void Stop() {
        onStop();

        timerStep->stop();
        timerPath->stop();
        isInProcess = false;
    }

    bool isInstantUpdateMode = false;
    virtual void step()=0;

signals:
    void finished(); // called after path is found and drawn

protected slots:
    virtual void PathStep()=0;

    virtual void onStart()=0;
    virtual void onStop()=0;

    void FindPathInstantly() {
        isInstantUpdateMode = true;
        isFinihsed = false;
        onStart();
        while (!isFinihsed) {
            step();
        }
    }

    void makeSteps() {
        for (int i = 0; i < stepsPerTime && !isFinihsed; i++) {
            step();
        }
    }


    void drawPathInstantly() {
        while (!isPathFinished) {
            PathStep();
        }
    }

    void onStepsFinished() {
        isFinihsed = true;
        timerStep->stop();
        StartDrawingPath();
    }

    void onPathFinished() {
        timerStep->stop();
        timerPath->stop();
        isInstantUpdateMode = true;
        isInProcess = false;
        isPathFinished = true;
        isFinihsed = true;

        emit finished();
    }

protected:
    QTimer *timerStep;
    QTimer *timerPath;
    int stepsPerTime = 1;
    bool isInProcess = false;
    bool isFinihsed = false;
    bool isPathFinished = false;

    VirtualMap *vm;

private:
    void StartDrawingPath() {
        isPathFinished = false;
        if (!isInstantUpdateMode) {
            timerPath->start();
        } else {
            drawPathInstantly();
        }
    }

};

#endif // PATHFINDINGALGORITHM_H
