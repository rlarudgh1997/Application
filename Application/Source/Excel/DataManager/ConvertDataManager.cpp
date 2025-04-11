#include "ConvertDataManager.h"

#include "CommonEnum.h"
#include "CommonUtil.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"

#include "ExcelData.h"
#include "ExcelUtil.h"
#include "ExcelDataManager.h"
#include "SignalDataManager.h"
#include <QRegularExpression>
#include "JSEngineManager.h"

// #define ENABLE_CONFIG_DEBUG_LOG
// #define ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG
// #define ENABLE_MULTI_RESULT_SHEET_DEBUG_LOG
// #define ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG
// #define ENABLE_CONVERTING_KEYWORD_DATA_INFO
// #define ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG

const QString kEasterEggTrigger = "$EASTEREGG$";
const QString GEN_TYPE_NEGATIVE_AND_POSITIVE = QString("Negative/Positive");
const QString GEN_TYPE_POSITIVE = QString("Positive");
const QString SFC_IGN_ELAPSED = QString("SFC.Private.IGNElapsed.Elapsed");

QSharedPointer<ConvertDataManager>& ConvertDataManager::instance() {
    static QSharedPointer<ConvertDataManager> gManager;
    if (gManager.isNull()) {
        gManager = QSharedPointer<ConvertDataManager>(new ConvertDataManager());
    }
    return gManager;
}

ConvertDataManager::ConvertDataManager() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    setMergeStart(mergeStart);
    setMerge(merge);
    setMergeEnd(mergeEnd);
    setMergeInfos(QStringList({mergeStart, merge, mergeEnd}));
}

bool ConvertDataManager::excuteConvertDataManager() {
    ivis::common::CheckTimer checkTimer;

    // NOTE(csh): [Sheet] Keyword 해석 기능 수행(Data 추가) -> 나머지 Keyword 기능 수행(cell data changed) + 001 excel 파일 생성
    if (convertKeywordData() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QString filePath = ivis::common::APP_PWD() + "/Convert.excel_001";
            ExcelUtil::instance().data()->writeExcelSheet(filePath, true);
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_001");
    }

    // NOTE(csh): Config(동작 조건) Data 처리 (Data 추가) + config excel 파일 생성
    if (appendConvertConfigSignalSet() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QString filePath = ivis::common::APP_PWD() + "/Convert.excel_Config";
            ExcelUtil::instance().data()->writeExcelSheet(filePath, true);
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_Config");
    }

    // NOTE(csh): 최종 signal 조합 set 구성(Data 추가) + 002 excel 파일 생성
    if (appendConvertAllTCSignalSet() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QString filePath = ivis::common::APP_PWD() + "/Convert.excel_002";
            ExcelUtil::instance().data()->writeExcelSheet(filePath, true);
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_002");
    }

    return true;
}

bool ConvertDataManager::convertKeywordData() {
    bool result = false;

    // InputData, OutputSignal, OutputValue Keyword
    if (convertNonInputSignalKeyword() == true) {
        result = true;
    } else {
        qDebug() << "[convertNonInputSignalKeyword] Failed";
    }

    // [Preset], [Not_Trigger] process

    // InputSignal Keyword ([Sheet])
    //  - 2 depth이상 [Sheet] 처리
    //  - 최대 3 depth까지만 처리 -> 최대 depth count 변경 시에 Application.ini 파일의 ConfigTypeGenTCParsingCycle 수정
    const int maxSheetProccessingCount =
        ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenTCParsingCycle).toInt();
    int sheetKeywordProcessingCount = 0;
    int testResult = 0;
    do {
        result = true;
        sheetKeywordProcessingCount++;
        // TODO(csh): preset, not_trigger, dependent_on 키워드를 별도로 처리하는 경우 하기 함수명을 sheet 처리 함수로 변경 필요
        testResult = convertInputSignalSheetKeyword();
        if (testResult == -1) {
            result = false;
        }
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
        qDebug() << " >> process count/max  (" << sheetKeywordProcessingCount << "/" << maxSheetProccessingCount << ")";
        qDebug() << " >> result (-1: fail / 0 : pass / 1 : sheet keyword processing)";
        qDebug() << " >> result : " << testResult;
#endif
    } while ((sheetKeywordProcessingCount < maxSheetProccessingCount) && (testResult == 1));

    return result;
}

#define NO_DUPLICATED_SIGNAL -1
bool ConvertDataManager::appendConvertConfigSignalSet() {
    bool result = false;
    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) + 1;
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);

    ExcelDataManager::instance().data()->reloadExcelData();

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs))) {
            qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }

        QStringList tcNameList = ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, true);
#if defined(ENABLE_CONFIG_DEBUG_LOG)
        qDebug() << "============================[appendConvertConfigSignalSet]=====================================";
        qDebug() << "Sheet Index     : " << sheetIndex;
        qDebug() << "TCName List     : " << tcNameList;
#endif
        // Sheet에서 TCName 리스트 기반으로 Result/Case 하위 Data 처리
        for (int tcIdx = 0; tcIdx < tcNameList.size(); ++tcIdx) {
            QString tcNameStr = tcNameList.at(tcIdx);
            QStringList resultStrList = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcNameStr);
            // get - config 관련 Signal List set
            QString configStr = ExcelDataManager::instance().data()->isConfigData(sheetIndex, tcNameStr);
            QList<QList<QStringList>> configDataInfoList = constructConvertConfigSignalSet(constructMergeKeywords("", configStr));
#if defined(ENABLE_CONFIG_DEBUG_LOG)
            qDebug() << "TCName          : " << tcNameStr;
            qDebug() << "Config          : " << configStr << ", Data : " << configDataInfoList;
            qDebug() << "Result List     : " << resultStrList;
            qDebug() << "############################################################################################";
#endif
            // TCName 하위의 Result 리스트 기반으로 Case Data 처리
            for (int resultIdx = 0; resultIdx < resultStrList.size(); ++resultIdx) {
                QString resultStr = resultStrList.at(resultIdx);
                QStringList caseStrList = ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcNameStr, resultStr);

#if defined(ENABLE_CONFIG_DEBUG_LOG)
                qDebug() << "Result          : " << resultStr;
#endif
                // config data set 기준으로 Result 배수 증가 (OR 기준 : A or B or C -> Case1_A, Case1_B, Case1_C ... )
                for (int caseIdx = 0; caseIdx < caseStrList.size(); ++caseIdx) {
                    QString caseStr = caseStrList.at(caseIdx);
                    QPair<QStringList, QStringList> caseInputDataList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcNameStr, resultStr, caseStr, false);

                    // config 동작 조건이 없는 경우
                    if (configDataInfoList.size() == 0) {
                        if (caseInputDataList.first.isEmpty() == true && caseInputDataList.second.isEmpty() == true) {
                            // others 인 경우 (input signal/data가 존재하지 않기 때문에, config signal/data append set 수행 X)
                            caseInputDataList.first.append("");
                            caseInputDataList.second.append("");
                        }
                        ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, tcNameStr, resultStr, caseStr,
                                                                                 caseInputDataList);
#if defined(ENABLE_CONFIG_DEBUG_LOG)
                        qDebug() << "No Config Data Exist Condition";
#endif
                        continue;
                    }

                    // config 동작 조건이 있는 경우
                    for (int configSetIndex = 0; configSetIndex < configDataInfoList.size(); ++configSetIndex) {
                        QPair<QStringList, QStringList> inputDataPairList = caseInputDataList;
#if defined(ENABLE_CONFIG_DEBUG_LOG)
                        qDebug() << "---------------------------------- [Case] ----------------------------------";
                        qDebug() << "Case            : " << caseStr;
                        qDebug() << "InputData(sig)  : " << inputDataPairList.first;
                        qDebug() << "InputData(val)  : " << inputDataPairList.second;
#endif
                        // other 예외 처리 조건 (other에는 config data set append (X))
                        if (inputDataPairList.first.isEmpty() == false && inputDataPairList.second.isEmpty() == false) {
                            QList<QStringList> tmpConfigDataSet = configDataInfoList.at(configSetIndex);
                            QString customConfigKeywordStr = ExcelUtil::instance().data()->isKeywordString(
                                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomConfig));

                            for (int idx = 0; idx < tmpConfigDataSet.size(); ++idx) {
#if defined(ENABLE_CONFIG_DEBUG_LOG)
                                qDebug() << "[AND Case] Merged Config Data Set for Insert : " << tmpConfigDataSet.at(idx);
#endif
                                QString inputSignalName =
                                    tmpConfigDataSet[idx][static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)];

                                QString inputSignalData =
                                    QString("%1%2")
                                        .arg(customConfigKeywordStr)
                                        .arg(tmpConfigDataSet[idx]
                                                             [static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)]);

                                // TODO(csh): Error Type function 추가(arg: enum type / QStringList infoData)
                                // Config의 SignalData Pair와 Sheet의 InputSignalData Pair에 중복되는 pair가 존재할 경우
                                // InputData를 우선적으로 사용하기 위한 로직
                                // Data Format : [CustomConfig]+InputData
                                int findSignalIndex = inputDataPairList.first.indexOf(inputSignalName);

                                if (findSignalIndex == NO_DUPLICATED_SIGNAL) {  // config 와 input signal 간 중복 항목 X 조건
                                    inputDataPairList.first.append(inputSignalName);   // InputSignal - StringList
                                    inputDataPairList.second.append(inputSignalData);  // InputData - StringList
                                }
                                // (Config OR 조건인 경우) input signal/data에서 Config(동작조건)의 signal/data가 이미
                                // 사용되어지는 경우, 해당 사용 signal도 [CustomConfig] 키워드 추가 필요
                                for (int configIdx = 0; configIdx < configDataInfoList.size(); ++configIdx) {
                                    QList<QStringList> configDataSet = configDataInfoList.at(configIdx);
                                    for (int i = 0; i < configDataSet.size(); ++i) {
                                        QString configSignalName =
                                            configDataSet[i][static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)];

                                        int findConfigSignalIndex = inputDataPairList.first.indexOf(configSignalName);
                                        if (findConfigSignalIndex != NO_DUPLICATED_SIGNAL) {  // config와 input signal 중복 조건
                                            if (inputDataPairList.second.size() > findConfigSignalIndex) {
                                                QString tmpConfigDataInInputData =
                                                    inputDataPairList.second[findConfigSignalIndex];
                                                // [CustomConfig] 키워드가 없는 경우에만 추가
                                                if (tmpConfigDataInInputData.contains(customConfigKeywordStr) == false) {
                                                    inputDataPairList.second[findConfigSignalIndex] =
                                                        QString("%1%2").arg(customConfigKeywordStr).arg(tmpConfigDataInInputData);
                                                }
                                            }
                                        }
                                    }
                                }
                            }

#if defined(ENABLE_CONFIG_DEBUG_LOG)
                            qDebug() << "InputData(sig) after append : " << inputDataPairList.first;
                            qDebug() << "InputData(val) after append : " << inputDataPairList.second;
#endif
                            ExcelDataManager::instance().data()->updateInputDataInfo(
                                sheetIndex, tcNameStr, resultStr,
                                QString("%1 %2_%3").arg(caseStr).arg("config").arg(QString::number(configSetIndex)),
                                inputDataPairList);
                        } else {
                            // others 인 경우 (input signal/data가 존재하지 않기 때문에, config signal/data append set 수행 X)
                            inputDataPairList.first.append("");
                            inputDataPairList.second.append("");
                            ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, tcNameStr, resultStr, caseStr,
                                                                                     inputDataPairList);
#if defined(ENABLE_CONFIG_DEBUG_LOG)
                            qDebug() << "others case (not operated input signal/data)";
#endif
                            break;
                        }
                    }
                    // config 동작 조건이 한개 이상인 경우 (Config excel 생성)
                    result = true;
#if defined(ENABLE_CONFIG_DEBUG_LOG)
                    qDebug() << "Config Data Exist : " << configDataInfoList;
#endif
                }
#if defined(ENABLE_CONFIG_DEBUG_LOG)
                qDebug() << "############################################################################################";
