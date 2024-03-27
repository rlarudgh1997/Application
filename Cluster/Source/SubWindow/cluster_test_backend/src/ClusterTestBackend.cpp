#include "ClusterTestBackend.h"

#define DOCKER_ENV false

using namespace cTest;

void printUsage() {
    std::cout << "\n------------------------------------------------------------------------------------------------"
              << std::endl;
    std::cout << std::fixed;
    std::cout.precision(1);
    std::cout << "Cluster test backend Release : v" << float(0.1) << std::endl << std::endl;
    std::cout.unsetf(std::ios::fixed);
    std::cout << "Usage                        : $ cluster_test_backend OPTIONS" << std::endl << std::endl;
    std::cout << "Available options            : " << std::endl;
    std::cout << "  -f FILE_NAME | set .tav file name (not a essential option: No default)" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
}

void printChange(std::string str) {
    std::cout << "----------------------------------Text Changes------------------------------------------------" << std::endl;
    std::cout << str;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl
              << std::endl;
}

void printChange2(std::string str) {
    std::cout << "----------------------------------Text Changes2------------------------------------------------" << std::endl;
    std::cout << str;
    std::cout << "------------------------------------------------------------------------------------------------" << std::endl
              << std::endl;
}

stringToVoidCbk getSetRunningCbk() {
    return std::bind(printChange, std::placeholders::_1);
}

stringToVoidCbk getSetRunningCbk2() {
    return std::bind(printChange2, std::placeholders::_1);
}

int main() {
    Logger::initialize("SFCTest", "test_backend");
    globalLogLevel = log_level::DEBUG_LOG;

    // printUsage();

    sigset_t sigMask;
    struct signalfd_siginfo sigFdInfo;
    int signalFD;

    sigemptyset(&sigMask);
    sigaddset(&sigMask, SIGTERM);
    sigaddset(&sigMask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &sigMask, NULL) == -1) {
        cError() << "sigprocmask error";
        return -1;
    }

    signalFD = signalfd(-1, &sigMask, 0);
    if (signalFD == -1) {
        cError() << "signalfd error";
        return -1;
    }

    // .tav Parser
    auto TAVParserPtr = std::make_unique<TAVParser>();

    if (TAVParserPtr->setTAVFile("../TAVFiles/tavExample.tav")) {
        cDebug() << "Parser Worked correctly.";
        if (globalLogLevel == log_level::DEBUG_LOG) {
            std::cout << std::endl;
        }
    }
    TAVParserPtr->printPreconditionSignals();
    TAVParserPtr->printStepSignals();
    TAVParserPtr->printExpectedResultSignals();
    std::cout << std::endl;

    // Altonservice
    auto AltonServiceRunnerPtr = std::make_unique<AltonServiceRunner>();
    AltonServiceRunnerPtr->run();

    // AltonClient
    auto AltonClientRunnerPtr = std::make_unique<AltonClientRunner>();
    AltonClientRunnerPtr->listen(TAVParserPtr->getExpectedResultSignals());

    // LogWatcher
    auto AltonServiceLogWatcherPtr = std::make_unique<LogWatcher>();
    auto AltonClientLogWatcherPtr = std::make_unique<LogWatcher>();

    AltonServiceLogWatcherPtr->registerPath("../AltonServiceLog/AltonServiceLog.txt");
    AltonClientLogWatcherPtr->registerPath("../AltonClientLog/AltonClientLog.txt");

    AltonServiceLogWatcherPtr->registerSendChangeCbk(getSetRunningCbk());
    AltonServiceLogWatcherPtr->registerSendChangeCbk(getSetRunningCbk2());
    AltonClientLogWatcherPtr->registerSendChangeCbk(getSetRunningCbk());
    AltonClientLogWatcherPtr->registerSendChangeCbk(getSetRunningCbk2());

    AltonServiceLogWatcherPtr->watch();
    AltonClientLogWatcherPtr->watch();

    while (1) {
        ssize_t receivedBytes = read(signalFD, &sigFdInfo, sizeof(struct signalfd_siginfo));
        if (receivedBytes != sizeof(struct signalfd_siginfo)) {
            std::cout << "\n\n\nsignalfd read error\nExiting..." << std::endl;
            break;
        }

        if (sigFdInfo.ssi_signo == SIGTERM) {
            std::cout << "\n\n\nReceived signal : SIGTERM\nExiting..." << std::endl;
            break;
        }

        if (sigFdInfo.ssi_signo == SIGINT) {
            std::cout << "\n\n\nReceived signal : SIGINT\nExiting..." << std::endl;
            break;
        }
    }

    AltonServiceLogWatcherPtr->stopWatch();
    AltonClientLogWatcherPtr->stopWatch();
    AltonServiceLogWatcherPtr->join();
    AltonClientLogWatcherPtr->join();

    return 0;
}