#pragma once

#ifndef ALTONCLIENTRUNNER_H__
#define ALTONCLIENTRUNNER_H__

#include <fstream>
#include <unistd.h>

#include "ClusterTestBackendLog.h"
// #include "SFCSignalDataGetter.h"

using namespace cTest;

class AltonClientRunner {
public:
    AltonClientRunner();
    ~AltonClientRunner();

    void listen(std::vector<std::string> signals);
    void stopAll();
    bool isAltonClientExist();

    void setSignal(std::string signal);
    void injectSignal(std::string signal);

    void setSignals(std::vector<std::string> signals);
    void injectSignals(std::vector<std::string> signals);

private:
    void setListeningNow(bool boolean) {
        mListeningNow = boolean;
    }

    bool mListeningNow;
};

#endif  // ALTONCLIENTRUNNER_H__