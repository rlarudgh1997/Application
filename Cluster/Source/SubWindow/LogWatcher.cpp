#include "LogWatcher.h"

#include <QDebug>
#include <QFile>

LogWatcher::LogWatcher() : mWatching(false) {
}

LogWatcher::~LogWatcher() {
    unSubscribeAll();
}

bool LogWatcher::watch() {
    mFile = fopen(mPath.c_str(), "r");  // example.txt 파일을 읽기 모드로 열기

    if (mFile == NULL) {
        // cError() << "file open failed...";
        return false;
    }

    // cDebug() << "You are now watching '" << mPath << "'";
    // if (globalLogLevel == log_level::DEBUG_LOG) {
    //     std::cout << std::endl;
    // }

    mWatching = true;
    mRunTheadPtr = std::make_unique<std::thread>(&LogWatcher::run, this);

    return true;
}

void LogWatcher::run() {
    long int size = 0;
    long int prevSize = 0;
    std::string filename = mPath.substr(mPath.rfind("/"));
    std::string prevLog;

    qDebug() << "Watcher File :" << QFile::exists(mPath.c_str()) << mPath.c_str();

    fseek(mFile, 0, SEEK_END);
    prevSize = ftell(mFile);
    prevLog = getPrevLog(prevSize);

    while (mWatching) {
        fseek(mFile, 0, SEEK_END);
        size = ftell(mFile);

        if (size < prevSize) {
            throwChange(prevLog.substr(static_cast<std::size_t>(size)));
            prevLog.clear();
            prevLog = getPrevLog(size);
            prevSize = size;
        } else if (size > prevSize) {
            // cDebug() << filename << " File size Changed";
            // cDebug() << filename << " Prev File size: " << prevSize << "[Byte]";
            // cDebug() << filename << " File size: " << size << "[Byte]";
            qDebug() << "File Changed :" << prevSize << "->" << size << "," << mPath.c_str();
            throwChange(getChange(static_cast<int>(size - prevSize), prevSize));
            prevLog.clear();
            prevLog = getPrevLog(size);
            prevSize = size;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

std::string LogWatcher::getPrevLog(int bufSize) {
    if ((bufSize < 0)) {
        qDebug() << "Fail to error buffer size :" << bufSize;
        return std::string();
    }

    std::string ret;
    fseek(mFile, 0, SEEK_SET);

    while (static_cast<int>(ret.length()) < bufSize) {
        char str[bufSize];
        fgets(str, bufSize, mFile);
        ret += std::string(str);
        // cDebug() << "bufSize: " << bufSize;
        // cDebug() << "ret.length(): " << ret.length();
    }
    return ret;
}

std::string LogWatcher::getChange(int bufSize, long int prevSize) {
    std::string ret;
    fseek(mFile, prevSize, SEEK_SET);

    while (static_cast<int>(ret.length()) < bufSize) {
        char str[bufSize];
        fgets(str, bufSize, mFile);
        ret += std::string(str);
        // cDebug() << "bufSize: " << bufSize;
        // cDebug() << "ret.length(): " << ret.length();
    }
    return ret;
}

void LogWatcher::throwChange(std::string textChanges) {
    if (mSendChangeCbkVector.size() == 0) {
        qDebug() << "Not emit signal - ata size : 0";
        return;
    }
    for (auto& cbk : mSendChangeCbkVector) {
        cbk(textChanges);
    }
}

void LogWatcher::join() {
    if (mRunTheadPtr) {
        mRunTheadPtr->join();
    }
    mRunTheadPtr.reset(nullptr);
}

void LogWatcher::stopWatch() {
    mWatching = false;
    if (mFile == NULL) {
        // cError() << "There is no file pointer...";
        return;
    }
    fclose(mFile);
    join();
}

bool LogWatcher::subscribeSignal(std::string signal) {
    if (mSubscribeSet.find(signal) != mSubscribeSet.end()) {
        // cWarning() << "The Signal(" << signal << ") already exist in subscribed signal List...";
        return false;
    }
    mSubscribeSet.insert(signal);
    return true;
}

// returns failed signal list.
// If the return vector is empty, all signals have been successfully subscribed.
std::vector<std::string> LogWatcher::subscribeSiganls(std::vector<std::string> signalList) {
    std::vector<std::string> temp;
    for (auto signal : signalList) {
        if (subscribeSignal(signal) == false) {
            temp.push_back(signal);
        }
    }
    return temp;
}

void LogWatcher::unSubscribe(std::string signal) {
    mSubscribeSet.erase(signal);
}

void LogWatcher::unSubscribeAll() {
    mSubscribeSet.clear();
}

std::set<std::string>& LogWatcher::getSubscribedSignals() {
    return mSubscribeSet;
}

// Start : Call back functions
stringToVoidCbk LogWatcher::getRegisterPathCbk() {
    return std::bind(&LogWatcher::registerPath, this, std::placeholders::_1);
}

voidToBoolCbk LogWatcher::getWatchCbk() {
    return std::bind(&LogWatcher::watch, this);
}

voidToVoidCbk LogWatcher::getStopWatchCbk() {
    return std::bind(&LogWatcher::stopWatch, this);
}

stringToBoolCbk LogWatcher::getSubscribeCbk() {
    return std::bind(&LogWatcher::subscribeSignal, this, std::placeholders::_1);
}

stringVectorToStringVectorCbk LogWatcher::getSubscribeVectorCbk() {
    return std::bind(&LogWatcher::subscribeSiganls, this, std::placeholders::_1);
}

stringToVoidCbk LogWatcher::getUnSubscribeCbk() {
    return std::bind(&LogWatcher::unSubscribe, this, std::placeholders::_1);
}

voidToVoidCbk LogWatcher::getUnSubscribeAllCbk() {
    return std::bind(&LogWatcher::unSubscribeAll, this);
}

void LogWatcher::registerSendChangeCbk(const stringToVoidCbk& cbk) {
    if (cbk != nullptr) {
        stringToVoidCbk newCbk = std::move(cbk);
        mSendChangeCbkVector.push_back(newCbk);
    }
}
// END : Call back functions