#endif
            }
        }
        if (result == true) {
            ExcelDataManager::instance().data()->writeExcelSheetData(sheetIndex);
        }
    }

    return result;
}

bool ConvertDataManager::appendConvertAllTCSignalSet() {
    bool result = true;

    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) + 1;
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);

    ExcelDataManager::instance().data()->reloadExcelData();

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs))) {
            qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }

        QStringList tcNameList = ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, true);
#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
        qDebug() << "============================[appendConvertAllTCSignalSet]=====================================";
        qDebug() << "Sheet Index     : " << sheetIndex;
#endif
        // Sheet에서 TCName 리스트 기반으로 Result/Case 하위 Data 처리
        for (int tcIdx = 0; tcIdx < tcNameList.size(); ++tcIdx) {
            QString tcNameStr = tcNameList.at(tcIdx);
            QStringList resultStrList = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcNameStr);
#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
            qDebug() << "TCName          : " << tcNameStr;
            qDebug() << "############################################################################################";
#endif
            // TCName 하위의 Result 리스트 기반으로 Case Data 처리
            for (int resultIdx = 0; resultIdx < resultStrList.size(); ++resultIdx) {
                QString resultStr = resultStrList.at(resultIdx);
                QStringList caseStrList = ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcNameStr, resultStr);
#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
                qDebug() << "Result          : " << resultStr;
                qDebug() << "caseStrList : " << caseStrList;
#endif
                for (int caseIdx = 0; caseIdx < caseStrList.size(); ++caseIdx) {
                    QString caseStr = caseStrList.at(caseIdx);

                    auto inputList = ExcelDataManager::instance().data()->isInputDataWithoutCaseList(sheetIndex, tcNameStr,
                                                                                                     resultStr, caseStr);
                    auto appendInputSignalDataInfoMap =
                        SignalDataManager::instance().data()->isNormalInputSignalDataInfo(inputList);

                    QPair<QStringList, QStringList> inputDataList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcNameStr, resultStr, caseStr, false);

                    // others 인 경우 (input signal/data가 존재하지 않기 때문에, appendAllTCSignalset 수행 X)
                    if (inputDataList.first.isEmpty() == true && inputDataList.second.isEmpty() == true) {
                        inputDataList.first.append("");
                        inputDataList.second.append("");
                        ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, tcNameStr, resultStr, caseStr,
                                                                                 inputDataList);
                        break;
                    }
#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
                    qDebug() << "--------------------------------------------------------------------------------------------";
                    qDebug() << "1) Case            : " << caseStr;
                    qDebug() << "2) InputData(sig)  : " << inputDataList.first;
                    qDebug() << "3) InputData(val)  : " << inputDataList.second;
#endif
                    for (const auto& mapKey : appendInputSignalDataInfoMap.keys()) {
                        QString inputDataStr = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger));

                        QString inputSignalStr = appendInputSignalDataInfoMap[mapKey].first;
#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
                        qDebug() << "--------------------------------------------------------";
                        qDebug() << " - inputSignalStr : " << inputSignalStr;
                        qDebug() << " - inputDataStr   : " << inputDataStr;
#endif
                        SignalData tmpInfo = appendInputSignalDataInfoMap[mapKey].second;
                        QString keywordStr = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined));

                        if (tmpInfo.getDataType() == static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid)) {
                            QStringList tmpOriginData = tmpInfo.getOriginData();

                            if (tmpOriginData.size() > 1) {
                                inputDataStr = tmpOriginData.join(", ");
#if 0
                                QString keywordStr1 = ExcelUtil::instance().data()->isKeywordString(tmpInfo.isKeywordType());
                                inputDataStr.replace(keywordStr1, keywordStr);
#endif
                                inputDataStr = keywordStr + inputDataStr;
                            } else if (tmpOriginData.size() == 1) {
                                inputDataStr.clear();
                                inputDataStr =
                                    QString("%1[%2], [%3]").arg(keywordStr).arg(tmpOriginData.at(0)).arg(tmpOriginData.at(0));
                            } else {
                                // no operation
                            }
                        } else if (inputSignalStr.compare(SFC_IGN_ELAPSED) == 0) {
                            // [Not_Trigger] SFC.Private.IGNElapsed.Elapsed
                            // no operation
                        } else if ((tmpInfo.getValueEnum().isEmpty() == false) && (tmpInfo.getNotUsedEnum().isEmpty() == false)) {
                            // Enum Value
                            inputDataStr += tmpInfo.getNotUsedEnum().join(", ");
                        } else if ((tmpInfo.getValueEnum().isEmpty() == true) && (tmpInfo.getNotUsedEnum().isEmpty() == false)) {
                            // Not Enum Value
                            inputDataStr += tmpInfo.getNotUsedEnum().join(", ");
                        } else if (tmpInfo.getValueEnum().isEmpty() == true &&
                                   tmpInfo.getDataType() == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) {
                            if (tmpInfo.getKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver) &&
                                tmpInfo.getKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder) &&
                                tmpInfo.getKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual) &&
                                tmpInfo.getKeywordType() !=
                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual)) {
                                uint64_t maxValue = static_cast<uint64_t>(UINT32_MAX) + 1;
                                inputDataStr += QString::number(maxValue);
                            } else {
                                QStringList tmpOriginData = tmpInfo.getOriginData();
                                if (tmpOriginData.size() > 1) {
                                    inputDataStr = tmpOriginData.join(", ");
                                    QString keywordStr1 = ExcelUtil::instance().data()->isKeywordString(tmpInfo.getKeywordType());
                                    inputDataStr.replace(keywordStr1, keywordStr);
                                }
                            }
                        } else {
                            // no operation
                        }
                        inputDataList.first.append(inputSignalStr);
                        inputDataList.second.append(inputDataStr);
                    }
                    ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, tcNameStr, resultStr, caseStr,
                                                                             inputDataList);
#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
                    qDebug() << "InputData(sig) after append : " << inputDataList.first;
                    qDebug() << "InputData(val) after append : " << inputDataList.second;
#endif
                }

#if 0  // updateOutputDataInfo 테스트 코드
                QList<QStringList> outputList;
                for (int index = 0; index < 10; ++index) {
                    QStringList output({
                        QString("Singal_%1").arg(index),
                        QString("%1").arg(((index % 3) == 0)? ("O") : ("")),
                        QString("Data_%1").arg(index),
                    });
                    outputList.append(output);
                }
                ExcelDataManager::instance().data()->updateOutputDataInfo(sheetIndex, tcNameStr, resultStr, outputList);
#endif

#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
                qDebug() << "############################################################################################";
#endif
            }
        }
        ExcelDataManager::instance().data()->writeExcelSheetData(sheetIndex);
    }

    return result;
}

bool ConvertDataManager::convertInputSignalSheetKeyword() {
    // return value description
    // -1 : Unexpected Result
    //  0 : Success Result
    //  1 : [Sheet] Keyword Exist
    int result = -1;

    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription);
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);

    ExcelDataManager::instance().data()->reloadExcelData();

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs))) {
            qDebug() << "[convertInputSignalKeyword] Not support sheet :" << sheetIndex;
            continue;
        }

        // List => TCName, ResultName, CaseName, <InputSignalList, InputDataList>
        QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>> backupCurSheetIndexData;
        QStringList tcNameList = ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, true);
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
        qDebug() << "============================[convertInputSignalKeyword]=====================================";
        qDebug() << "Sheet Index     : " << sheetIndex;
        qDebug() << "TCName List     : " << tcNameList;
#endif
        // Sheet에서 TCName 리스트 기반으로 Result/Case 하위 Data 처리
        for (int tcIdx = 0; tcIdx < tcNameList.size(); ++tcIdx) {
            QString tcNameStr = tcNameList.at(tcIdx);
            QStringList resultStrList = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcNameStr);
            QString genType;
            ExcelDataManager::instance().data()->isGenTypeData(sheetIndex, tcNameStr, genType);
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
            qDebug() << "TCName          : " << tcNameStr;
            qDebug() << "Result List     : " << resultStrList;
            qDebug() << "############################################################################################";
#endif
            // TCName 하위의 Result 리스트 기반으로 Case Data 처리
            for (int resultIdx = 0; resultIdx < resultStrList.size(); ++resultIdx) {
                QString resultStr = resultStrList.at(resultIdx);
                QList<QStringList> curOutputDataInfoList =
                    ExcelDataManager::instance().data()->isOutputDataList(sheetIndex, tcNameStr, resultStr);
                QStringList caseStrList = ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcNameStr, resultStr);
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG) || defined(ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG)
                qDebug() << "tcNameStr       : " << tcNameStr;
                qDebug() << "Result          : " << resultStr;
                qDebug() << "Case List       : " << caseStrList;
#endif
#if defined(ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG)
                qDebug() << "OutputData      : " << curOutputDataInfoList;
#endif
                for (int caseIdx = 0; caseIdx < caseStrList.size(); ++caseIdx) {
                    QString caseStr = caseStrList.at(caseIdx);
                    QPair<QStringList, QStringList> caseInputDataList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcNameStr, resultStr, caseStr, false);
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
                    qDebug() << "------------------------------------------------------------------------------------------";
                    qDebug() << "Case            : " << caseStr;
                    qDebug() << "InputData(sig)  : " << caseInputDataList.first;
                    qDebug() << "InputData(val)  : " << caseInputDataList.second;
#endif
                    QStringList inputSignalStringList = caseInputDataList.first;

                    QString sheetKeywordStr = ExcelUtil::instance().data()->isKeywordString(
                        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet));
                    QString notTriggerKeywordStr = ExcelUtil::instance().data()->isKeywordString(
                        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger));
                    QString presetKeywordStr = ExcelUtil::instance().data()->isKeywordString(
                        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Preset));

                    // Case 하위의 input signal list 중에 [Sheet] 키워드가 있는지 확인하는 방법
                    bool hasSheetKeyword = !inputSignalStringList.filter(sheetKeywordStr).isEmpty();
                    if (hasSheetKeyword == true) {
                        auto sheetKeywordResultInfo = convertSheetKeyword(caseInputDataList, genType);
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
                        qDebug() << "[[[Result]]] sheetKeywordResultInfo : " << sheetKeywordResultInfo;
#endif
                        QString tmpCaseName = caseStr;

                        for (int i = 0; i < sheetKeywordResultInfo.size(); ++i) {
                            QString sheetCaseName = sheetKeywordResultInfo.at(i).first;
                            QPair<QStringList, QStringList> sheetInputPairData = sheetKeywordResultInfo.at(i).second;
                            QString caseNameStr = mergeCaseName(tmpCaseName, sheetCaseName);
                            bool hasSheetKeywordInInterpretList = !sheetInputPairData.first.filter(sheetKeywordStr).isEmpty();
                            if (hasSheetKeywordInInterpretList == true) {
                                result = 1;
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
                                qDebug() << ":::: tcNameStr : " << tcNameStr;
                                qDebug() << ":::: resultStr : " << resultStr;
                                qDebug() << ":::: caseNameStr : " << caseNameStr;
                                qDebug() << ":::: sheetInputPairData : " << sheetInputPairData;
#endif
                            }

                            // appendCurSheetData(tcNameStr, resultStr, caseNameStr, sheetInputPairData, backupCurSheetIndexData);
                            ExcelDataManager::instance().data()->updateInputDataInfo(
                                sheetIndex, tcNameStr, deleteEasterEggKeyword(resultStr), caseNameStr, sheetInputPairData);
                        }
                    }
                    bool hasNotTriggerKeyword = !inputSignalStringList.filter(notTriggerKeywordStr).isEmpty();
                    // TODO(csh): TBD
                    bool hasPresetKeyword = !inputSignalStringList.filter(presetKeywordStr).isEmpty();
                    // TODO(csh): TBD

                    if (hasSheetKeyword == false && hasNotTriggerKeyword == false && hasPresetKeyword == false) {
                        // appendCurSheetData(tcNameStr, resultStr, caseStr, caseInputDataList, backupCurSheetIndexData);
                        ExcelDataManager::instance().data()->updateInputDataInfo(
                            sheetIndex, tcNameStr, deleteEasterEggKeyword(resultStr), caseStr, caseInputDataList);
                    }
                    if (result == -1) {
                        result = 0;
                    }
                }
                QList<QStringList> outputDataInfoList;
                // if () { ...
                // output 처리 조건이 있는 경우
                // outputDataInfoList << (QStringList() << "Data1" << "Data2" << "Data3");
                // else {
                // 기존 output 저장
                outputDataInfoList = curOutputDataInfoList;
                ExcelDataManager::instance().data()->updateOutputDataInfo(sheetIndex, tcNameStr, resultStr, outputDataInfoList);
            }
        }

        if (result != -1) {
            // updateCurSheetData(sheetIndex, backupCurSheetIndexData);
            ExcelDataManager::instance().data()->writeExcelSheetData(sheetIndex);
        }
    }

    return result;
}

