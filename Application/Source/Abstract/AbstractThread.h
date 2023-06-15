#ifndef ABSTRACT_THREAD_H
#define ABSTRACT_THREAD_H

#include <QObject>

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QSharedPointer>



class AbstractThread : public QThread {
public:
    enum {
        ThreadStateStop = 0,
        ThreadStateStart,
        ThreadStatePause,
        ThreadStateResume,
    };

    Q_OBJECT

public:
    AbstractThread();
    ~AbstractThread();
    void controlThread(const int& threadState = ThreadStateResume);


private:
    virtual void threadRun() = 0;


protected:
    void run();


public:
    QMutex mMutex;


private:
    QWaitCondition mPauseCondition;
    int mThreadState = ThreadStateStop;
};

#endif    // ABSTRACT_THREAD_H
