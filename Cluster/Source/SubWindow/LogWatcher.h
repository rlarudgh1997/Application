#pragma once

#ifndef LOGWATCHER_H__
#define LOGWATCHER_H__

#define _CRT_SECURE_NO_WARNINGS  // Visual Studio에서 보안 경고를 무시하도록 설정

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <QDebug>
// #include "ClusterTestBackendLog.h"

using voidToVoidCbk = std::function<void()>;
using voidToBoolCbk = std::function<bool()>;
// using voidToStringCbk = std::function<std::string()>;

using stringToVoidCbk = std::function<void(std::string signal)>;
using stringToBoolCbk = std::function<bool(std::string signal)>;

using stringVectorToStringVectorCbk = std::function<std::vector<std::string>(std::vector<std::string> signalList)>;

// using namespace cTest;

class LogWatcher {
public:
    LogWatcher();
    ~LogWatcher();

    inline void registerPath(std::string filePath) {
        mPath = filePath;
    }
    stringToVoidCbk getRegisterPathCbk();

    bool watch();
    voidToBoolCbk getWatchCbk();

    void stopWatch();
    voidToVoidCbk getStopWatchCbk();

    bool subscribeSignal(std::string signal);
    stringToBoolCbk getSubscribeCbk();

    std::vector<std::string> subscribeSiganls(std::vector<std::string> signalList);
    stringVectorToStringVectorCbk getSubscribeVectorCbk();

    void unSubscribe(std::string signal);
    stringToVoidCbk getUnSubscribeCbk();

    void unSubscribeAll();
    voidToVoidCbk getUnSubscribeAllCbk();

    std::set<std::string>& getSubscribedSignals();

    void registerSendChangeCbk(const stringToVoidCbk& cbk);

private:
    void run();
    std::string getPrevLog(int bufSize);
    std::string getChange(int bufSize, long int prevSize);
    void throwChange(std::string textChanges);
    void join();

    bool mWatching;
    std::unique_ptr<std::thread> mRunTheadPtr;
    std::string mPath;
    FILE* mFile;
    std::set<std::string> mSubscribeSet;
    std::vector<stringToVoidCbk> mSendChangeCbkVector;
};

#endif  // LOGWATCHER_H__