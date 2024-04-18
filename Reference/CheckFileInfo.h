#ifndef CHECK_FILE_INFO_H
#define CHECK_FILE_INFO_H

#include <sys/inotify.h>

#include <HCommon.h>
#include "VariantServiceLog.h"

namespace ccos {
namespace sys {
namespace variantservice {

#define LOG_TAG_CHECK_FILE_INFO "CheckFileInfo"

class CheckFileInfo {
public:
    CheckFileInfo() {
    }
    ~CheckFileInfo() {
        join();
    }
    void setFileInfo(const std::string& file) {
        mCheckFile = file;
        mBufferSize = sizeof(struct inotify_event) + file.size() + 1;
    }
    void start() {
        LOGD(LOG_TAG_CHECK_FILE_INFO) << std::string("CheckFileInfo : start");
        mThread = std::thread(&CheckFileInfo::runThread, this);
        mThread.detach();
    }
    void join() {
        if (mThread.joinable()) {
            if ((mFd >= 0) && (mWatch >= 0)) {
                inotify_rm_watch(mFd, mWatch);
                close(mFd);
            }
            mThread.join();
            LOGD(LOG_TAG_CHECK_FILE_INFO) << std::string("CheckFileInfo : end");
        }
    }
    using RegisterChangedFileInfoCallback = std::function<void(const std::string& fileName)>;
    void registerChangedFileInfo(RegisterChangedFileInfoCallback pCallback) {
        mRegisterChangedFileInfoCallback = pCallback;
    }

private:
    void runThread() {
        std::string file = mCheckFile;
        if (file.size() == 0) {
            LOGE(LOG_TAG_CHECK_FILE_INFO) << "Fail to file : " << file;
            return;
        }

        mFd = inotify_init();
        if (mFd < 0) {
            LOGE(LOG_TAG_CHECK_FILE_INFO) << "Fail to init : " << mFd;
            return;
        }

        mWatch = inotify_add_watch(mFd, file.c_str(), IN_CLOSE_WRITE);
        if (mWatch < 0) {
            LOGE(LOG_TAG_CHECK_FILE_INFO) << "Fail to add watch : " << mWatch;
            return;
        }

        char buffer[mBufferSize] = {
            0,
        };
        while (true) {
            std::memset(buffer, 0, mBufferSize);
            if (read(mFd, buffer, mBufferSize) >= 0) {
                LOGI(LOG_TAG_CHECK_FILE_INFO) << "changed file : " << file;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                mRegisterChangedFileInfoCallback(file);
                break;
            }
        }

        join();
    }

private:
    RegisterChangedFileInfoCallback mRegisterChangedFileInfoCallback;
    std::thread mThread;
    std::string mCheckFile = std::string();
    ccos::HInt32 mFd = 0;
    ccos::HInt32 mWatch = 0;
    ccos::HInt32 mBufferSize = 0;
};

}  // end of namespace variantservice
}  // end of namespace sys
}  // end of namespace ccos

#endif