bool ConvertDataManager::convertNonInputSignalKeyword() {
    bool result = false;

    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription);
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);

    ExcelDataManager::instance().data()->reloadExcelData();

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs))) {
            qDebug() << "[convertNonInputSignalKeyword] Not support sheet :" << sheetIndex;
            continue;
        }

        QStringList tcNameList = ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, true);
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
        qDebug() << "============================[convertNonInputSignalKeyword]=====================================";
        qDebug() << "Sheet Index     : " << sheetIndex;
        qDebug() << "TCName List     : " << tcNameList;
#endif
        // Sheet에서 TCName 리스트 기반으로 Result/Case 하위 Data 처리
        for (int tcIdx = 0; tcIdx < tcNameList.size(); ++tcIdx) {
            QString tcNameStr = tcNameList.at(tcIdx);
            QStringList resultStrList = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcNameStr);
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "TCName          : " << tcNameStr;
            qDebug() << "Result List     : " << resultStrList;
            qDebug() << "############################################################################################";
#endif
            // TCName 하위의 Result 리스트 기반으로 Case Data 처리
            for (int resultIdx = 0; resultIdx < resultStrList.size(); ++resultIdx) {
                QString resultStr = resultStrList.at(resultIdx);
                QStringList caseStrList = ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcNameStr, resultStr);
                QList<QStringList> curOutputDataInfoList =
                    ExcelDataManager::instance().data()->isOutputDataList(sheetIndex, tcNameStr, resultStr);

                ResultInfo resultDataInfo;
                resultDataInfo.resultName = resultStr;

#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                qDebug() << "tcNameStr       : " << tcNameStr;
                qDebug() << "Result          : " << resultStr;
                qDebug() << "Case List       : " << caseStrList;
#endif
                for (int caseIdx = 0; caseIdx < caseStrList.size(); ++caseIdx) {
                    QString caseStr = caseStrList.at(caseIdx);
                    QPair<QStringList, QStringList> caseInputDataList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcNameStr, resultStr, caseStr, false);

                    CaseDataInfo caseDataInfo;
                    caseDataInfo.caseName = caseStr;

#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                    qDebug() << "------------------------------------------------------------------------------------------";
                    qDebug() << "Case            : " << caseStr;
                    qDebug() << "InputData(sig)  : " << caseInputDataList.first;
                    qDebug() << "InputData(val)  : " << caseInputDataList.second;
#endif
                    QStringList inputSignalList = caseInputDataList.first;
                    QStringList inputDataList = caseInputDataList.second;

                    if ((inputSignalList.isEmpty() == false && inputDataList.isEmpty() == false) &&
                        (inputSignalList.size() == inputDataList.size())) {
                        QList<ConvertKeywordInfo> inputDataInfoForOutput;

                        for (int idx = 0; idx < inputSignalList.size(); ++idx) {
                            QString inputSignal = inputSignalList.at(idx);
                            QString inputData = inputDataList.at(idx);

#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
                            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< [Info] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
                            qDebug() << "- Input signal string : " << inputSignal;
                            qDebug() << "- Input data   string : " << inputData;
#endif
                            if (idx == 0 && inputSignal.isEmpty() == true && inputData.isEmpty() == true) {
                                // other case check
                                //   - Other other Others others oTher oTHers 만 others로 인식
                                //   - another mother brother This is no others. 와 같은 문구의 other는 description word로 인식
                                caseStr.replace(QRegularExpression("(?i)^\\s*other[s]?\\s*$"),
                                                ExcelUtil::instance().data()->isKeywordString(
                                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)));
                                caseDataInfo.caseName = caseStr;
                            } else {
                                // process inputData keyword in InputSignal & InputData Column
                                ConvertKeywordInfo convertKeywordInfo = interpretInputValueKeyword(inputSignal, inputData);
                                inputDataInfoForOutput.append(convertKeywordInfo);
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
                                qDebug() << " - Convert Keyword Type : " << static_cast<int>(convertKeywordInfo.keywordType);
#endif
                            }

                            result = true;
                        }
                        caseDataInfo.convertInputDataInfo = inputDataInfoForOutput;
                    }
                    resultDataInfo.caseDataInfoList.append(caseDataInfo);
                }
                QList<OutputDataInfo> tmpOutputDataInfoList = convertOutputStructData(curOutputDataInfoList);
                resultDataInfo.outputDataInfoList.append(tmpOutputDataInfoList);
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
                displayResultDataInfo(resultDataInfo);
#endif
// process [Cal] keyword in OutputValue Column
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
                qInfo() << "----------------------------------------------------------------------------------";
                qInfo() << "Result Name: " << resultDataInfo.resultName;
                for (const auto& caseDataInfo : resultDataInfo.caseDataInfoList) {
                    qInfo() << "  Case Name: " << caseDataInfo.caseName;
                    for (const auto& keywordInfo : caseDataInfo.convertInputDataInfo) {
                        if (keywordInfo.inputSignal.isEmpty() || keywordInfo.inputValue.isEmpty()) {
                            qInfo() << "    WARNING: Input Signal or Input Value is empty!";
                        }
                        qInfo() << "    Input Signal: " << keywordInfo.inputSignal;
                        qInfo() << "    Input Value: " << keywordInfo.inputValue;
                        qInfo() << "    Valid Input Data: " << keywordInfo.validInputData;
                    }
                }
                for (const auto& outputDataInfo : resultDataInfo.outputDataInfoList) {
                    qInfo() << "  Output Signal: " << outputDataInfo.outputSignal;
                    qInfo() << "  Initialize: " << outputDataInfo.initialize;
                    qInfo() << "  Output Value: " << outputDataInfo.outputValue;
                }
                qInfo() << "----------------------------------------------------------------------------------";
#endif
                QList<ResultInfo> resultList = mergeAndCleanResultList(interpretCalKeywordAndRedefineResultInfo(resultDataInfo));
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
                qInfo() << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
                for (const auto& resultInfo : resultList) {
                    qInfo() << "Result Name: " << resultInfo.resultName;
                    for (const auto& caseDataInfo : resultInfo.caseDataInfoList) {
                        qInfo() << "  Case Name: " << caseDataInfo.caseName;
                        for (const auto& keywordInfo : caseDataInfo.convertInputDataInfo) {
                            if (keywordInfo.inputSignal.isEmpty() || keywordInfo.inputValue.isEmpty()) {
                                qInfo() << "    WARNING: Input Signal or Input Value is empty!";
                            }
                            qInfo() << "    Input Signal: " << keywordInfo.inputSignal;
                            qInfo() << "    Input Value: " << keywordInfo.inputValue;
                            qInfo() << "    Valid Input Data: " << keywordInfo.validInputData;
                        }
                    }
                    for (const auto& outputDataInfo : resultInfo.outputDataInfoList) {
                        qInfo() << "  Output Signal: " << outputDataInfo.outputSignal;
                        qInfo() << "  Initialize: " << outputDataInfo.initialize;
                        qInfo() << "  Output Value: " << outputDataInfo.outputValue;
                    }
                }
                qInfo() << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
#endif
                // InputSignal/InputData + [Cal] Keyword 해석 Data를 최종 Convert Data로 update
                updateResultDataInfo(sheetIndex, tcNameStr, resultList);
            }
        }

        if (result == true) {
            ExcelDataManager::instance().data()->writeExcelSheetData(sheetIndex);
        }
    }

    return result;
}

bool ConvertDataManager::convertOutputDataKeyword() {
    bool result = false;

    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax);

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        int convertSheetIndex = sheetIndex -
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) +
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription);
        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs))) {
            qDebug() << "[convertOutputDataKeyword] Not support sheet :" << sheetIndex;
            continue;
        }

        QStringList tcNameList = ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, true);
#if defined(ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG)
        qDebug() << "============================[convertOutputDataKeyword]=====================================";
        qDebug() << "Sheet Index     : " << sheetIndex;
        qDebug() << "TCName List     : " << tcNameList;
#endif
        // Sheet에서 TCName 리스트 기반으로 Result/Case 하위 Data 처리
        for (int tcIdx = 0; tcIdx < tcNameList.size(); ++tcIdx) {
            QString tcNameStr = tcNameList.at(tcIdx);
            QStringList resultStrList = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcNameStr);
#if defined(ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG)
            qDebug() << "TCName          : " << tcNameStr;
            qDebug() << "Result List     : " << resultStrList;
            qDebug() << "############################################################################################";
#endif
            // TCName 하위의 Result 리스트 기반으로 Case Data 처리
            for (int resultIdx = 0; resultIdx < resultStrList.size(); ++resultIdx) {
                QString resultStr = resultStrList.at(resultIdx);
                QList<QStringList> curOutputDataInfoList =
                    ExcelDataManager::instance().data()->isOutputDataList(sheetIndex, tcNameStr, resultStr);
#if defined(ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG)
                qDebug() << "tcNameStr       : " << tcNameStr;
                qDebug() << "Result          : " << resultStr;
                qDebug() << "OutputData      : " << curOutputDataInfoList;
#endif
            }
        }
    }

    return result;
}

QString ConvertDataManager::convertCustomKeywordType(const int& keywordType) {
    QString customKeywordType;

    switch (keywordType) {
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over): {
            customKeywordType = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver));
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
            qDebug() << "[[[Over]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under): {
            customKeywordType = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder));
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
            qDebug() << "[[[Under]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::MoreThanEqual): {
            customKeywordType = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual));
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
            qDebug() << "[[[MoreThanEqual]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::LessThanEqual): {
            customKeywordType = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual));
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
            qDebug() << "[[[LessThanEqual]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range): {
            customKeywordType = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange));
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
            qDebug() << "[[[Range]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay): {
            customKeywordType = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow));
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
            qDebug() << "[[[Flow]]]";
#endif
            break;
        }
        default: {
            // no operation
            break;
        }
    }

    return customKeywordType;
}

QString ConvertDataManager::constructConvertKeywordDataInfo(const int& keywordType, const QString& inputData) {
    QString retInputData;
    QString tmpInputData = inputData;
    QStringList splitInputDataList = tmpInputData.remove(" ").split(",");
    QString customKeyword = convertCustomKeywordType(keywordType);
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
    qDebug() << "====================== [constructConvertKeywordDataInfo] ======================";
#endif
    if (customKeyword.isEmpty() == false) {
        QStringList preconditionValueList;
        QStringList inputValueList;
        for (int index = 0; index < splitInputDataList.size(); index++) {
            if (index % 2 == 0) {
                preconditionValueList.append(splitInputDataList.at(index));
            } else {
                inputValueList.append(splitInputDataList.at(index));
            }
        }
        if ((preconditionValueList.size() == 1) && (preconditionValueList.size() < inputValueList.size())) {
            preconditionValueList.fill(preconditionValueList[0], inputValueList.size());
        }
        retInputData =
            QString("%1[%2], [%3]").arg(customKeyword).arg(preconditionValueList.join(", ")).arg(inputValueList.join(", "));
    } else {
        retInputData = inputData;
    }
#if defined(ENABLE_CONVERTING_KEYWORD_DATA_INFO)
    qDebug() << "1. Keyword Type    : " << keywordType;
    qDebug() << "2. Keyword Data    : " << inputData;
    qDebug() << "3. Converted Value : " << retInputData;
#endif
    return retInputData;
}

