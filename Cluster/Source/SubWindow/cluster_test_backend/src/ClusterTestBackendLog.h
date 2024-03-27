#pragma once

#ifndef CLUSTERTESTBACKENDLOG_H__
#define CLUSTERTESTBACKENDLOG_H__

#include <cstdarg>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <utility>
#include <vector>

#ifndef CLUSTER_TEST_LOG_TAG
#define CLUSTER_TEST_LOG_TAG __func__
#endif

#ifndef DISABLE_DEBUG_LOG
#define DISABLE_DEBUG_LOG false
#endif

namespace cTest {

enum log_level {
    RELEASE_LOG,
    DEBUG_LOG,
    NO_LOG
};

extern log_level globalLogLevel;

class LogContext {
public:
    LogContext(const char* contextId, const char* description);
    LogContext();
    ~LogContext();
    void registerLogger();
    void registerContext(const char* contextId, const char* description);

    std::string mContextId;
    std::string mDescription;
};

using LogContextRef = std::shared_ptr<LogContext>;

class Logger {
protected:
    Logger();
    virtual ~Logger();

public:
    void logi(const LogContextRef context, const char* tag, const char* message);
    void logw(const LogContextRef context, const char* tag, const char* message);
    void logf(const LogContextRef context, const char* tag, const char* message);
    void loge(const LogContextRef context, const char* tag, const char* message);
    void logd(const LogContextRef context, const char* tag, const char* debugTag, const char* message);

    void addContext(const LogContextRef context);

    static void initialize(const std::string& appId, const std::string& appDesc);
    static Logger* getInstance();

private:
    bool mInitialized;
    LogContextRef mDefaultContext;
    std::vector<LogContextRef> mContextList;
    friend class LoggerHelper;
};

class LoggerHelper {
public:
    LoggerHelper() = delete;
    explicit LoggerHelper(const char* tag);
    explicit LoggerHelper(const char* tag, std::string debugTag);
    ~LoggerHelper();

    std::ostringstream& dummy();
    std::ostringstream& dummy(const LogContextRef /*context*/);

    std::ostringstream& logi();
    std::ostringstream& logw();
    std::ostringstream& logf();
    std::ostringstream& loge();
    std::ostringstream& logd();

    std::ostringstream& logi(const LogContextRef context);
    std::ostringstream& logw(const LogContextRef context);
    std::ostringstream& logf(const LogContextRef context);
    std::ostringstream& loge(const LogContextRef context);
    std::ostringstream& logd(const LogContextRef context);

private:
    std::string mLogTag;
    std::string mDebugTag;
    std::ostringstream mStringStream;
    std::function<void(const LogContextRef context, const char*, std::string)> mWriter = nullptr;
    LogContextRef mContext = nullptr;
};

#define LOGGER_FILENAME ((strrchr(__FILE__, '/') != nullptr) ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOGGER_DEBUG_TAG (std::string(LOGGER_FILENAME) + ":" + std::to_string(__LINE__) + ", " + __func__)

#define cInfo LoggerHelper(CLUSTER_TEST_LOG_TAG).logi
#define cWarning LoggerHelper(CLUSTER_TEST_LOG_TAG).logw
#define cFatal LoggerHelper(CLUSTER_TEST_LOG_TAG).logf
#define cError LoggerHelper(CLUSTER_TEST_LOG_TAG).loge
#if !DISABLE_DEBUG_LOG
#define cDebug LoggerHelper(CLUSTER_TEST_LOG_TAG, LOGGER_DEBUG_TAG).logd
#else
#define cDebug LoggerHelper(CLUSTER_TEST_LOG_TAG).dummy
#endif

#define LOGF cFatal
#define LOGE cError
#define LOGW cWarning
#define LOGI cInfo
#define LOGD cDebug

}  // namespace cTest

#endif  // CLUSTERTESTBACKENDLOG_H__
