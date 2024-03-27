#pragma once

#ifndef ALTONSERVICERUNNER_H__
#define ALTONSERVICERUNNER_H__

#include <fstream>
#include <unistd.h>

#include "ClusterTestBackendLog.h"

using namespace cTest;

class AltonServiceRunner {
public:
    AltonServiceRunner();
    ~AltonServiceRunner();

    void run();
    void stopAll();
    bool isAltonServiceExist();

private:
    void setRunning(bool boolean) {
        mRunning = boolean;
    }

    bool mRunning;
};

#endif  // ALTONSERVICERUNNER_H__