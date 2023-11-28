#include "AbstractThread.h"

AbstractThread::AbstractThread() {
    connect(this, &QThread::finished, this, &QObject::deleteLater);
    controlThread(ThreadStateStart);
}

AbstractThread::~AbstractThread() {
    controlThread(ThreadStateStop);
}

void AbstractThread::controlThread(const int& threadState) {
    QMutexLocker lock(&mMutex);
    mThreadState = threadState;

    if (threadState == ThreadStateStart) {
        if (this->isRunning() == false) {
            this->start();
        }
    } else if (threadState == ThreadStatePause) {
        mPauseCondition.wait(&mMutex);
    } else if (threadState == ThreadStateResume) {
        mPauseCondition.wakeAll();
    } else {
        // do nothing
    }
}

void AbstractThread::run() {
    while (mThreadState) {
        if (mThreadState == ThreadStateStop) {
            break;
        }

        if (mThreadState == ThreadStateResume) {
            threadRun();
        } else {
            msleep(500);
        }
    }

    this->quit();
    this->wait();
}