QList<QList<QStringList>> ConvertDataManager::constructConvertConfigSignalSet(const QString& configName) {
    const int columnDataMaxSize = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
    /*****************************************************************
     * [Config Data Data Container return Format]
     *  QList(
     * 	  QList(
     * 		QStringList("", "", "", "", "", "Vehicle.System.Config.Inter_ConfigVehicleType", "ICV, EV", "", "", "", "", "", ""),
     * 		QStringList("", "", "", "", "", "Vehicle.System.Config.Inter_ConfigDriveModeOption", "ON", "", "", "", "", "", "")
     * 	  ),
     * 	  QList(
     * 		QStringList("", "", "", "", "", "Vehicle.System.Config.Inter_ConfigECALL", "ON", "", "", "", "", "", "")
     * 	  )
     *  )
     ******************************************************************/

    QList<QList<QStringList>> retConfigList;
    QList<QStringList> convertConfigDataList;
    QList<QStringList> tmpConfifDataList = ExcelDataManager::instance().data()->isConfigDataList(configName);

    for (const QStringList& configValueStrList : tmpConfifDataList) {
        QStringList andSignalList(columnDataMaxSize, "");
        QStringList orSignalList(columnDataMaxSize, "");
        QString getConfigName = configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
        // Default Config Name은 Config Column에 빈 칸도 허용하기 때문에 예외 case
        if ((configName.compare(constructMergeKeywords("", getConfigName)) != 0) &&
            (constructMergeKeywords("", getConfigName) != "Default")) {
            qDebug("[Warning] check config name(%s / %s)", configName.toUtf8().constData(),
                   constructMergeKeywords("", getConfigName).toUtf8().constData());
            continue;
        }
        const int configMergeType =
            getMergeKeywordType(configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::AndGroup)));
        static bool isMergeStart = false;
        switch (configMergeType) {
            case static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType): {
                // or
                isMergeStart = false;
                orSignalList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)] =
                    configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal));
                orSignalList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)] =
                    configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
                convertConfigDataList.append(orSignalList);
                retConfigList.append(convertConfigDataList);
                orSignalList.clear();
                convertConfigDataList.clear();
                break;
            }
            case static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart):
            case static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::Merge): {
                // and
                isMergeStart = true;
                andSignalList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)] =
                    configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal));
                andSignalList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)] =
                    configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
                convertConfigDataList.append(andSignalList);
                break;
            }
            case static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd): {
                // and
                if (isMergeStart == true) {
                    isMergeStart = false;
                    andSignalList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)] =
                        configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal));
                    andSignalList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)] =
                        configValueStrList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
                    convertConfigDataList.append(andSignalList);
                    retConfigList.append(convertConfigDataList);
                    andSignalList.clear();
                    convertConfigDataList.clear();
                }
                break;
            }
            default: {
                // no operation [MAX]
                break;
            }
        }
    }
    return retConfigList;
}

QString ConvertDataManager::findVehicleSignalElseTimeoutCrcValue(const QString& vehicleSignal, const QString& elseTimeoutCrc) {
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
    QMap<int, QStringList> dataInfo =
        SignalDataManager::instance().data()->isSignalDataList(vehicleSignal, QStringList(), QString("ICV"), dataType);
    QString resultValue;
    QStringList matchingTableList;
    if (vehicleSignal.contains("Vehicle.CV")) {
        matchingTableList = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV];
    } else {
        matchingTableList = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem];
    }
    auto findAndExtractValue = [&](const QList<QString>& list, const QString& prefix) -> QString {
        auto it = std::find_if(list.begin(), list.end(), [&prefix](const QString& str) { return str.startsWith(prefix); });

        if (it != list.end()) {
            int colonIndex = it->indexOf(":");
            if (colonIndex != -1) {
                QString value = it->mid(colonIndex + 1);
                // qDebug() << prefix << ":" << value;
                if (value.isEmpty()) {
                    value = "isEmpty";
                }
                return value;
            }
        }
        // qDebug() << prefix << "not found!";
        return QString();
    };

    resultValue = findAndExtractValue(matchingTableList, elseTimeoutCrc);
    if (!resultValue.isEmpty()) {
        return resultValue;
    }

    resultValue = findAndExtractValue(matchingTableList, "ELSE");
    return resultValue;
}

ConvertKeywordInfo ConvertDataManager::interpretInputValueKeyword(const QString& signal, const QString& value) {
    const QString notTriggerStr =
        ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger));
    const QString maxValue = QString::number(static_cast<uint64_t>(UINT32_MAX) + 1);
    ConvertKeywordInfo convertKeywordInfo;
    convertKeywordInfo.inputSignal = signal;
    QString removeNotTriggerSignal = signal;
    if (removeNotTriggerSignal.contains(notTriggerStr, Qt::CaseInsensitive)) {
        removeNotTriggerSignal.replace(
            QRegularExpression(QRegularExpression::escape(notTriggerStr), QRegularExpression::CaseInsensitiveOption), "");
    }
    QString inputValue = value;
    inputValue.remove(' ');
    QRegularExpression regex(",(?![^()]*\\))");
    QStringList parts = inputValue.split(regex);
    QString resultString;
    QString tempString;
    QString prevTempString;
    // qInfo() << "@@@@ inputSignal: " << convertKeywordInfo.inputSignal;
    // qInfo() << "@@@@ inputValue: " << inputValue;
    auto processTimeoutOrCrc = [&](const QString& type, QString& tempString) {
        if ((findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "VALUE") == "isEmpty" &&
             !findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, type).isEmpty()) ||
            !SignalDataManager::instance()
                 .data()
                 ->isSignalValueEnum(removeNotTriggerSignal, findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "ELSE"))
                 .isEmpty()) {
            if (type.contains("timeout", Qt::CaseInsensitive)) {
                tempString = "MESSAGE_TIMEOUT";
            } else if (type.contains("crc", Qt::CaseInsensitive)) {
                tempString = "CRC_ERROR";
            } else {
                qWarning() << QString("[ERROR] %1 is not defined in VSM").arg(type);
            }
        } else {
            if (type.contains("timeout", Qt::CaseInsensitive)) {
                tempString = "MESSAGE_TIMEOUT";
            } else if (type.contains("crc", Qt::CaseInsensitive)) {
                tempString = "CRC_ERROR";
            } else {
                qWarning() << QString("[ERROR] %1 is not defined in VSM").arg(type);
            }
        }
    };
    auto processEnum = [&](const QString& part, QStringList& enumList) {
        if (part.contains("timeout", Qt::CaseInsensitive)) {
            if (!SignalDataManager::instance().data()->isSignalValueEnum(removeNotTriggerSignal, "MESSAGE_TIMEOUT").isEmpty()) {
                enumList.removeAll("MESSAGE_TIMEOUT");
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(removeNotTriggerSignal,
                                                findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "TIMEOUT"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    removeNotTriggerSignal, findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "TIMEOUT")));
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(removeNotTriggerSignal,
                                                findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "ELSE"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    removeNotTriggerSignal, findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "ELSE")));
            } else {
                enumList << maxValue;
            }
        } else if (part.contains("crc", Qt::CaseInsensitive)) {
            if (!SignalDataManager::instance().data()->isSignalValueEnum(removeNotTriggerSignal, "CRC_ERROR").isEmpty()) {
                enumList.removeAll("CRC_ERROR");
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(removeNotTriggerSignal,
                                                findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "CRCERROR"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    removeNotTriggerSignal, findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "CRCERROR")));
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(removeNotTriggerSignal,
                                                findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "ELSE"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    removeNotTriggerSignal, findVehicleSignalElseTimeoutCrcValue(removeNotTriggerSignal, "ELSE")));
            } else {
                enumList << maxValue;
            }
        } else {
            enumList.removeAll(part);
        }
    };
    auto processBidirectionalPart = [&](const QString& part, QStringList& enumList, QString& tempResult) {
        tempString = part;
        if (part.contains('!') && part.contains('(')) {
            tempString.remove(QRegularExpression("[!()]"));
            if (tempString.toDouble() || tempString == "0") {
                tempResult += maxValue;
            } else {
                for (const QString& enumString : tempString.split(',')) {
                    processEnum(enumString, enumList);
                }
                tempResult += enumList.join(", ");
            }
        } else if (part.contains('(')) {
            tempString.remove(QRegularExpression("[()]"));
            if (tempString.contains("timeout", Qt::CaseInsensitive)) {
                processTimeoutOrCrc("TIMEOUT", tempString);
            } else if (tempString.contains("crc", Qt::CaseInsensitive)) {
                processTimeoutOrCrc("CRCERROR", tempString);
            }
            tempResult += tempString;
        } else if (part.contains('!')) {
            tempString.remove('!');
            if (tempString.toDouble() || tempString == "0") {
                tempResult += maxValue;
            } else {
                processEnum(tempString, enumList);
                tempResult += enumList.join(", ");
            }
        } else {
            if (tempString.contains("timeout", Qt::CaseInsensitive)) {
                tempResult += "MESSAGE_TIMEOUT";
            } else if (tempString.contains("crc", Qt::CaseInsensitive)) {
                tempResult += "CRC_ERROR";
            } else {
                tempResult += tempString;
            }
        }
    };
    for (const QString& part : parts) {
        QStringList inputSignalEnumList = SignalDataManager::instance().data()->isSignalValueEnum(true, removeNotTriggerSignal);
        if (part.contains("<->") || part.contains("<=>")) {
            convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::TwoWay;
            QStringList bidirectionalParts = part.split(QRegularExpression(R"((<->|<=>))"));
            QString tempResult;
            for (const QString& bidirectionalPart : bidirectionalParts) {
                processBidirectionalPart(bidirectionalPart, inputSignalEnumList, tempResult);
                tempResult += "<->";
            }
            if (tempResult.endsWith("<->")) {
                tempResult.chop(3);
            }
            QStringList aaaa = tempResult.split("<->");
            for (const QString& leftValue : aaaa[0].split(",")) {
                for (const QString& rightValue : aaaa[1].split(",")) {
                    if (!leftValue.isEmpty() && !rightValue.isEmpty()) {
                        bool leftIsNumber, rightIsNumber;
                        double leftNum = leftValue.toDouble(&leftIsNumber);
                        double rightNum = rightValue.toDouble(&rightIsNumber);
                        if ((leftValue != rightValue) || (leftIsNumber && rightIsNumber && leftNum != rightNum)) {
                            resultString += leftValue + ", " + rightValue + ", ";
                            resultString += rightValue + ", " + leftValue + ", ";
                            convertKeywordInfo.validInputData += rightValue + ", " + leftValue + ", ";
                        }
                    }
                }
            }
        } else if (part.contains("->") || part.contains("=>")) {
            convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::Flow;
            QStringList unidirectionalParts = part.split(QRegularExpression(R"((->|=>))"));
            QString tempResult;
            for (const QString& unidirectionalPart : unidirectionalParts) {
                processBidirectionalPart(unidirectionalPart, inputSignalEnumList, tempResult);
                tempResult += "->";
            }
            if (tempResult.endsWith("->")) {
                tempResult.chop(2);
            }
            QStringList aaaa = tempResult.split("->");
            for (const QString& leftValue : aaaa[0].split(",")) {
                for (const QString& rightValue : aaaa[1].split(",")) {
                    bool leftIsNumber, rightIsNumber;
                    double leftNum = leftValue.toDouble(&leftIsNumber);
                    double rightNum = rightValue.toDouble(&rightIsNumber);
                    if ((leftValue != rightValue) || (leftIsNumber && rightIsNumber && leftNum != rightNum)) {
                        resultString += leftValue + ", " + rightValue + ", ";
                        convertKeywordInfo.validInputData += rightValue + ", ";
                    }
                }
            }
        } else {
            tempString = part;
            if (tempString.contains("others", Qt::CaseInsensitive)) {
                resultString += QString("others");
            } else if ((tempString.contains("valuechanged", Qt::CaseInsensitive) ||
                        tempString.contains("value_changed", Qt::CaseInsensitive)) &&
                       !removeNotTriggerSignal.contains("SFC.Private.IGNElapsed.")) {
                convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::ValueChanged;
                if (inputSignalEnumList.isEmpty()) {
                    resultString += QString("1, 2");
                    convertKeywordInfo.validInputData += resultString;
                } else {
                    convertKeywordInfo.validInputData += inputSignalEnumList.join(", ");
                    for (const QString& leftValue : inputSignalEnumList) {
                        for (const QString& rightValue : inputSignalEnumList) {
                            if (!leftValue.isEmpty() && !rightValue.isEmpty()) {
                                bool leftIsNumber, rightIsNumber;
                                double leftNum = leftValue.toDouble(&leftIsNumber);
                                double rightNum = rightValue.toDouble(&rightIsNumber);

                                if (!(leftValue == rightValue || (leftIsNumber && rightIsNumber && leftNum == rightNum))) {
                                    resultString += leftValue + ", " + rightValue + ", ";
                                    resultString += rightValue + ", " + leftValue + ", ";
                                }
                            }
                        }
                        inputSignalEnumList.removeAll(leftValue);
                        if (inputSignalEnumList.size() == 1) {
                            break;
                        }
                    }
                }
            } else if (tempString.contains("D'") || tempString.contains("D`") || tempString.contains("D'") ||
                       tempString.contains("D’")) {
                convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::DontCare;
                if (removeNotTriggerSignal.contains("SFC.Private.IGNElapsed.")) {
                    resultString += QString("D'");
                } else {
                    resultString += inputSignalEnumList.isEmpty() ? maxValue : inputSignalEnumList.join(", ");
                }
            } else if (tempString.contains('~')) {
                QStringList enumString;
                if (tempString.contains('!') && tempString.contains('(')) {
                    tempString.remove(QRegularExpression("[!()]"));
                    enumString = tempString.split('~');
                    convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::Flow;
                    if (enumString[0].toDouble() || enumString[0] == "0") {
                        QString tempLeftValue =
                            ((enumString[0].toInt() - 1) < 0) ? "negative" : QString::number(enumString[0].toInt() - 1);
                        QString tempRightValue = QString::number(enumString[1].toInt() + 1);
                        if (enumString[0].toDouble() || enumString[0] == "0") {
                            if (tempLeftValue == "negative") {
                                resultString = QString("%1,%2").arg(enumString[1]).arg(tempRightValue);
                                convertKeywordInfo.validInputData += tempRightValue;
                            } else {
                                resultString = QString("%1,%2,%3,%4")
                                                   .arg(enumString[0])
                                                   .arg(tempLeftValue)
                                                   .arg(enumString[1])
                                                   .arg(tempRightValue);
                                convertKeywordInfo.validInputData += QString("%1,%2").arg(tempLeftValue).arg(tempRightValue);
                            }
                        }
                    }
                } else {
                    enumString = tempString.split('~');
                    convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::Range;
                    if (enumString[0].toDouble() || enumString[0] == "0") {
                        resultString += QString("%1,%2,%3,%4,%5,%6,%7,%8")
                                            .arg(maxValue)
                                            .arg(enumString[0])
                                            .arg(maxValue)
                                            .arg(QString::number(enumString[0].toInt() + 1))
                                            .arg(maxValue)
                                            .arg(QString::number(enumString[1].toInt() - 1))
                                            .arg(maxValue)
                                            .arg(enumString[1]);
                        convertKeywordInfo.validInputData += QString("%1,%2,%3,%4")
                                                                 .arg(enumString[0])
                                                                 .arg(QString::number(enumString[0].toInt() + 1))
                                                                 .arg(QString::number(enumString[1].toInt() - 1))
                                                                 .arg(enumString[1]);
                    }
                }
            } else if (tempString.contains("<=")) {
                tempString.remove("<=");
                if (tempString.toDouble() || tempString == "0") {
                    convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::LessThanEqual;
                    resultString += QString("%1,%2").arg(QString::number(tempString.toInt() + 1)).arg(tempString);
                    convertKeywordInfo.validInputData += tempString;
                }
            } else if (tempString.contains(">=")) {
                tempString.remove(">=");
                if (tempString.toDouble() || tempString == "0") {
                    convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::MoreThanEqual;
                    resultString += QString("%1,%2").arg(QString::number(tempString.toInt() - 1)).arg(tempString);
                    convertKeywordInfo.validInputData += tempString;
                }
            } else if (tempString.contains('<')) {
                tempString.remove('<');
                if (tempString.toDouble() || tempString == "0") {
                    convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::Under;
                    resultString += QString("%1,%2").arg(tempString).arg(QString::number(tempString.toInt() - 1));
                    convertKeywordInfo.validInputData += QString::number(tempString.toInt() - 1);
                }
            } else if (tempString.contains('>')) {
                tempString.remove('>');
                if (tempString.toDouble() || tempString == "0") {
                    convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::Over;
                    resultString += QString("%1,%2").arg(tempString).arg(QString::number(tempString.toInt() + 1));
                    convertKeywordInfo.validInputData += QString::number(tempString.toInt() + 1);
                }
            } else if (tempString.contains('!') && tempString.contains('(')) {
                tempString.remove(QRegularExpression("[!()]"));
                if (tempString.toDouble() || tempString == "0") {
                    tempString = maxValue;
                } else {
                    for (const QString& enumString : tempString.split(", ")) {
                        processEnum(enumString, inputSignalEnumList);
                    }
                    tempString = inputSignalEnumList.join(", ");
                }
                if (prevTempString != tempString) {
                    resultString += tempString + ", ";
                }
            } else if (tempString.contains('(')) {
                tempString.remove(QRegularExpression("[()]"));
                if (tempString.contains("timeout", Qt::CaseInsensitive)) {
                    processTimeoutOrCrc("TIMEOUT", tempString);
                } else if (tempString.contains("crc", Qt::CaseInsensitive)) {
                    processTimeoutOrCrc("CRCERROR", tempString);
                }
                resultString += tempString + ", ";
            } else if (tempString.contains('!')) {
                tempString.remove('!');
                if (tempString.toDouble() || tempString == "0") {
                    tempString = maxValue;
                } else {
                    processEnum(tempString, inputSignalEnumList);
                    tempString = inputSignalEnumList.join(", ");
                }
                if (prevTempString != tempString) {
                    resultString += tempString + ", ";
                }
            } else {
                if (tempString.contains("timeout", Qt::CaseInsensitive)) {
                    processTimeoutOrCrc("TIMEOUT", tempString);
                } else if (tempString.contains("crc", Qt::CaseInsensitive)) {
                    processTimeoutOrCrc("CRCERROR", tempString);
                }
                resultString += tempString + ", ";
            }
        }
        prevTempString = tempString;
    }
    if (resultString.endsWith(", ")) {
        resultString.chop(2);
    }
    if (convertKeywordInfo.validInputData.endsWith(", ")) {
        convertKeywordInfo.validInputData.chop(2);
    }
    convertKeywordInfo.inputValue = resultString;
    if (convertKeywordInfo.validInputData.isEmpty()) {
        convertKeywordInfo.validInputData = resultString;
    }
    // qInfo() << "@@@@ keywordType: " << static_cast<int>(convertKeywordInfo.keywordType) << " / "
    //         << "result: " << convertKeywordInfo.inputValue;
    // qInfo() << "@@@@ validInputData: " << convertKeywordInfo.validInputData;
    // qInfo() << "@@@@ removeNotTriggerSignal: " << removeNotTriggerSignal;
    return convertKeywordInfo;
}

