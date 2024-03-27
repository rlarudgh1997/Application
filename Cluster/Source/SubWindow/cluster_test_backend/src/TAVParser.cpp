#include "TAVParser.h"

#define GET_NAME(n) #n

TAVParser::TAVParser() : mFileSetted(false), mDescription("") {
}

TAVParser::~TAVParser() {
}

std::string TAVParser::eraseAnnotation(std::string line) {
    std::size_t idx = line.find("#");
    if (idx != std::string::npos) {
        return line.substr(0, idx);
    }
    return line;
}

std::string TAVParser::eraseWhiteSpace(std::string line) {
    while ((line.length() - 1 == static_cast<int>(line.rfind(" "))) && line != "") {
        line = line.substr(0, line.length() - 1);
    }
    return line;
}

unsigned int TAVParser::countFrontSpace(std::string line) {
    unsigned int cnt = 0;
    while (line.find(" ", cnt) == cnt) {
        cnt++;
    }
    return cnt;
}

bool TAVParser::isKeyword(std::string& line, const char* keyword) {
    return (line.find(keyword) != std::string::npos);
}

bool TAVParser::parseTAVFile(const char* fileName) {
    bool isDescriptionPart = false;
    bool isPreconditionPart = false;
    bool isStepPart = false;
    bool isExpectedResultPart = false;
    bool isGroup = false;
    int tapCnt = 1;
    std::string line;
    std::string signalGroup;
    std::ifstream file(fileName);

    if (file.is_open()) {
        while (getline(file, line)) {
            // Erase annotation
            line = eraseAnnotation(line);

            // Erase white space
            line = eraseWhiteSpace(line);

            if (line == "") {
                continue;
            }

            if (line.find("[") == 0) {
                if (isKeyword(line, "[Description]")) {
                    isDescriptionPart = true;
                    isPreconditionPart = false;
                    isStepPart = false;
                    isExpectedResultPart = false;
                    continue;
                }

                if (isKeyword(line, "[Precondition]")) {
                    isDescriptionPart = false;
                    isPreconditionPart = true;
                    isStepPart = false;
                    isExpectedResultPart = false;
                    continue;
                }

                if (isKeyword(line, "[Step]")) {
                    isDescriptionPart = false;
                    isPreconditionPart = false;
                    isStepPart = true;
                    isExpectedResultPart = false;
                    continue;
                }

                if (isKeyword(line, "[Expected Result]")) {
                    isDescriptionPart = false;
                    isPreconditionPart = false;
                    isStepPart = false;
                    isExpectedResultPart = true;
                    continue;
                }

                cWarning() << line << " is Not a Available [Keyword]...";
                isDescriptionPart = false;
                isPreconditionPart = false;
                isStepPart = false;
                isExpectedResultPart = false;
                continue;
            }

            if (isDescriptionPart) {
                mDescription += line;
                mDescription += "\n";
                continue;
            }

            if (isPreconditionPart && !isGroup) {
                // group example code (should be enhanced)
                if (isKeyword(line, "[Group]")) {
                    isGroup = true;
                    tapCnt = 2;
                    continue;
                }

                if (isGroup && countFrontSpace(line) == 4 * tapCnt) {
                    if (signalGroup.empty()) {
                        signalGroup += line.substr(4 * tapCnt);
                    } else {
                        signalGroup += " " + line.substr(4 * tapCnt);
                    }
                } else if (isGroup && countFrontSpace(line) != 4 * tapCnt) {
                    isGroup = false;
                    tapCnt = 1;
                    mPreconditionSignals.push_back(signalGroup);
                    mPreconditionSignals.push_back(line.substr(4 * tapCnt));
                } else {
                    mPreconditionSignals.push_back(line.substr(4 * tapCnt));
                }
                // group example code end (should be enhanced)

                continue;
            }

            if (isStepPart && !isGroup) {
                mStepSignals.push_back(line.substr(4 * tapCnt));
                continue;
            }

            if (isExpectedResultPart && !isGroup) {
                mExpectedResultSignals.push_back(line.substr(4 * tapCnt));
                continue;
            }
        }
        file.close();
    } else {
        cError() << "Fail to open " << fileName << " file...";
        return false;
    }
    return true;
}

bool TAVParser::setTAVFile(const char* fileName) {
    return parseTAVFile(fileName);
}

bool TAVParser::setTAVFile(std::string fileName) {
    return parseTAVFile(fileName.data());
}

std::string TAVParser::getDescription() {
    return mDescription;
}

std::vector<std::string> TAVParser::getPreconditionSignals() {
    return mPreconditionSignals;
}

std::vector<std::string> TAVParser::getStepSignals() {
    return mStepSignals;
}

std::vector<std::string> TAVParser::getExpectedResultSignals() {
    return mExpectedResultSignals;
}

void TAVParser::printSignals(std::vector<std::string> signals, std::string name) {
    int idx = 0;
    std::cout << std::endl;
    for (auto& signal : signals) {
        std::cout << name.substr(1) << "[" << idx << "]: " << signal << std::endl;
    }
}

void TAVParser::printPreconditionSignals() {
    printSignals(mPreconditionSignals, GET_NAME(mPreconditionSignals));
}

void TAVParser::printStepSignals() {
    printSignals(mStepSignals, GET_NAME(mStepSignals));
}

void TAVParser::printExpectedResultSignals() {
    printSignals(mExpectedResultSignals, GET_NAME(mExpectedResultSignals));
}