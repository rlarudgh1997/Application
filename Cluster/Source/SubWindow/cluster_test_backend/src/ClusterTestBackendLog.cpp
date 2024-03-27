#include "ClusterTestBackendLog.h"

#include <chrono>
#include <cstdarg>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <thread>
#include <utility>

#include <sys/syscall.h>
#include <unistd.h>

namespace cTest {

log_level globalLogLevel;

#define DATETIME_STR getDateTimeString()

static std::string getDateTimeString() {
    std::ostringstream ss;
    auto now = std::chrono::system_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = std::chrono::system_clock::to_time_t(now);
    std::tm* bt = std::localtime(&timer);
    ss << "[" << std::put_time(bt, "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count() << "]";
    return ss.str();
}

Logger::Logger() {
    mInitialized = false;
}

Logger::~Logger() {
}

void Logger::initialize(const std::string& appid, const std::string& description) {
    getInstance()->mInitialized = true;
    getInstance()->mDefaultContext = std::make_shared<LogContext>();
    getInstance()->mDefaultContext->mContextId = appid;
    getInstance()->mDefaultContext->mDescription = description;
}

Logger* Logger::getInstance() {
    static Logger logger;
    return &logger;
}

void Logger::addContext(const LogContextRef /*context*/) {
}

void Logger::logi(const LogContextRef context, const char* tag, const char* message) {
    static pid_t pid = getpid();
    thread_local const int64_t tid = syscall(__NR_gettid);
    std::cout << DATETIME_STR << ' ';
    std::cout << "[" << pid << ":" << tid << "]" << ' ';
    if (context->mContextId.length() == 0)
        std::cout << mDefaultContext->mContextId << " ";
    else
        std::cout << context->mContextId << " ";
    std::cout << "I/";
    std::cout << tag << ": ";
    std::cout << message;
    std::cout << std::endl;
}

void Logger::logw(const LogContextRef context, const char* tag, const char* message) {
    static pid_t pid = getpid();
    thread_local const int64_t tid = syscall(__NR_gettid);
    std::cout << "\033[33m" << DATETIME_STR << ' ';
    std::cout << "[" << pid << ":" << tid << "]" << ' ';
    if (context->mContextId.length() == 0)
        std::cout << mDefaultContext->mContextId << " ";
    else
        std::cout << context->mContextId << " ";
    std::cout << "W/";
    std::cout << tag << ": ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
}

void Logger::logf(const LogContextRef context, const char* tag, const char* message) {
    static pid_t pid = getpid();
    thread_local const int64_t tid = syscall(__NR_gettid);
    std::cout << "\033[1;4;31m" << DATETIME_STR << ' ';
    std::cout << "[" << pid << ":" << tid << "]" << ' ';
    if (context->mContextId.length() == 0)
        std::cout << mDefaultContext->mContextId << " ";
    else
        std::cout << context->mContextId << " ";
    std::cout << "F/";
    std::cout << tag << ": ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
}

void Logger::loge(const LogContextRef context, const char* tag, const char* message) {
    static pid_t pid = getpid();
    thread_local const int64_t tid = syscall(__NR_gettid);
    std::cout << "\033[1;31m" << DATETIME_STR << ' ';
    std::cout << "[" << pid << ":" << tid << "]" << ' ';
    if (context->mContextId.length() == 0)
        std::cout << mDefaultContext->mContextId << " ";
    else
        std::cout << context->mContextId << " ";
    std::cout << "E/";
    std::cout << tag << ": ";
    std::cout << message;
    std::cout << "\033[0m" << std::endl;
}

void Logger::logd(const LogContextRef context, const char* tag, const char* debugTag, const char* message) {
    static pid_t pid = getpid();
    thread_local const int64_t tid = syscall(__NR_gettid);
    std::cout << DATETIME_STR << ' ';
    std::cout << "[" << pid << ":" << tid << "]" << ' ';
    if (context->mContextId.length() == 0)
        std::cout << mDefaultContext->mContextId << " ";
    else
        std::cout << context->mContextId << " ";
    std::cout << "D/";
    std::cout << tag << ": ";
    std::cout << message;
    std::cout << " [" << debugTag << "]";
    std::cout << std::endl;
}

LogContext::LogContext() : mContextId(), mDescription() {
}

LogContext::LogContext(const char* contextId, const char* description) : mContextId(contextId), mDescription(description) {
}

void LogContext::registerLogger() {
}

void LogContext::registerContext(const char* contextId, const char* description) {
    if (contextId != nullptr) {
        mContextId = contextId;
    }
    if (description != nullptr) {
        mDescription = description;
    }
    registerLogger();
}

LogContext::~LogContext() {
}

LoggerHelper::LoggerHelper(const char* tag) : mLogTag(tag) {
}

LoggerHelper::LoggerHelper(const char* tag, std::string debugTag) : mLogTag(tag), mDebugTag(std::move(debugTag)) {
}

std::ostringstream& LoggerHelper::dummy() {
    return mStringStream;
}

std::ostringstream& LoggerHelper::dummy(const LogContextRef /*context*/) {
    return mStringStream;
}

std::ostringstream& LoggerHelper::logi() {
    mContext = Logger::getInstance()->mDefaultContext;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logi(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::logw() {
    mContext = Logger::getInstance()->mDefaultContext;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logw(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::logf() {
    mContext = Logger::getInstance()->mDefaultContext;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logf(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::loge() {
    mContext = Logger::getInstance()->mDefaultContext;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->loge(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::logd() {
    mContext = Logger::getInstance()->mDefaultContext;
    auto debugTag = mDebugTag;
    mWriter = [debugTag](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logd(contextRef, tag, debugTag.c_str(), outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::logi(const LogContextRef context) {
    mContext = context;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logi(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::logw(const LogContextRef context) {
    mContext = context;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logw(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::logf(const LogContextRef context) {
    mContext = context;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logf(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::loge(const LogContextRef context) {
    mContext = context;
    mWriter = [](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->loge(contextRef, tag, outString.c_str());
    };
    return mStringStream;
}

std::ostringstream& LoggerHelper::logd(const LogContextRef context) {
    mContext = context;
    auto debugTag = mDebugTag;
    mWriter = [debugTag](const LogContextRef contextRef, const char* tag, const std::string& outString) -> void {
        Logger::getInstance()->logd(contextRef, tag, debugTag.c_str(), outString.c_str());
    };
    return mStringStream;
}

LoggerHelper::~LoggerHelper() {
    if (globalLogLevel == DEBUG_LOG) {
        if ((mContext != nullptr) && (mWriter != nullptr)) {
            if (mLogTag.empty()) {
                mWriter(mContext, CLUSTER_TEST_LOG_TAG, mStringStream.str());
            } else {
                mWriter(mContext, mLogTag.c_str(), mStringStream.str());
            }
        } else {
            std::cout << mStringStream.str() << std::endl;
        }
    } else if (globalLogLevel == RELEASE_LOG) {
        if (mDebugTag.empty()) {
            if ((mContext != nullptr) && (mWriter != nullptr)) {
                if (mLogTag.empty()) {
                    mWriter(mContext, CLUSTER_TEST_LOG_TAG, mStringStream.str());
                } else {
                    mWriter(mContext, mLogTag.c_str(), mStringStream.str());
                }
            }
        }
    }
}

}  // namespace cTest