QStringList parseArray(const QString& input) {
    QStringList result;
    QRegularExpression re("(\\d+)");
    QRegularExpressionMatchIterator it = re.globalMatch(input);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        result.append(match.captured(1));
    }
    return result;
}

QPair<QStringList, QList<CaseDataInfo>> ConvertDataManager::generateCombinations(
    const QString& templateExpr, const QMap<QString, QStringList>& valueMap, const QStringList& keys, const QList<int>& keywords,
    const QStringList& inputValues, const QStringList& validInputData, const QString& caseName, const QString& notTriggerStr,
    const QString& customNotTrigger, int& calValArrCnt, int index, QMap<QString, QString> current, const QString& maxValue) {
    QPair<QStringList, QList<CaseDataInfo>> results;
    if (index == keys.size()) {
        QString expr = templateExpr;
        CaseDataInfo caseDataInfo;
        int count = 0;
        for (const auto& key : keys) {
            ConvertKeywordInfo info;
            info.inputSignal = key;
            info.validInputData = current[key].remove(' ');
            if (expr.contains(key)) {
                if (info.inputSignal.contains(notTriggerStr, Qt::CaseInsensitive)) {
                    QString tempInputValues = inputValues[count];
                    info.inputValue = tempInputValues.prepend(customNotTrigger);
                    info.inputSignal.replace(
                        QRegularExpression(QRegularExpression::escape(notTriggerStr), QRegularExpression::CaseInsensitiveOption),
                        "");
                } else if (keywords[count] == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range) ||
                           keywords[count] == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged) ||
                           keywords[count] == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow) ||
                           keywords[count] == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)) {
                    QString tempInputValues = inputValues[count];
                    QStringList array = parseArray(tempInputValues.remove("[CustomFlow]"));
                    int midpoint = array.size() / 2;
                    QStringList array1 = array.mid(0, midpoint);
                    QStringList array2 = array.mid(midpoint);
                    QString tempString = QString(array1[calValArrCnt] + ", " + array2[calValArrCnt]);
                    info.inputValue = constructConvertKeywordDataInfo(keywords[count], tempString);
                    calValArrCnt++;
                } else if (inputValues[count].contains('[')) {
                    info.inputValue = inputValues[count];
                } else {
                    if (keywords[count] == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                        info.inputValue = current[key].remove(' ');
                    } else {
                        info.inputValue = constructConvertKeywordDataInfo(keywords[count], inputValues[count]);
                    }
                }
            } else {
                if (info.inputSignal.contains(notTriggerStr, Qt::CaseInsensitive)) {
                    QString tempInputValues = inputValues[count];
                    info.inputValue = tempInputValues.prepend(customNotTrigger);
                    info.inputSignal.replace(
                        QRegularExpression(QRegularExpression::escape(notTriggerStr), QRegularExpression::CaseInsensitiveOption),
                        "");
                } else if (inputValues[count].contains('[')) {
                    info.inputValue = inputValues[count];
                } else {
                    if (keywords[count] == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                        info.inputValue = current[key].remove(' ');
                    } else {
                        info.inputValue = constructConvertKeywordDataInfo(keywords[count], inputValues[count]);
                    }
                }
            }
            QString tempKey = key;
            if (key.contains("[sheet]", Qt::CaseInsensitive)) {
                tempKey = tempKey.remove(QRegularExpression("\\[sheet\\]", QRegularExpression::CaseInsensitiveOption));
                expr.replace(tempKey, current[key]);
            } else {
                expr.replace(key, current[key]);
            }
            caseDataInfo.convertInputDataInfo.append(info);
            count++;
        }
        QJSValue result = JSEngineManager::instance().getEngine().evaluate(
            expr.remove(QRegularExpression("\\[(cal|sheet)\\]", QRegularExpression::CaseInsensitiveOption))
                .replace("math", "Math"));
        if (caseName.contains("others", Qt::CaseInsensitive)) {
            caseDataInfo.caseName = "others";
        } else {
            caseDataInfo.caseName = caseName + " == " + result.toString();
        }
        results.first << result.toString();
        results.second << caseDataInfo;
        return results;
    }
    const QString& key = keys[index];
    for (const QString& value : valueMap[key]) {
        QMap<QString, QString> next = current;
        next[key] = value;
        auto subResult = generateCombinations(templateExpr, valueMap, keys, keywords, inputValues, validInputData, caseName,
                                              notTriggerStr, customNotTrigger, calValArrCnt, index + 1, next, maxValue);
        results.first += subResult.first;
        results.second += subResult.second;
    }
    return results;
}

