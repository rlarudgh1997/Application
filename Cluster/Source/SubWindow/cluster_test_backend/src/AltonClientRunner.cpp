#include "AltonClientRunner.h"

AltonClientRunner::AltonClientRunner() : mListeningNow(false) {
    if (isAltonClientExist()) {
        cDebug() << "Alton_client already exist!";
        stopAll();
    }
}

AltonClientRunner::~AltonClientRunner() {
    if (isAltonClientExist()) {
        stopAll();
    }

    // need to check the return value of rm command
    if (std::system("rm -rf altonClientLog.txt") != 0) {
        cWarning() << "Fail to erase altonClientLog.txt file...";
    }
}

void AltonClientRunner::listen(std::vector<std::string> signals) {
    std::string tmp;
#if !DOCKER_ENV
    // tmp += "~/usr/bin/alton_client listen ";
    tmp += "../../../../../Alton/alton_client listen ";
#else
    tmp += "alton_client listen ";
#endif
    for (auto& signal : signals) {
        tmp += signal + " ";
    }
    tmp += "> ../AltonClientLog/AltonClientLog.txt &";

    if (std::system(tmp.data()) != 0) {
        cError() << "Alton_client execution failed...";
        setListeningNow(false);
        return;
    }
    sleep(0.5);

    if (!isAltonClientExist()) {
        cError() << "Alton_client listen failed...";
        setListeningNow(false);
        return;
    }
    cDebug() << "Alton_client listening now...";
    if (globalLogLevel == log_level::DEBUG_LOG) {
        std::cout << "Listening Signal list" << std::endl;
        for (auto& signal : signals) {
            std::cout << "- " << signal.substr(0, signal.find(" ")) << std::endl;
        }
        std::cout << std::endl;
    }
    setListeningNow(true);
}

void AltonClientRunner::stopAll() {
    if (mListeningNow == false) {
        cWarning() << "Alton_client is not running now...";
        return;
    }

    int sysRet = std::system("killall -9 alton_client");
    if (sysRet == 0) {
        cDebug() << "Alton_client shutdown";
        setListeningNow(false);
    } else if (sysRet == 256) {
        cWarning() << "Alton_client is not running now...";
        setListeningNow(false);
    } else {
        cError() << "killall -9 alton_client error...";
    }
}

bool AltonClientRunner::isAltonClientExist() {
    int sysRet = std::system("ps -e | grep alton_client > altonClientLog.txt");
    if (sysRet == 0) {
        std::string line;
        std::ifstream file("altonClientLog.txt");
        if (file.is_open()) {
            while (getline(file, line)) {
                if (line.find("alton_client") != std::string::npos) {
                    setListeningNow(true);
                    return true;
                }
            }
            file.close();
        } else {
            cError() << "Fail to open altonClientLog.txt file...";
        }
    } else if (sysRet == 256) {
        // cWarning() << "There is no alton client...";
    } else {
        cError() << "System command(ps -e | grep alton_client) Failed...";
    }
    setListeningNow(false);
    return false;
}

void setSignal(std::string signal) {
    std::string tmp;
#if !DOCKER_ENV
    // tmp += "~/usr/bin/alton_client set ";
    tmp += "../../../../../Alton/alton_client set ";
#else
    tmp += "alton_client set ";
#endif

    tmp += signal;

    if (std::system(tmp.data()) != 0) {
        cError() << "alton_client execution failed...";
        return;
    }
}

void injectSignal(std::string signal) {
    std::string tmp;
#if !DOCKER_ENV
    // tmp += "~/usr/bin/alton_client inject ";
    tmp += "../../../../../Alton/alton_client inject ";
#else
    tmp += "alton_client inject ";
#endif

    tmp += signal;

    if (std::system(tmp.data()) != 0) {
        cError() << "alton_client execution failed...";
        return;
    }
}

void setSignals(std::vector<std::string> signals) {
    for (auto& signal : signals) {
        setSignal(signal);
    }
}

void injectSignals(std::vector<std::string> signals) {
    for (auto& signal : signals) {
        injectSignal(signal);
    }
}