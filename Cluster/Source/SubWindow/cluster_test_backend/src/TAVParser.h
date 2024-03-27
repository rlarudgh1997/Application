#pragma once

#ifndef TAVPARSER_H__
#define TAVPARSER_H__

#include <fstream>

#include "ClusterTestBackendLog.h"

using namespace cTest;

class TAVParser {
public:
    TAVParser();
    ~TAVParser();

    bool setTAVFile(const char* fileName);
    bool setTAVFile(std::string fileName);

    std::string getAnnotations();  // Should implement

    std::string getDescription();
    std::vector<std::string> getPreconditionSignals();
    std::vector<std::string> getStepSignals();
    std::vector<std::string> getExpectedResultSignals();

    void printPreconditionSignals();
    void printStepSignals();
    void printExpectedResultSignals();

    inline bool isFileSetted() {
        return mFileSetted;
    }

private:
    std::string eraseAnnotation(std::string line);
    std::string eraseWhiteSpace(std::string line);
    unsigned int countFrontSpace(std::string line);
    bool isKeyword(std::string& line, const char* keyword);
    bool parseTAVFile(const char* fileName);
    void printSignals(std::vector<std::string> signals, std::string name);

    bool mFileSetted;
    std::vector<std::string> mPreconditionSignals;
    std::vector<std::string> mStepSignals;
    std::vector<std::string> mExpectedResultSignals;
    std::string mDescription;
};

#endif  // TAVPARSER_H__