bool isAnyInputSignalInOutputValues(const ResultInfo& resultInfo) {
    QSet<QString> inputSignals;
    for (const auto& caseDataInfo : resultInfo.caseDataInfoList) {
        for (const auto& convertInfo : caseDataInfo.convertInputDataInfo) {
            inputSignals.insert(convertInfo.inputSignal);
        }
    }
    for (const auto& outputDataInfo : resultInfo.outputDataInfoList) {
        const QString& outputValue = outputDataInfo.outputValue;

        for (const auto& inputSignal : inputSignals) {
            if (outputValue.contains(inputSignal)) {
                return true;
            }
        }
    }
    return false;
}

QList<ResultInfo> ConvertDataManager::interpretCalKeywordAndRedefineResultInfo(const ResultInfo& resultInfo) {
    QList<ResultInfo> resultList;
    const QString notTriggerStr =
        ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger));
    const QString customNotTrigger = ExcelUtil::instance().data()->isKeywordString(
        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger));
    auto convertInputValues = [this, notTriggerStr, customNotTrigger](CaseDataInfo& caseDataInfo) {
        for (auto& convertKeywordInfo : caseDataInfo.convertInputDataInfo) {
            if (convertKeywordInfo.inputSignal.contains(notTriggerStr, Qt::CaseInsensitive)) {
                convertKeywordInfo.inputValue.prepend(customNotTrigger);
                convertKeywordInfo.inputSignal.replace(
                    QRegularExpression(QRegularExpression::escape(notTriggerStr), QRegularExpression::CaseInsensitiveOption), "");
            } else if (convertKeywordInfo.keywordType != ivis::common::KeywordTypeEnum::KeywordType::Invalid &&
                       !convertKeywordInfo.inputValue.contains('[')) {
                convertKeywordInfo.inputValue = this->constructConvertKeywordDataInfo(
                    static_cast<int>(convertKeywordInfo.keywordType), convertKeywordInfo.inputValue);
            }
        }
    };
    // if (isAnyInputSignalInOutputValues(resultInfo)) {
    //     qDebug() << "있다. : " << resultInfo.resultName;
    // }
    if (!resultInfo.outputDataInfoList.isEmpty()) {
        bool isIncludeCalKeyword =
            std::any_of(resultInfo.outputDataInfoList.begin(), resultInfo.outputDataInfoList.end(),
                        [](const OutputDataInfo& info) { return info.outputValue.contains("[cal]", Qt::CaseInsensitive); });
        if (isIncludeCalKeyword) {
            for (const auto& outputDataInfo : resultInfo.outputDataInfoList) {
                if (outputDataInfo.outputValue.contains("[cal]", Qt::CaseInsensitive)) {
                    for (const auto& caseDataInfo : resultInfo.caseDataInfoList) {
                        CaseDataInfo caseCopy = caseDataInfo;
                        int calArrCount = 0;
                        convertInputValues(caseCopy);
                        const QString& exprTemplate = outputDataInfo.outputValue;

                        QMap<QString, QStringList> valueMap;
                        QStringList keys, inputValues, validInputData;
                        QList<int> keywords;
                        for (const auto& keyword : caseCopy.convertInputDataInfo) {
                            valueMap[keyword.inputSignal] = keyword.validInputData.split(',');
                            keys << keyword.inputSignal;
                            keywords << static_cast<int>(keyword.keywordType);
                            inputValues << keyword.inputValue;
                            validInputData << keyword.validInputData;
                        }
                        auto results = generateCombinations(exprTemplate, valueMap, keys, keywords, inputValues, validInputData,
                                                            caseCopy.caseName, notTriggerStr, customNotTrigger, calArrCount);
                        for (int i = 0; i < results.first.size(); ++i) {
                            ResultInfo newResult;
                            newResult.resultName = caseCopy.caseName.contains("others", Qt::CaseInsensitive)
                                                       ? resultInfo.resultName
                                                       : resultInfo.resultName + kEasterEggTrigger + " == " + results.first[i];
                            QList<OutputDataInfo> tempOutputList = resultInfo.outputDataInfoList;
                            for (auto& outputDataInfo : tempOutputList) {
                                if (outputDataInfo.outputValue.contains("[cal]", Qt::CaseInsensitive)) {
                                    outputDataInfo.outputValue = results.first[i];
                                }
                                newResult.outputDataInfoList << outputDataInfo;
                            }
                            newResult.caseDataInfoList << results.second[i];
                            resultList << newResult;
                        }
                    }
                }
            }
        } else {
            ResultInfo tempResultInfo = resultInfo;
            for (auto& caseDataInfo : tempResultInfo.caseDataInfoList) {
                convertInputValues(caseDataInfo);
            }
            resultList << tempResultInfo;
        }
    } else {
        ResultInfo tempResultInfo = resultInfo;
        for (auto& caseDataInfo : tempResultInfo.caseDataInfoList) {
            convertInputValues(caseDataInfo);
        }
        resultList << tempResultInfo;
    }
    return resultList;
}

bool ConvertDataManager::decideSameCaseList(const QList<CaseDataInfo>& list1, const QList<CaseDataInfo>& list2) {
    if (list1.size() != list2.size()) {
        return false;
    }
    for (const CaseDataInfo& item : list1) {
        if (!list2.contains(item)) {
            return false;
        }
    }
    return true;
}

QList<ResultInfo> ConvertDataManager::mergeAndCleanResultList(const QList<ResultInfo>& resultList) {
    QMap<QString, QList<ResultInfo>> groupedMap;
    for (const ResultInfo& result : resultList) {
        groupedMap[result.resultName].append(result);
    }
    QList<ResultInfo> mergedResultList;
    for (auto it = groupedMap.begin(); it != groupedMap.end(); ++it) {
        const QString& resultName = it.key();
        const QList<ResultInfo>& groupList = it.value();
        QList<QList<CaseDataInfo>> uniqueCaseLists;
        QList<OutputDataInfo> sharedOutputList;
        bool isFirst = true;
        for (const ResultInfo& r : groupList) {
            bool found = false;
            for (const QList<CaseDataInfo>& existing : uniqueCaseLists) {
                if (decideSameCaseList(existing, r.caseDataInfoList)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                uniqueCaseLists.append(r.caseDataInfoList);
                if (isFirst) {
                    sharedOutputList = r.outputDataInfoList;
                    isFirst = false;
                }
            }
        }
        QList<CaseDataInfo> mergedCases;
        for (const QList<CaseDataInfo>& caseList : uniqueCaseLists) {
            for (const CaseDataInfo& c : caseList) {
                if (!mergedCases.contains(c)) {
                    mergedCases.append(c);
                }
            }
        }
        ResultInfo merged;
        merged.resultName = resultName;
        merged.outputDataInfoList = sharedOutputList;
        merged.caseDataInfoList = mergedCases;
        mergedResultList.append(merged);
    }
    return mergedResultList;
}

void ConvertDataManager::updateResultDataInfo(const int& sheetIndex, const QString& tcName,
                                              const QList<ResultInfo>& resultDataList) {
    // update result data
    for (int resultIdx = 0; resultIdx < resultDataList.size(); ++resultIdx) {
        QString resultStr;
        QList<QStringList> convertedOutputDataList;
        const ResultInfo& resultData = resultDataList.at(resultIdx);
        resultStr = resultData.resultName;
        const QList<CaseDataInfo>& caseDataInfoList = resultData.caseDataInfoList;
        const QList<OutputDataInfo>& outputDataInfoList = resultData.outputDataInfoList;
        // 1. update Case Data Info first
        for (int caseIdx = 0; caseIdx < caseDataInfoList.size(); ++caseIdx) {
            const QString& caseStr = caseDataInfoList.at(caseIdx).caseName;
            QPair<QStringList, QStringList> inputDataInfo;
            QList<ConvertKeywordInfo> convertInputDataInfoList = caseDataInfoList.at(caseIdx).convertInputDataInfo;
            // QList<ConvertKeywordInfo> convertInputDataInfo -> QPair<QStringList, QStringList>로 convert 필요
            for (const ConvertKeywordInfo& convertKeywordInfo : convertInputDataInfoList) {
                inputDataInfo.first.append(convertKeywordInfo.inputSignal);
                inputDataInfo.second.append(convertKeywordInfo.inputValue);
            }
            ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, tcName, resultStr, caseStr, inputDataInfo);
        }
        // 2. processing output data after case data update is completed
        QList<OutputDataInfo> outputDataList = resultDataList.at(resultIdx).outputDataInfoList;
        for (int outputIdx = 0; outputIdx < outputDataList.size(); ++outputIdx) {
            OutputDataInfo outputDataInfo = outputDataList.at(outputIdx);
            QStringList convertedOutputDataInfo = {outputDataInfo.outputSignal, outputDataInfo.initialize,
                                                   outputDataInfo.outputValue};
            convertedOutputDataList.append(convertedOutputDataInfo);
        }
        ExcelDataManager::instance().data()->updateOutputDataInfo(sheetIndex, tcName, resultStr, convertedOutputDataList);
    }
}

void ConvertDataManager::displayResultDataInfo(const ResultInfo& result) {
    qDebug() << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";
    qDebug() << "1. resultName   : " << result.resultName;
    for (int i = 0; i < result.caseDataInfoList.size(); ++i) {
        qDebug() << "2. caseName     : " << result.caseDataInfoList.at(i).caseName;
        QStringList signalList;
        QStringList valueList;

        for (const auto& info : result.caseDataInfoList.at(i).convertInputDataInfo) {
            signalList.append(info.inputSignal);
            valueList.append(info.inputValue);
        }
        qDebug() << "3. inputSignal  : " << signalList.join(", ");
        qDebug() << "4. inputValue   : " << valueList.join(", ");
    }
    for (int i = 0; i < result.outputDataInfoList.size(); ++i) {
        qDebug() << "5. outputSignal : " << result.outputDataInfoList.at(i).outputSignal;
        qDebug() << "6. initialize   : " << result.outputDataInfoList.at(i).initialize;
        qDebug() << "7. outputValue  : " << result.outputDataInfoList.at(i).outputValue;
    }
}

#define OUTPUT_DATA_INFO_SIZE 3
QList<OutputDataInfo> ConvertDataManager::convertOutputStructData(const QList<QStringList>& outputList) {
    QList<OutputDataInfo> retInfoList;
    for (int i = 0; i < outputList.size(); ++i) {
        OutputDataInfo info;
        QStringList outputDataList = outputList.at(i);
        if (outputDataList.size() == OUTPUT_DATA_INFO_SIZE) {
            info.outputSignal = outputDataList.at(0);
            info.initialize = outputDataList.at(1);
            info.outputValue = outputDataList.at(2);
            retInfoList.append(info);
        } else {
            qDebug() << "[Error] Invalid of Output Data Info(Signal, Initialize, Value)";
        }
    }

    return retInfoList;
}

void ConvertDataManager::appendCurSheetData(
    const QString& tcName, const QString& resultName, const QString& caseName,
    const QPair<QStringList, QStringList>& inputDataInfo,
    QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>>& retCurSheetData) {
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
    qDebug() << "======================= [appendCurSheetData] ==========================================";
    qDebug() << "insert Cur Sheet Data : ";
    qDebug() << "1) tcName        : " << tcName;
    qDebug() << "2) resultName    : " << resultName;
    qDebug() << "3) caseName      : " << caseName;
    qDebug() << "4) inputDataInfo : " << inputDataInfo;
#endif
    retCurSheetData.append(make_tuple(tcName, resultName, caseName, inputDataInfo));
}

void ConvertDataManager::updateCurSheetData(
    const int& sheetIndex, const QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>>& retCurSheetData) {
    for (int i = 0; i < retCurSheetData.size(); ++i) {
        auto tmpSheetData = retCurSheetData.at(i);
        // std::get<0> : tcName
        // std::get<1> : resultName
        // std::get<2> : caseName
        // std::get<3> : inputDataInfo
        ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, std::get<0>(tmpSheetData), std::get<1>(tmpSheetData),
                                                                 std::get<2>(tmpSheetData), std::get<3>(tmpSheetData));
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
        qDebug() << "======================= [updateCurSheetData] ==========================================";
        qDebug() << "0. sheetIndex    : " << sheetIndex;
        qDebug() << "1. tcName        : " << std::get<0>(tmpSheetData);
        qDebug() << "2. resultName    : " << std::get<1>(tmpSheetData);
        qDebug() << "3. caseName      : " << std::get<2>(tmpSheetData);
        qDebug() << "4. inputDataInfo : " << std::get<3>(tmpSheetData);
#endif
    }
}

