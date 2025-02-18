#ifndef ABSTRACT_THREAD_H
#define ABSTRACT_THREAD_H

#include <QThread>
#include <QWaitCondition>

class AbstractThread : public QObject {
    Q_OBJECT

public:
    explicit AbstractThread() {
        mThread.moveToThread(&mThread);
        connect(&mThread, &QThread::started, this, &AbstractThread::runThread);
        connect(&mThread, &QThread::finished, this, &QObject::deleteLater);
    }
    ~AbstractThread() {
        terminateThread();
    }
    void start() {
        controlThread(true);
    }

private:
#if 0
    void runThread() {
        while (true) {
            controlThread(false);

            // add doing code
        }
    }
#else
    virtual void runThread() = 0;
#endif
    void terminateThread() {
        QMutexLocker locker(&mMutex);
        if (mThread.isRunning()) {
            mThread.quit();
            mThread.wait();
            qDebug() << "AbstractThread terminated.";
        }
    }
    void controlThread(const bool& start) {
        QMutexLocker lock(&mMutex);
        if (start) {
            if (mThread.isRunning() == false) {
                mThread.start();
            }
            mWaitCondition.wakeOne();
        } else {
            while (mThread.isRunning()) {
                mWaitCondition.wait(&mMutex);
            }
        }
    }

private slots:
    void slotStarted() {
        runThread();
    }

private:
    QThread mThread;
    QWaitCondition mWaitCondition;
    QMutex mMutex;
};

#endif  // ABSTRACT_THREAD_H
