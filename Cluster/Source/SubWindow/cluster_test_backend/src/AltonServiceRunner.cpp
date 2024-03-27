#include "AltonServiceRunner.h"

AltonServiceRunner::AltonServiceRunner() : mRunning(false) {
    if (isAltonServiceExist()) {
        cDebug() << "Altonservice already exist!";
        stopAll();
    }
}

AltonServiceRunner::~AltonServiceRunner() {
    if (isAltonServiceExist()) {
        stopAll();
    }

    // need to check the return value of rm command
    if (std::system("rm -rf altonServiceLog.txt") != 0) {
        cWarning() << "Fail to erase altonServiceLog.txt file...";
    }
}

void AltonServiceRunner::run() {
#if !DOCKER_ENV
    // if (std::system("~/usr/bin/altonservice > ../AltonServiceLog/AltonServiceLog.txt &") != 0) {
    if (std::system("../../../../../Alton/altonservice > ../AltonServiceLog/AltonServiceLog.txt &") != 0) {
#else
    if (std::system("altonservice > ../AltonServiceLog/AltonServiceLog.txt &") != 0) {
#endif
        cError() << "Altonservice execution failed...";
        setRunning(false);
        return;
    }
    sleep(1);

    if (!isAltonServiceExist()) {
        cError() << "Altonservice run failed...";
        setRunning(false);
        return;
    }
    cDebug() << "Altonservice run...";
    if (globalLogLevel == log_level::DEBUG_LOG) {
        std::cout << std::endl;
    }
    setRunning(true);
}

void AltonServiceRunner::stopAll() {
    if (mRunning == false) {
        cWarning() << "Altonservice is not running now...";
        return;
    }

    int sysRet = std::system("killall -9 altonservice");
    if (sysRet == 0) {
        cDebug() << "Altonservice shutdown";
        setRunning(false);
    } else if (sysRet == 256) {
        cWarning() << "Altonservice is not running now...";
        setRunning(false);
    } else {
        cError() << "killall -9 altonservice error...";
    }
}

bool AltonServiceRunner::isAltonServiceExist() {
    int sysRet = std::system("ps -e | grep altonservice > altonServiceLog.txt");
    if (sysRet == 0) {
        std::string line;
        std::ifstream file("altonServiceLog.txt");
        if (file.is_open()) {
            while (getline(file, line)) {
                if (line.find("altonservice") != std::string::npos) {
                    setRunning(true);
                    return true;
                }
            }
            file.close();
        } else {
            cError() << "Fail to open altonServiceLog.txt file...";
        }
    } else if (sysRet == 256) {
        // cWarning() << "There is no altonservice...";
    } else {
        cError() << "System command(ps -e | grep altonservice) Failed...";
    }
    setRunning(false);
    return false;
}