// [Sheet] 키워드 해석 기능
QList<QPair<QString, QPair<QStringList, QStringList>>> ConvertDataManager::convertSheetKeyword(
    const QPair<QStringList, QStringList>& inputDataPairInfo, const QString& genTypeStr) {
    QList<QPair<QString, QPair<QStringList, QStringList>>> sheetDataInfo;
    QStringList inputSignalStrList = inputDataPairInfo.first;
    QStringList inputDataStrList = inputDataPairInfo.second;

    bool isSheetKeywordAlreadyExsit = false;
    if ((inputSignalStrList.isEmpty() == false && inputDataStrList.isEmpty() == false) &&
        (inputSignalStrList.size() == inputDataStrList.size())) {
        for (int index = 0; index < inputSignalStrList.size(); ++index) {
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
            qDebug() << "-------------------------------------------------------------------------------";
            qDebug() << "---> signal : " << inputSignalStrList.at(index);
            qDebug() << "--->   data : " << inputDataStrList.at(index);
#endif
            QString inputSignalStr = inputSignalStrList.at(index);
            QString inputDataStr = inputDataStrList.at(index);
            if (inputSignalStrList.at(index).contains("[Sheet]") == true && isSheetKeywordAlreadyExsit == false) {
                inputSignalStr.remove("[Sheet]");
                isSheetKeywordAlreadyExsit = true;
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
                qDebug() << "----> [Sheet] keyword Exist!";
                qDebug() << "----> remove [Sheet] : " << inputSignalStr;
#endif
                // input signal : [Sheet]Private_A
                // input data   : ON, OFF... (2개 이상의 data 지원하기 위함)
                QStringList inputDataSplitStr = inputDataStr.remove(" ").split(",");
                for (int i = 0; i < inputDataSplitStr.size(); ++i) {
                    QString tcNameInSheetKeyword = inputSignalStr;
                    QString resultNameInSheetKeyword = inputDataSplitStr.at(i);
                    // [Cal]이나 [Sheet]에 의해서 Result 가 추가되면서 Result 명칭이 바뀌기 때문에
                    // 바뀐 명칭의 origin(원래)의 이름으로 찾을 수 있도록 하는 함수
                    QStringList multiResultList = getSheetHasMultiResult(tcNameInSheetKeyword, resultNameInSheetKeyword);

                    QStringList resultParamList =
                        (multiResultList.isEmpty()) ? QStringList() << resultNameInSheetKeyword : multiResultList;

                    for (const QString& resultKeyword : resultParamList) {
                        // QList<QPair<QString, QPair<QStringList, QStringList>>>
                        // QList<CaseName, pair<input signal list, input data list>>
                        auto sheetSignalDataInfo = getSheetSignalDataInfo(tcNameInSheetKeyword, resultKeyword);

                        if (sheetSignalDataInfo.isEmpty() == false) {
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
                            qDebug() << "-----> [Sheet] Info : " << sheetSignalDataInfo;
#endif
                            for (int i = 0; i < sheetSignalDataInfo.size(); ++i) {
                                // [CaseName, <InputSignalList, InputDataList>]
                                QString sheetCaseName = sheetSignalDataInfo.at(i).first;
                                QPair<QStringList, QStringList> sheetInputDataInfo = sheetSignalDataInfo.at(i).second;
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
                                qDebug()
                                    << "====================================================================================";
                                qDebug() << "1. caseName      : " << sheetCaseName;
                                qDebug() << "2. [Sheet] inputSignalStrList : " << sheetInputDataInfo.first;
                                qDebug() << "3. [Sheet] inputDataInfo      : " << sheetInputDataInfo.second;
                                qDebug() << "4. [Excel] inputSignalStrList : " << inputSignalStrList;
                                qDebug() << "5. [Excel] inputDataInfo      : " << inputDataStrList;
#endif
                                auto tmpInputDataInfo = getMergedInputDataInfo(genTypeStr, inputDataPairInfo, sheetInputDataInfo);
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
                                qDebug() << "[[[Result]]] tmpInputDataInfo : " << tmpInputDataInfo;
#endif
                                sheetDataInfo.append(qMakePair(sheetCaseName, tmpInputDataInfo));
                            }
                        }
                    }
                }
            } else {
                // [Sheet] 키워드 존재하지 않음
                // 원래 data 삽입
                // sheetDataInfo Empty
            }
        }
    }
    return sheetDataInfo;
}

QString ConvertDataManager::deleteEasterEggKeyword(const QString& keyword) {
    return QString(keyword).remove(kEasterEggTrigger);
}

QStringList ConvertDataManager::getSheetHasMultiResult(const QString& tcName, const QString& result) {
    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) + 1;
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);

    QStringList resultList;

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        if (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }
        QStringList resultStrList = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName);

        for (QString easterEggResultStr : resultStrList) {
#if defined(ENABLE_MULTI_RESULT_SHEET_DEBUG_LOG)
            qDebug() << "=========================================================";
            qDebug() << " easterEggResultStr : " << easterEggResultStr;
            qDebug() << " origin result String  : " << result;
#endif
            QStringList splitResultStrUsingEastEgg = easterEggResultStr.split(kEasterEggTrigger);
            if (splitResultStrUsingEastEgg.size() > 1) {
                // $EASTEREGG$ 존재
                if (result == splitResultStrUsingEastEgg.at(0)) {
                    // EasterEgg Flag 뒤 문자열을 제거하면, 원본의 Result 와의 동일 여부 판단
                    // [Cal] 또는 [Sheet] 해석 시 result가 추가되지만, 원본 result 결과는 동일하기 때문
#if defined(ENABLE_MULTI_RESULT_SHEET_DEBUG_LOG)
                    qDebug() << "Appended Multi-Result String : " << easterEggResultStr;
#endif
                    resultList.append(easterEggResultStr);
                }
            }
        }
    }
#if defined(ENABLE_MULTI_RESULT_SHEET_DEBUG_LOG)
    qDebug() << "-------------------------------------------------------------";
    qDebug() << "[Last] Multi-Result List : " << resultList;
#endif

    return resultList;
}

// List = [CaseName1, (InputSignalList, InputDataList)] + [CaseName2, (InputSignalList, InputDataList)] + ...
QList<QPair<QString, QPair<QStringList, QStringList>>> ConvertDataManager::getSheetSignalDataInfo(const QString& name,
                                                                                                  const QString& data) {
    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) + 1;
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);
    QList<QPair<QString, QPair<QStringList, QStringList>>> sheetKeywordSignalDataInfo;

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        if (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }

        QStringList caseStrList = ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, name, data);
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
        qDebug() << "-----> Find Sheet Case List : " << caseStrList;
#endif
        for (int caseIndex = 0; caseIndex < caseStrList.size(); ++caseIndex) {
            QString caseNameStr = caseStrList.at(caseIndex);
            // name = tcName
            // data = resultName
            QPair<QStringList, QStringList> caseInputDataList =
                ExcelDataManager::instance().data()->isInputDataList(sheetIndex, name, data, caseNameStr, false);

            if (caseInputDataList.first.isEmpty() == false && caseInputDataList.second.isEmpty() == false) {
                sheetKeywordSignalDataInfo.append(qMakePair(caseNameStr, caseInputDataList));
            }
        }
        if (caseStrList.isEmpty() == false) {
            break;
        }
    }
#if defined(ENABLE_INPUT_SIGNAL_SHEET_KEYWORD_DEBUG_LOG)
    qDebug() << "[result] sheetKeywordSignalDataInfo : " << sheetKeywordSignalDataInfo;
#endif
    return sheetKeywordSignalDataInfo;
}

QString ConvertDataManager::getIgnPrefixString(const QString& signal) {
    QString retStr;
    if (signal.startsWith(SFC_IGN_ELAPSED + "On")) {
        retStr = SFC_IGN_ELAPSED + "On";
    } else if (signal.startsWith(SFC_IGN_ELAPSED + "Off")) {
        retStr = SFC_IGN_ELAPSED + "Off";
    } else {
        // no operation
    }

    return retStr;
}

QPair<QStringList, QStringList> ConvertDataManager::getMergedInputDataInfo(const QString& genTypeStr,
                                                                           const QPair<QStringList, QStringList>& origin,
                                                                           const QPair<QStringList, QStringList>& sheet) {
    QPair<QStringList, QStringList> retVal;

    QSet<QString> duplicatedSignalCheckList;  // 중복 확인용 Set
    QStringList mergedSignalList;
    QStringList mergedDataList;

    const QStringList& originSignalList = origin.first;
    const QStringList& originDataList = origin.second;
    const QStringList& sheetSignalList = sheet.first;
    const QStringList& sheetDataList = sheet.second;

    bool isSheetAlreadyInserted = false;
    bool isSheetAlreadyExist = false;
    for (int i = 0; i < originSignalList.size(); ++i) {
        const QString& signal = originSignalList[i];
        const QString& data = originDataList[i];

        // [Sheet]를 사용하는 origin signal/data가 사용되는 [Sheet] 내부 signal/data와 같으면 origin 데이터를 우선적으로 사용
        // 원본 excel에서 [Sheet] Keyword를 만나면 삭제하고 [Sheet] 내부 input signal/data 리스트 삽입
        if (signal.contains("[Sheet]") && isSheetAlreadyExist == false) {
            isSheetAlreadyExist = true;
            QString customNotTriggerStr = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger));
            if (!isSheetAlreadyInserted) {
                // 현재 위치부터 [Sheet] 데이터를 삽입하는 로직
                for (int j = 0; j < sheetSignalList.size(); ++j) {
                    if (!duplicatedSignalCheckList.contains(sheetSignalList[j])) {
                        bool isIGNSignalDuplicated = false;
                        // IGN의 경우 SFC.Private.IGNElapsed.ElapsedOn500ms 와 ElapedOn3500ms 과 같이 full string data는 다르지만
                        // IGN ON 이라는 값의 의미는 같기 때문에 같은 노드로 인식하게 하기 위한 조건
                        QString ignPrefixSignalInSheet = getIgnPrefixString(sheetSignalList[j]);
                        if (ignPrefixSignalInSheet.isEmpty() == false) {
                            for (const QString& str : duplicatedSignalCheckList) {
                                if (str.contains(ignPrefixSignalInSheet)) {
                                    isIGNSignalDuplicated = true;
                                    break;
                                }
                            }
                        }
                        // [Sheet] 내부 input signal/data 추가하는 로직
                        if (isIGNSignalDuplicated == false) {
                            QString appendMergedSignalStr = sheetSignalList[j];
                            QString appendMergedDataStr = sheetDataList[j];

                            duplicatedSignalCheckList.insert(appendMergedSignalStr);
                            mergedSignalList.append(appendMergedSignalStr);
                            // Negative or Positive 인 경우에만 [Sheet] Data 참조 시 [Not_Trigger] keyword 추가
                            if (genTypeStr == GEN_TYPE_NEGATIVE_AND_POSITIVE || genTypeStr == GEN_TYPE_POSITIVE) {
                                // [Sheet]의 내부 input signal에 또 [Sheet]가 있는 경우는 data에 [Not_Trigger] 추가하지 않음
                                // e.g) [Sheet]Private_A - [Not_Trigger]OFF 가 되면 다음 [Sheet] 해석 시 OFF 찾지 못함
                                if (!appendMergedSignalStr.contains("[Sheet]") &&
                                    !appendMergedSignalStr.startsWith(SFC_IGN_ELAPSED)) {
                                    appendMergedDataStr = customNotTriggerStr + appendMergedDataStr;
                                }
                            }
                            mergedDataList.append(appendMergedDataStr);
                        }
                    }
                }
                // 한 Case에 [Sheet]가 2개 이상있는 경우, 한 번만 삽입하도록 하기 위한 조건
                // 구조상 원본 Data와 실시간 Update Data를 독립적으로 관리하여,
                // 2개의 Case를 동시에 Sheet 데이터를 해석하지 않고,
                // 2 depth [Sheet] 키워드 해석 방식과 동일한 로직 수행하도록 구현
                isSheetAlreadyInserted = true;
            }
            continue;  // 기존의 [Sheet] 포함된 내부 signal/data는 무시
        }

        // [Sheet] 사용하는 case에 존재하는 input signal/data 추가하는 로직 (중복 아닌 경우 추가)
        if (!duplicatedSignalCheckList.contains(signal)) {
            duplicatedSignalCheckList.insert(signal);
            mergedSignalList.append(signal);
            mergedDataList.append(data);
        }
    }

    retVal.first = mergedSignalList;
    retVal.second = mergedDataList;

    return retVal;
}

QString ConvertDataManager::mergeCaseName(const QString& prefix, const QString& suffix) {
    QString mergedCaseName;

    if (prefix.isEmpty() == false && suffix.isEmpty() == false) {
        mergedCaseName = prefix + "_" + suffix;
    }

    return mergedCaseName;
}

inline QString ConvertDataManager::constructMergeKeywords(const QString& additionalKeyword, const QString& baseKeyword) const {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    QString returnStr = baseKeyword;
    ivis::common::getRemoved(returnStr, mergeInfos);
    if (additionalKeyword.isEmpty() == false) {
        returnStr = additionalKeyword + returnStr;
    }
    return returnStr;
}

int ConvertDataManager::getMergeKeywordType(const QString& data) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();

    int mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType);

    if (data.trimmed().startsWith(mergeStart) == true) {
        mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart);
    } else if (data.trimmed().startsWith(merge) == true) {
        mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::Merge);
    } else if (data.trimmed().startsWith(mergeEnd) == true) {
        mergeKeywordType = static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd);
    } else {
        // NoMergeType
    }

    return mergeKeywordType;
}

QMap<int, QStringList> ConvertDataManager::isTCNameDataInfo(const QString& tcName, const QString& result,
                                                            const QList<int>& columnList, const bool& convert,
                                                            const bool& mergeInfoErase, QList<QStringList>& convertData) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    QPair<int, int> rowInfo((-1), (-1));
    int foundSheetIndex = 0;

    if (convert) {
        startIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
        endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;
    }

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        rowInfo = isContainsRowInfo(sheetIndex, tcName, result, QString());
        if ((rowInfo.first >= 0) && (rowInfo.second >= 0)) {
            foundSheetIndex = sheetIndex;
            break;
        }
    }

    if (foundSheetIndex == 0) {
        qDebug() << "Fail to found tcName :" << tcName << " or result :" << result;
        return QMap<int, QStringList>();
    }

    QMap<int, QStringList> tcNameDataInfo = QMap<int, QStringList>();
    QMap<int, QMap<int, QString>> tempTcNameDataInfo;
    int searchRowIndex = 0;

    qDebug() << "Found TCName :" << foundSheetIndex << tcName << result << rowInfo;

    const auto sheetData = ExcelData::instance().data()->getSheetData(foundSheetIndex).toList();
    for (const auto& rowDataList : sheetData) {
        QVariantList rowData = rowDataList.toList();
        if ((searchRowIndex >= rowInfo.first) && (searchRowIndex <= rowInfo.second)) {
            QMap<int, QString> columnData;
            for (const auto& columnIndex : columnList) {
                if ((result.size() > 0) && (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))) {
                    // Skip - [reulst value valid] & [columnList contains Result]
                    continue;
                }
                QString dataInfo = rowData.at(columnIndex).toString();
                if (mergeInfoErase) {
                    ivis::common::getRemoved(dataInfo, mergeInfos);
                }
                tcNameDataInfo[columnIndex].append(dataInfo);
                tcNameDataInfo[columnIndex].removeAll("");
                tcNameDataInfo[columnIndex].removeDuplicates();

                columnData[columnIndex] = dataInfo;
            }

            tempTcNameDataInfo[searchRowIndex] = columnData;
        }
        searchRowIndex++;
    }

    QList<QStringList> foundRowData;
    for (auto iter = tempTcNameDataInfo.cbegin(); iter != tempTcNameDataInfo.cend(); ++iter) {
        QStringList dataInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
        for (auto iterData = iter.value().cbegin(); iterData != iter.value().cend(); ++iterData) {
            int columnIndex = iterData.key();
            if (columnIndex < static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max)) {
                dataInfo[columnIndex] = iterData.value();
            }
        }
        foundRowData.append(dataInfo);
    }

    bool appendState = false;
    QList<QStringList> tempList;
    QList<QStringList> rowData = convertData;
    convertData.clear();

    for (auto& foundRowInfo : foundRowData) {
        QString caseInfo = foundRowInfo.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
        if (caseInfo.contains(mergeStart)) {
            tempList.append(foundRowInfo);
        } else if (caseInfo.contains(merge)) {
            tempList.append(foundRowInfo);
        } else if (caseInfo.contains(mergeEnd)) {
            tempList.append(foundRowInfo);
            appendState = true;
        } else {
            if (foundRowData.size() == 1) {
                if (rowData.size() == 0) {
                    convertData.append(rowData + foundRowData);
                    break;
                } else {
                    tempList.append(foundRowInfo);
                    appendState = true;
                }
            }
        }

        if (appendState) {
            appendState = false;
            for (auto& rowInfo : rowData) {
                QString rowSignalInfo = rowInfo.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                auto iter = std::remove_if(tempList.begin(), tempList.end(),
                                           [&](const QStringList& row) { return row.contains(rowSignalInfo); });
                tempList.erase(iter, tempList.end());

                rowInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] = QString();
                rowInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] = QString();
                rowInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] = QString();
            }

            tempList = rowData + tempList;
            ivis::common::getRemoved(caseInfo, mergeInfos);

            for (int index = 0; index < tempList.size(); ++index) {
                QString mergeText;
                if (index == 0) {
                    mergeText = mergeStart;
                } else if (index == (tempList.size() - 1)) {
                    mergeText = mergeEnd;
                } else {
                    mergeText = merge;
                }
                tempList[index][static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] = (mergeText + caseInfo);
            }
            convertData.append(tempList);
            tempList.clear();
        }
    }

#if 0
    qDebug() << "\n\n\n >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "========================================================================================";
    qDebug() << "isTCNameDataInfo :" << tcName << result << ", ColumnIndex :" << columnList;
    qDebug() << "\t ========================================================================================";
    for (const auto& dataInfo : rowData) {
        qDebug() << "\t RowInfo     :" << dataInfo;
    }
    qDebug() << "\t ========================================================================================";
    for (const auto& dataInfo : foundRowData) {
        qDebug() << "\t FoundInfo   :" << dataInfo;
    }
    qDebug() << "\t ========================================================================================";
    for (const auto& dataInfo : convertData) {
        qDebug() << "\t ConvertInfo :" << dataInfo;
    }
    qDebug() << "========================================================================================";

#if 0
    qDebug() << "isTCNameDataInfo :" << tcName << result << ", ColumnIndex :" << columnList;
    qDebug() << "Sheet :" << foundSheetIndex;
    qDebug() << "RowInfo[TCName] :" << tcNamerowInfo.first << tcNamerowInfo.second;
    qDebug() << "RowInfo[Result] :" << resultRowInfo.first << resultRowInfo.second;
    qDebug() << "TCNameDataInfo  :" << tcName << result;
    qDebug() << "RowData         :" << rowData;
    qDebug() << "FoundData       :" << foundRowData;
    qDebug() << "ConvertData     :" << convertData;
#endif
    qDebug() << "\n <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
#endif

    return tcNameDataInfo;
}

QPair<int, int> ConvertDataManager::isContainsRowInfo(const int& sheetIndex, const QString& input1, const QString& input2,
                                                      const QString& input3, const bool& normal) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    int columnMax = 0;
    QList<QPair<int, QString>> compareDataInfo;

    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
        // not support sheet : description
        return QPair<int, int>((-1), (-1));
    } else if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
               (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::Max);
        compareDataInfo = QList({
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName), input1),
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal), input2),
        });
    } else {
        columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
        compareDataInfo = QList({
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName), input1),
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result), input2),
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case), input3),
        });
    }

    if (normal == false) {
        compareDataInfo = QList({
            QPair<int, QString>(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case), input3),
        });
    }

    QMap<int, QStringList> readColumnDatas;
    const auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
    for (const auto& rowDataList : sheetData) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < columnMax) {
            continue;
        }
        for (const auto& dataInfo : compareDataInfo) {
            int readColumnIndex = dataInfo.first;
            QString readColumnText = rowData.at(static_cast<int>(readColumnIndex));
            readColumnDatas[readColumnIndex].append(readColumnText);
        }
    }

    QMap<int, QPair<int, int>> rowIndexs;
    int previousIndex = (-1);
    for (const auto& dataInfo : compareDataInfo) {
        QString compareColumnText = dataInfo.second;
        if (compareColumnText.size() == 0) {
            continue;
        }
        ivis::common::getRemoved(compareColumnText, mergeInfos);
        int columnIndex = dataInfo.first;
        QStringList originColumnText = readColumnDatas[columnIndex];
        QPair<int, int> foundRowInfo((-1), (-1));
        int rowIndex = 0;

        for (const auto& originColumnText : originColumnText) {
            QString removeOriginText = originColumnText;
            ivis::common::getRemoved(removeOriginText, mergeInfos);
            bool state = ivis::common::isCompareString(removeOriginText, compareColumnText);
            // qDebug() << "\t\t State[" << columnIndex << "] :" << state << compareColumnText << removeOriginText
            //          << originColumnText;
            if (state) {
                if (ivis::common::isContainsString(originColumnText, mergeEnd)) {
                    foundRowInfo = QPair<int, int>(foundRowInfo.first, rowIndex);
                    break;
                } else {
                    if (ivis::common::isContainsString(originColumnText, merge) == false) {
                        foundRowInfo = QPair<int, int>(rowIndex, rowIndex);
                    }
                }
            }
            rowIndex++;
        }

        if (foundRowInfo == QPair<int, int>((-1), (-1))) {
            // qDebug() << "\t Not found matching column["<< columnIndex << "] text :" << compareColumnText << originColumnText;
            break;
        }

        QPair<int, int> previousRowInfo = (rowIndexs.size() == 0) ? (foundRowInfo) : (rowIndexs[previousIndex]);
        bool update = ((foundRowInfo.first >= previousRowInfo.first) && (foundRowInfo.second <= previousRowInfo.second));
        // qDebug() << "\t RowInfo[" << previousIndex << "] :" << update << previousRowInfo << ">" << foundRowInfo;

        if (update) {
            previousIndex = columnIndex;
            rowIndexs[columnIndex] = foundRowInfo;
            // qDebug() << "\t RowInfo[" << columnIndex << "] :" << foundRowInfo;
            // qDebug() << "\t ---------------------------------------------------------------------------------";
        }
    }

    // input3 -> input2 -> input1 순서로 사이즈 > 0 이면, 해당 리스트에서 rowIndex 리턴, 0 이면 초기값 리턴
    QPair<int, int> rowInfo((-1), (-1));
    for (auto iter = compareDataInfo.rbegin(); iter != compareDataInfo.rend(); ++iter) {
        QString columnText = iter->second;
        if (columnText.size() > 0) {
            int columnIndex = iter->first;
            rowInfo = (rowIndexs.contains(columnIndex)) ? (rowIndexs[columnIndex]) : (QPair<int, int>((-1), (-1)));
            break;
        }
    }
    // qDebug() << "\t isContainsRowInfo :" << columnMax << input1 << input2 << input3;
    // qDebug() << "\t\t RowInfo :"  << rowInfo << ", All :" << rowIndexs;

    return rowInfo;
}
