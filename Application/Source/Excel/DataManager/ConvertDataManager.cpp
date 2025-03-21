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
// #define ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG
// #define ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG
// #define ENABLE_CONVERTING_KEYWORD_DATA_INFO
// #define ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG

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

    // NOTE(csh): [Sheet] Keyword 기능 수행(row data append) -> 나머지 Keyword 기능 수행(cell data changed) + 001 excel 파일 생성
    if (convertKeywordData() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_001";
            ExcelUtil::instance().data()->writeExcelSheet(filePath, true);
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_001");
    }

    // if (convertKeywordData() == true) {
    //     if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
    //         QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_003";
    //         ExcelUtil::instance().data()->writeExcelSheet(filePath, true);
    //     }
    //     checkTimer.check("excuteConvertDataManager : Convert.excel_003");
    // }

    if (appendConvertConfigSignalSet() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_Config";
            ExcelUtil::instance().data()->writeExcelSheet(filePath, true);
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_Config");
    }

    // NOTE(csh): 최종 signal 조합 set 구성(row data append) + 002 excel 파일 생성
    if (appendConvertAllTCSignalSet() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_002";
            ExcelUtil::instance().data()->writeExcelSheet(filePath, true);
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_002");
    }

    return true;
}

bool ConvertDataManager::replaceGenDataInfo() {
    const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    bool result = true;

    qDebug() << "\n\n=========================================================================================================";

    for (int originIndex = originStart; originIndex < originEnd; ++originIndex) {
        int convertIndex = convertStart + (originIndex - originStart);
        QVariantList sheetData = ExcelData::instance().data()->getSheetData(originIndex).toList();
        ExcelData::instance().data()->setSheetData(convertIndex, sheetData);
    }
    const QList<int> columnListForSheetKeyword = QList({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
    });
    QMap<int, QList<KeywordInfo>> keywordTypeInfoListForSheet =
        constructKeywordTypeInfoList(originStart, originEnd, columnListForSheetKeyword);
    constructConvertSheetDataInfo(keywordTypeInfoListForSheet);

    qDebug() << "\n\n=========================================================================================================";

    const QList<int> columnListForNonSheetKeyword = QList({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
    });
    QMap<int, QList<KeywordInfo>> keywordTypeInfoListForNonSheet =
        constructKeywordTypeInfoList(convertStart, convertEnd, columnListForNonSheetKeyword);
    duplicatedConstructConvertKeywordDataInfo(keywordTypeInfoListForNonSheet);

    qDebug() << "\n=========================================================================================================\n\n";
    return result;
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
    // TODO(csh): preset, not_trigger, dependent_on 키워드를 별도로 처리하는 경우 하기 함수명을 sheet 처리 함수로 변경 필요

    const int parsingCycle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenTCParsingCycle).toInt();
    int cycle = 0;
    do {
        if (convertInputSignalKeyword() == true) {
            result = true;
        } else {
            qDebug() << "[convertInputSignalKeyword] Failed- Cycle :" << cycle << "/" << parsingCycle;
        }
        cycle++;
    } while (cycle < parsingCycle);

    // TODO(csh): 삭제 예정
    // const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    // const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    // const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    // const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    // const QList<int> columnListForNonSheetKeyword = QList({
    //     static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
    //     static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
    //     static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
    //     static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
    // });
    // QMap<int, QList<KeywordInfo>> keywordTypeInfoListForNonSheet =
    //     constructKeywordTypeInfoList(convertStart, convertEnd, columnListForNonSheetKeyword);
    // constructConvertKeywordDataInfo(keywordTypeInfoListForNonSheet);

    return result;
}

QMap<int, QList<KeywordInfo>> ConvertDataManager::constructKeywordTypeInfoList(const int& startSheetIndex,
                                                                               const int& endSheetIndex,
                                                                               const QList<int>& columnList) {
    QMap<int, QList<KeywordInfo>> keywordTypeInfoList;

    for (int sheetIndex = startSheetIndex; sheetIndex < endSheetIndex; ++sheetIndex) {
        auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        QList<KeywordInfo> keywordTypeInfo = ExcelUtil::instance().data()->isKeywordTypeInfo(sheetData, columnList);
        for (auto& keyword : keywordTypeInfo) {
            if ((keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet))) {
                QList<int> columnList = QList<int>({
                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
                });

                QList<QStringList> convertData = keyword.isRowData();
                isTCNameDataInfo(keyword.isText(), keyword.isData(), columnList, false, false, convertData);
                keyword.updateConvertData(convertData);
            } else {
                // no operation
            }
        }
        if (keywordTypeInfo.size() > 0) {
            keywordTypeInfoList[sheetIndex] = keywordTypeInfo;
        }

#if 0
        for (const auto& keyword : keywordTypeInfo) {
            qDebug() << "-----------------------------------------------------------------------------------------";
            qDebug() << "Keyword[" << sheetIndex << "]";
            qDebug() << "\t Info        :" << keyword.isRow() << keyword.isColumn() << keyword.isKeyword() << keyword.isText();
            qDebug() << "\t Data        :" << keyword.isData();
            qDebug() << "\t RowData     :" << keyword.isRowData();
            qDebug() << "\t ConvertData :" << keyword.isConvertData();
        }
#endif
    }

    return keywordTypeInfoList;
}

bool ConvertDataManager::isDataAlreadyExistInKeywordInfoList(const QStringList& rowDataList, const KeywordInfo& keywordInfo,
                                                             const int& originSheetIndex, const bool& isEqualData) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    bool isAllDataEqual = isEqualData;

    if (originSheetIndex != static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) &&
        isAllDataEqual == false) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
        qDebug() << "1 rowDataList : " << rowDataList;
        qDebug() << "1 keywordInfo.isRowData : " << keywordInfo.isRowData();
#endif
        for (int index = 0; index < keywordInfo.isRowData().length(); index++) {
            QStringList keywordRowData = keywordInfo.isRowData().at(index);
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
            qDebug() << "2 keyword.isRowData : " << keywordRowData;
#endif
            for (int columnIdx = static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName);
                 columnIdx <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData); columnIdx++) {
                QString originRowDataStr = rowDataList.at(columnIdx);
                if (static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName) <= columnIdx &&
                    columnIdx <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
                    originRowDataStr.remove(mergeStart);
                    originRowDataStr.remove(mergeEnd);
                    originRowDataStr.remove(merge);
                    if (originRowDataStr.isEmpty() == false) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "==============================================================";
                        qDebug() << "3 origin  : " << originRowDataStr;
                        qDebug() << "3 keyword : " << keywordRowData.at(columnIdx);
#endif
                        if (keywordRowData.at(columnIdx).contains(originRowDataStr) == false) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 : Not Contains";
#endif
                            isAllDataEqual = false;
                            break;
                        } else {
                            isAllDataEqual = true;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 : Contains";
#endif
                        }
                    } else {
                        // VehicleType empty
                        QString vehicleTypeStr = keywordRowData.at(columnIdx);
                        ivis::common::getRemoved(vehicleTypeStr, mergeInfos);
                        if (vehicleTypeStr.compare(originRowDataStr) != 0) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 VehicleType : Not Equal";
#endif
                            isAllDataEqual = false;
                            break;
                        } else {
                            isAllDataEqual = true;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "4 VehicleType : Equal";
#endif
                        }
                    }
                } else if (static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal) <= columnIdx &&
                           columnIdx <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "==============================================================";
                    qDebug() << "3 origin  : " << rowDataList.at(columnIdx);
                    qDebug() << "3 keyword : " << keywordRowData.at(columnIdx);
#endif
                    if (keywordRowData.at(columnIdx).compare(originRowDataStr) != 0) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "4 : Not Equal";
#endif
                        isAllDataEqual = false;
                        break;
                    } else {
                        isAllDataEqual = true;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "4 : Equal";
#endif
                    }
                } else {
                    // no operation
                }
            }
            if (isAllDataEqual == true) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                qDebug() << "5 [Equal] find equal row data in keyword info";
#endif
                break;
            } else {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                qDebug() << "5 [Not Equal] diff row data in keyword info";
#endif
            }
        }
    }

    return isAllDataEqual;
}

void ConvertDataManager::constructConvertSheetDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList) {
    const int originSheetStartIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int convertSheetStartIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertSheetEndIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();

    QList<int> usedSheetIndexList;

    for (auto iter = keywordTypeInfoList.cbegin(); iter != keywordTypeInfoList.cend(); ++iter) {
        int originIndex = iter.key();
        int convertIndex = 0;
        int rowIndex = 0;
        QVariantList convertRowData;

        bool isNotAppendOriginTCNameMergeStart = false;
        bool isNotAppendOriginResultMergeStart = false;
        QStringList tmpNotAppendMergeKeywordOriginList;
        int convertSheetIndex = originIndex -
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) +
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription);
        usedSheetIndexList.append(convertSheetIndex);
        auto sheetData = ExcelData::instance().data()->getSheetData(originIndex).toList();
        for (const auto& rowDataList : sheetData) {
            QList<QStringList> convertData;
            QList<QStringList> rowData;
            KeywordInfo curKeywordInfo;
            QStringList tmpRowDataList = rowDataList.toStringList();

            bool isEqualData = false;

            QString originCheck = tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)).trimmed();
            QString originTCName = tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)).trimmed();
            QString originGenType = tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)).trimmed();
            QString originConfig = tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)).trimmed();
            QString originVehicleType =
                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)).trimmed();
            QString originResult = tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)).trimmed();

            const int originCheckMergeType = getMergeKeywordType(originCheck);
            const int originGenTypeMergeType = getMergeKeywordType(originGenType);
            const int originConfigMergeType = getMergeKeywordType(originConfig);
            const int originTCNameMergeType = getMergeKeywordType(originTCName);
            const int originVehicleTypeMergeType = getMergeKeywordType(originVehicleType);
            const int originResultMergeType = getMergeKeywordType(originResult);

            // Origin Row Data가 [Sheet] Keyword에 존재 여부 판단 로직
            for (KeywordInfo keyword : iter.value().toList()) {
                if ((rowIndex == keyword.isRow()) &&
                    (keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet))) {
                    convertData = keyword.isConvertData();
                } else {
                    isEqualData = isDataAlreadyExistInKeywordInfoList(tmpRowDataList, keyword, originIndex, isEqualData);
                }
            }

            if (convertData.size() == 0) {
                if (isEqualData == false) {
                    convertRowData.append(QVariant(tmpRowDataList));
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "5 append rowDataList (not in keyword info list) : " << tmpRowDataList;
#endif
                } else {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                    qDebug() << "5 [PASS] not append rowDataList : " << tmpRowDataList;
#endif
                    // [Sheet] Keyword가 [MergeStart]TCName || [MergeStart]Result 이후 row_data에 존재하는 경우에 처리하는
                    // 로직
                    if (originTCNameMergeType == static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) {
                        isNotAppendOriginTCNameMergeStart = true;
                        tmpNotAppendMergeKeywordOriginList << originCheck << originTCName << originGenType << originVehicleType
                                                           << originConfig << originResult;
                    } else if (originResultMergeType ==
                               static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) {
                        isNotAppendOriginResultMergeStart = true;
                        tmpNotAppendMergeKeywordOriginList << originCheck << originTCName << originGenType << originVehicleType
                                                           << originConfig << originResult;
                    } else {
                        // no operation
                    }

                    if (convertRowData.length() > 0) {
                        QStringList tmpConvertList = convertRowData[convertRowData.length() - 1].toStringList();
                        if (originTCNameMergeType ==
                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) {
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] =
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
                        }

                        if (originVehicleTypeMergeType ==
                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) {
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
                        }

                        if (originResultMergeType ==
                            static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "OriginResult before >>>>> Result [MergeEnd] : "
                                     << convertRowData[convertRowData.length() - 1]
                                            .toList()[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)]
                                     << " <= "
                                     << tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
#endif
                            tmpConvertList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] =
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                            convertRowData[convertRowData.length() - 1] = tmpConvertList;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "OriginResult after  >>>>> Result [MergeEnd] : "
                                     << convertRowData[convertRowData.length() - 1]
                                            .toList()[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)];
#endif
                        }
                    }
                }
            } else {
                if (convertIndex == 0) {
                    convertIndex = convertSheetStartIndex + (originIndex - originSheetStartIndex);
                }

                // TODO(csh): TCName, Result, Case Data 누락 시 Error keyword 추가 예정
                // TCName / VehicleType / Result 까지의 병합을 위한 조건 처리 로직
                int convertDataListLength = convertData.length();
                if (convertDataListLength > 1) {
                    for (int idx = 0; idx < convertDataListLength; ++idx) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "============================================================================================"
                                    "==========";
                        qDebug() << "1. convlenth : (" << idx << "/" << convertDataListLength << ")";
                        qDebug() << "2. rowIndex  : " << rowIndex;
                        qDebug() << "3. covtData  : " << convertData[idx];
                        qDebug() << "4. ori_Data  : " << rowDataList;
                        qDebug() << "5. prevData : " << tmpNotAppendMergeKeywordOriginList;
#endif
                        QStringList tmpConvertDataList = convertData[idx];
                        QString checkStr;
                        QString tcNameStr;
                        QString genTypeStr;
                        QString vehicleTypeStr;
                        QString configStr;
                        QString resultStr;
                        if ((idx == 0) && (isNotAppendOriginTCNameMergeStart == true)) {
                            isNotAppendOriginTCNameMergeStart = false;
                            if (tmpNotAppendMergeKeywordOriginList.length() == 3) {
                                checkStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
                                tcNameStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                                genTypeStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
                                vehicleTypeStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                                configStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config));
                                resultStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                                tmpNotAppendMergeKeywordOriginList.clear();
                            }
                        } else if (isNotAppendOriginResultMergeStart == true) {
                            isNotAppendOriginResultMergeStart = false;
                            if (tmpNotAppendMergeKeywordOriginList.length() == 3) {
                                checkStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
                                tcNameStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                                genTypeStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
                                vehicleTypeStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                                configStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config));
                                resultStr = tmpNotAppendMergeKeywordOriginList.at(
                                    static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                                tmpNotAppendMergeKeywordOriginList.clear();
                            }
                        } else {
                            // TCName && VehicleType Merge 기능
                            if ((originTCNameMergeType ==
                                 static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) &&
                                (originVehicleTypeMergeType ==
                                 static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) &&
                                (idx == 0)) {
                                checkStr = constructMergeKeywords(
                                    mergeStart, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)));
                                genTypeStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)));
                                configStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)));
                                tcNameStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                                vehicleTypeStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)));
                            } else if ((originTCNameMergeType ==
                                        static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) &&
                                       (originVehicleTypeMergeType ==
                                        static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) &&
                                       (idx == convertDataListLength - 1)) {
                                checkStr = constructMergeKeywords(
                                    mergeEnd, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)));
                                genTypeStr = constructMergeKeywords(
                                    mergeEnd, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)));
                                configStr = constructMergeKeywords(
                                    mergeEnd, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)));
                                tcNameStr = constructMergeKeywords(
                                    mergeEnd, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                                vehicleTypeStr = constructMergeKeywords(
                                    mergeEnd,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)));
                            } else {
                                checkStr = constructMergeKeywords(
                                    merge, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)));
                                genTypeStr = constructMergeKeywords(
                                    merge, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)));
                                configStr = constructMergeKeywords(
                                    merge, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)));
                                tcNameStr = constructMergeKeywords(
                                    merge, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
                                vehicleTypeStr = constructMergeKeywords(
                                    merge,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)));
                            }

                            // Result Merge 기능
                            if (((originResultMergeType ==
                                  static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)) ||
                                 ((originResultMergeType ==
                                       static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType) ||
                                   originResultMergeType ==
                                       static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeStart)))) &&
                                (idx == 0)) {
                                resultStr = constructMergeKeywords(
                                    mergeStart,
                                    tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)));
                            } else if (((originResultMergeType ==
                                         static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd)) ||
                                        (originResultMergeType ==
                                             static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType) ||
                                         originResultMergeType ==
                                             static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::MergeEnd))) &&
                                       (idx == convertDataListLength - 1)) {
                                resultStr = constructMergeKeywords(
                                    mergeEnd, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)));
                            } else {
                                resultStr = constructMergeKeywords(
                                    merge, tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)));
                            }
                        }
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)] = checkStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] = tcNameStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)] = genTypeStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] = vehicleTypeStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)] = configStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] = resultStr;
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] =
                            constructKeywordCaseName(
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)),
                                tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)]);

#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "5 append keyword convert DataList : " << tmpConvertDataList;
#endif
                        convertRowData.append(QVariant(tmpConvertDataList));
                        tmpConvertDataList.clear();
                    }
                } else {
                    // [Sheet] Keyword 내부에 Input_Signal이 1개만 존재하는 경우
                    if (convertDataListLength == 1) {
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug()
                            << "============================================================================================";
                        qDebug() << "1. convlenth : (" << 0 << "/" << convertDataListLength << ")";
                        qDebug() << "2. rowIndex  : " << rowIndex;
                        qDebug() << "3. covtData  : " << convertData[0];
                        qDebug() << "4. ori_Data  : " << rowDataList;
                        qDebug() << "5. prevData : " << tmpNotAppendMergeKeywordOriginList;
#endif
                        QStringList tmpConvertDataList = convertData[0];
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] =
                            tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
                        tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] =
                            constructKeywordCaseName(
                                tmpRowDataList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)),
                                tmpConvertDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)]);

                        convertRowData.append(QVariant(tmpConvertDataList));
                        tmpConvertDataList.clear();
                    }
                }
            }
            rowIndex++;
        }
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
        qDebug() << "8 convertRowData : \n" << convertRowData;
#endif
        if ((convertIndex != 0) && (convertRowData.size() > 0)) {
            ExcelData::instance().data()->setSheetData(convertIndex, convertRowData);
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
            qDebug() << "[Convert Sheet Keyword] Origin[" << originIndex << "] -> Convert[" << convertIndex
                     << "] :" << convertRowData.size();
            for (const auto& rowData : convertRowData) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }
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

QString ConvertDataManager::constructConvertKeywordDataInfoBackup(const int& keywordType, const QString& inputData) {
    QString retInputData = inputData;
    qDebug() << "============================== [constructConvertKeywordDataInfo] ==============================";
    switch (keywordType) {
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over): {
            QString customKeyword = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver));
            retInputData = QString("%1[%2], [%3]")
                               .arg(customKeyword)
                               .arg(QString::number(inputData.toULongLong()))
                               .arg(QString::number(inputData.toULongLong() + 1));
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "[[[Over]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under): {
            QString customKeyword = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder));
            retInputData = QString("%1[%2], [%3]")
                               .arg(customKeyword)
                               .arg(QString::number(inputData.toULongLong()))
                               .arg(QString::number(inputData.toULongLong() - 1));
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "[[[Under]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::MoreThanEqual): {
            QString customKeyword = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual));
            retInputData = QString("%1[%2], [%3]")
                               .arg(customKeyword)
                               .arg(QString::number(inputData.toULongLong() - 1))
                               .arg(QString::number(inputData.toULongLong()));
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "[[[MoreThanEqual]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::LessThanEqual): {
            QString customKeyword = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual));
            retInputData = QString("%1[%2], [%3]")
                               .arg(customKeyword)
                               .arg(QString::number(inputData.toULongLong() + 1))
                               .arg(QString::number(inputData.toULongLong()));
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "[[[LessThanEqual]]]";
#endif
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range): {
            QString tmpInputData = inputData;
            QStringList rangeValueList = tmpInputData.remove(" ").split(",");
            if (rangeValueList.length() == 2) {
                QString customKeyword = ExcelUtil::instance().data()->isKeywordString(
                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange));
                quint64 preconditionMaxValue = static_cast<quint64>(UINT32_MAX) + 1;
                quint64 rangeLValue = rangeValueList.at(0).toULongLong();
                quint64 rangeRValue = rangeValueList.at(1).toULongLong();
                QStringList preconditionValueList = {QString::number(preconditionMaxValue)};
                QStringList inputValueList = {QString::number(rangeLValue), QString::number(rangeLValue + 1),
                                              QString::number(rangeRValue - 1), QString::number(rangeRValue)};
                if (preconditionValueList.size() == 1 && preconditionValueList.size() < inputValueList.size()) {
                    preconditionValueList.fill(preconditionValueList[0], inputValueList.size());
                }
                QString rangeValue = QString("%1[%2], [%3]")
                                         .arg(customKeyword)
                                         .arg(preconditionValueList.join(", "))
                                         .arg(inputValueList.join(", "));
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                qDebug() << "[[[Range]]]";
#endif
            } else {
                qDebug() << "[Warning] Range Value Format Error : " << inputData;
            }
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay): {
            QString tmpInputData = inputData;
            QString customKeyword = ExcelUtil::instance().data()->isKeywordString(
                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow));
            QStringList flowValueList = tmpInputData.remove(" ").split(",");
            QStringList preconditionValueList;
            QStringList inputValueList;
            for (int index = 0; index < flowValueList.size(); index++) {
                if (index % 2 == 0) {
                    preconditionValueList.append(flowValueList.at(index));
                } else {
                    inputValueList.append(flowValueList.at(index));
                }
            }
            if ((preconditionValueList.size() == 1) && (preconditionValueList.size() < inputValueList.size())) {
                preconditionValueList.fill(preconditionValueList[0], inputValueList.size());
            }
            QString flowValue =
                QString("%1[%2], [%3]").arg(customKeyword).arg(preconditionValueList.join(", ")).arg(inputValueList.join(", "));
#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "[[[Flow]]]";
#endif
            break;
        }
        default: {
            // no operation
            break;
        }
    }

#if defined(ENABLE_NON_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
    qDebug() << "1. Keyword Type    : " << keywordType;
    qDebug() << "2. Keyword Data    : " << inputData;
    qDebug() << "3. Converted Value : " << retInputData;
#endif
    return retInputData;
}

void ConvertDataManager::duplicatedConstructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList) {
    for (auto iter = keywordTypeInfoList.cbegin(); iter != keywordTypeInfoList.cend(); ++iter) {
        // int originIndex = iter.key();
        int convertIndex = iter.key();
        int rowIndex = 0;
        QVariantList convertRowDataList;

        auto sheetData = ExcelData::instance().data()->getSheetData(convertIndex).toList();
        for (const auto& rowDataList : sheetData) {
            QStringList convertRowData = rowDataList.toStringList();
            for (KeywordInfo keyword : iter.value().toList()) {
                QString customKeyword;

                if (rowIndex != keyword.isRow()) {
                    continue;
                }
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                qDebug() << "====================================================================";
#endif
                switch (keyword.isKeyword()) {
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over): {
                        // convertRowData = rowDataList.toStringList();
                        customKeyword = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver));
                        QString overValue = QString("%1[%2], [%3]")
                                                .arg(customKeyword)
                                                .arg(QString::number(keyword.isText().toULongLong()))
                                                .arg(QString::number(keyword.isText().toULongLong() + 1));
                        convertRowData[keyword.isColumn()] = overValue;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[Over] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Over] keyword Data    : " << keyword.isText();
                        qDebug() << "[Over] Keyword RowData : " << convertRowData;
                        qDebug() << "[Over] Converted Value : " << overValue;
#endif
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under): {
                        // convertRowData = rowDataList.toStringList();
                        customKeyword = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder));
                        QString underValue = QString("%1[%2], [%3]")
                                                 .arg(customKeyword)
                                                 .arg(QString::number(keyword.isText().toULongLong()))
                                                 .arg(QString::number(keyword.isText().toULongLong() - 1));
                        convertRowData[keyword.isColumn()] = underValue;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[Under] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Under] keyword Data    : " << keyword.isText();
                        qDebug() << "[Under] Keyword RowData : " << convertRowData;
                        qDebug() << "[Under] Converted Value : " << underValue;
#endif
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range): {
                        // convertRowData = rowDataList.toStringList();
                        QStringList rangeValueList = keyword.isText().remove(" ").split(",");
                        if (rangeValueList.length() == 2) {
                            customKeyword = ExcelUtil::instance().data()->isKeywordString(
                                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange));
                            quint64 preconditionMaxValue = static_cast<quint64>(UINT32_MAX) + 1;
                            quint64 rangeLValue = rangeValueList.at(0).toULongLong();
                            quint64 rangeRValue = rangeValueList.at(1).toULongLong();
                            QStringList preconditionValueList = {QString::number(preconditionMaxValue)};
                            QStringList inputValueList = {QString::number(rangeLValue), QString::number(rangeLValue + 1),
                                                          QString::number(rangeRValue - 1), QString::number(rangeRValue)};
                            if (preconditionValueList.size() == 1 && preconditionValueList.size() < inputValueList.size()) {
                                preconditionValueList.fill(preconditionValueList[0], inputValueList.size());
                            }
                            QString rangeValue = QString("%1[%2], [%3]")
                                                     .arg(customKeyword)
                                                     .arg(preconditionValueList.join(", "))
                                                     .arg(inputValueList.join(", "));
                            convertRowData[keyword.isColumn()] = rangeValue;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                            qDebug() << "[Range] keyword Type    : " << keyword.isKeyword();
                            qDebug() << "[Range] keyword Data    : " << keyword.isText();
                            qDebug() << "[Range] Keyword RowData : " << convertRowData;
                            qDebug() << "[Range] Converted Value : " << rangeValue;
#endif
                        } else {
                            qDebug() << "[Warning] Range Value Format Error : " << keyword.isText();
                        }
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow): {
                        // convertRowData = rowDataList.toStringList();
                        customKeyword = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow));
                        QStringList flowValueList = keyword.isText().remove(" ").split(",");
                        QStringList preconditionValueList;
                        QStringList inputValueList;
                        for (int index = 0; index < flowValueList.size(); index++) {
                            if (index % 2 == 0) {
                                preconditionValueList.append(flowValueList.at(index));
                            } else {
                                inputValueList.append(flowValueList.at(index));
                            }
                        }
                        if ((preconditionValueList.size() == 1) && (preconditionValueList.size() < inputValueList.size())) {
                            preconditionValueList.fill(preconditionValueList[0], inputValueList.size());
                        }
                        QString flowValue = QString("%1[%2], [%3]")
                                                .arg(customKeyword)
                                                .arg(preconditionValueList.join(", "))
                                                .arg(inputValueList.join(", "));
                        convertRowData[keyword.isColumn()] = flowValue;
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[Flow] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Flow] keyword Data    : " << keyword.isText();
                        qDebug() << "[Flow] Keyword RowData : " << convertRowData;
                        qDebug() << "[Flow] Converted Value : " << flowValue;
#endif
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay): {
                        // convertRowData = rowDataList.toStringList();
                        customKeyword = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay));
                        QStringList twoWayValueList = keyword.isText().remove(" ").split(",");
                        QStringList preconditionValueList;
                        QStringList inputValueList;
                        for (int index = 0; index < twoWayValueList.size(); index++) {
                            preconditionValueList.append(twoWayValueList.at(index));
                            QString inputValue;
                            int calIndex = 0;
                            if (index % 2 == 0) {
                                calIndex = index + 1;
                            } else {
                                calIndex = index - 1;
                            }
                            if (calIndex >= 0 && calIndex < twoWayValueList.size()) {
                                inputValueList.append(twoWayValueList.at(calIndex));
                            } else {
                                qDebug() << "[Warning] TwoWay Keyword Value Format Error";
                            }
                        }
                        QString twoWayValue = QString("%1[%2], [%3]")
                                                  .arg(customKeyword)
                                                  .arg(preconditionValueList.join(", "))
                                                  .arg(inputValueList.join(", "));
                        convertRowData[keyword.isColumn()] = twoWayValue;
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::MoreThanEqual): {
                        // convertRowData = rowDataList.toStringList();
                        customKeyword = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual));
                        QString moreThanEqualValue = QString("%1[%2], [%3]")
                                                         .arg(customKeyword)
                                                         .arg(QString::number(keyword.isText().toULongLong() - 1))
                                                         .arg(QString::number(keyword.isText().toULongLong()));
                        convertRowData[keyword.isColumn()] = moreThanEqualValue;
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::LessThanEqual): {
                        // convertRowData = rowDataList.toStringList();
                        customKeyword = ExcelUtil::instance().data()->isKeywordString(
                            static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual));
                        QString lessThanEqualValue = QString("%1[%2], [%3]")
                                                         .arg(customKeyword)
                                                         .arg(QString::number(keyword.isText().toULongLong() + 1))
                                                         .arg(QString::number(keyword.isText().toULongLong()));
                        convertRowData[keyword.isColumn()] = lessThanEqualValue;
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare): {
                        // convertRowData = rowDataList.toStringList();
                        if (keyword.isData().contains(SFC_IGN_ELAPSED) == true) {
                            convertRowData[keyword.isColumn()] = keyword.isText();
                        } else {
                            // Not IGN Signal Don't care 조건
                            QStringList enumDontCareValueList;
                            int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
                            QString signalName = keyword.isData();
                            QString vehicleType = QString();
                            QStringList signalData = QStringList();
                            int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
                            QVariant vehicleTypeList =
                                ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV);
                            if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
                                vehicleTypeList =
                                    ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV);
                            }

                            for (const auto& vehicle : vehicleTypeList.toStringList()) {
                                if (vehicleType.size() > 0) {
                                    vehicleType.append(", ");
                                }
                                vehicleType.append(vehicle);
                            }
                            // TODO(csh): 추후 toEnum API로 변경 예정
                            QMap<int, QStringList> dataInfoFromSingnalName =
                                SignalDataManager::instance().data()->isSignalDataList(signalName, signalData, vehicleType,
                                                                                       dataType);
                            QStringList valueEnum =
                                dataInfoFromSingnalName[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
                            for (QString enumString : valueEnum) {
                                QStringList enumSplitList = enumString.split(":");
                                for (QString enumSplit : enumSplitList) {
                                    if (enumSplit.contains("0x") == false) {
                                        enumSplit.remove('\"');
                                        enumDontCareValueList.append(enumSplit);
                                        break;
                                    }
                                }
                            }
                            convertRowData[keyword.isColumn()] = enumDontCareValueList.join(", ");
                        }
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Not): {
                        // convertRowData = rowDataList.toStringList();
                        QString signalName = keyword.isData();
                        QStringList keywordValueList = keyword.isText().split(",");
                        QStringList allSignalEnumList = SignalDataManager::instance().data()->isSignalValueEnum(true, signalName);
                        QStringList notKeywordConvertDataList;
                        for (const QString& value1 : allSignalEnumList) {
                            if (!keywordValueList.contains(value1)) {  // keywordValueList에 없는 경우
                                notKeywordConvertDataList.append(value1);
                            }
                        }
                        // for (const QString& value1 : allSignalEnumList) {
                        //     for (const QString& value2 : keywordValueList) {
                        //         if (value1.compare(value2) == true) {
                        //             notKeywordConvertDataList.append(value1);
                        //         }
                        //     }
                        // }
                        convertRowData[keyword.isColumn()] = notKeywordConvertDataList.join(", ");
                        // #if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[Not] all num value   : " << allSignalEnumList;
                        qDebug() << "[Not] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Not] keyword Data    : " << keyword.isText();
                        qDebug() << "[Not] Keyword RowData : " << convertRowData;
                        qDebug() << "[Not] Not Used Enum   : " << notKeywordConvertDataList;
                        // #endif
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout): {
                        // convertRowData = rowDataList.toStringList();
                        convertRowData[keyword.isColumn()] = keyword.isText();
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[ETC] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[ETC] keyword Data    : " << keyword.isText();
                        qDebug() << "[ETC] Keyword RowData : " << convertRowData;
#endif
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other): {
                        if (keyword.isData().isEmpty() == true) {
                            // Others 조건 : Input_Signal column이 비어있는 경우
                            convertRowData = rowDataList.toStringList();
                            convertRowData[keyword.isColumn()] = keyword.isText();
                        }
                        break;
                    }
                    default: {
                        // no operation
                        break;
                    }
                }
            }
            if (convertRowData.size() == 0) {
                convertRowDataList.append(rowDataList);
            } else {
                convertRowDataList.append(QVariant(convertRowData));
            }
            rowIndex++;
            convertRowData.clear();
        }

        if ((convertIndex != 0) && (convertRowDataList.size() > 0)) {
            ExcelData::instance().data()->setSheetData(convertIndex, convertRowDataList);
#if defined(ENABLE_DEBUG_LOG_OUTPUT)
            qDebug() << "[Convert Non Sheet Keyword] Convert [" << convertIndex << "] -> Convert[" << convertIndex
                     << "] :" << convertRowDataList.size();
            for (const auto& rowData : convertRowDataList) {
                qDebug() << "\t" << rowData;
            }
#endif
        }
    }
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

bool ConvertDataManager::convertInputSignalKeyword() {
    bool result = false;

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
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
        qDebug() << "============================[convertInputSignalKeyword]=====================================";
        qDebug() << "Sheet Index     : " << sheetIndex;
        qDebug() << "TCName List     : " << tcNameList;
#endif
        // Sheet에서 TCName 리스트 기반으로 Result/Case 하위 Data 처리
        for (int tcIdx = 0; tcIdx < tcNameList.size(); ++tcIdx) {
            QString tcNameStr = tcNameList.at(tcIdx);
            QStringList resultStrList = ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcNameStr);
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
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
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG) || defined(ENABLE_OUTPUT_DATA_KEYWORD_DEBUG_LOG)
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
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
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
                        auto sheetKeywordResultInfo = convertSheetKeyword(caseInputDataList, sheetIndex);
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                        qDebug() << "[[[Result]]] sheetKeywordResultInfo : " << sheetKeywordResultInfo;
#endif
                        QString tmpCaseName = caseStr;

                        for (int i = 0; i < sheetKeywordResultInfo.size(); ++i) {
                            QString sheetCaseName = sheetKeywordResultInfo.at(i).first;
                            QPair<QStringList, QStringList> sheetInputPairData = sheetKeywordResultInfo.at(i).second;
                            QString caseNameStr = mergeCaseName(tmpCaseName, sheetCaseName);

                            // appendCurSheetData(tcNameStr, resultStr, caseNameStr, sheetInputPairData, backupCurSheetIndexData);
                            ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, tcNameStr, resultStr,
                                                                                     caseNameStr, sheetInputPairData);
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                            qDebug() << ":::: tcNameStr : " << tcNameStr;
                            qDebug() << ":::: resultStr : " << resultStr;
                            qDebug() << ":::: caseNameStr : " << caseNameStr;
                            qDebug() << ":::: sheetInputPairData : " << sheetInputPairData;
#endif
                        }
                    }
                    bool hasNotTriggerKeyword = !inputSignalStringList.filter(notTriggerKeywordStr).isEmpty();
                    // TODO(csh): TBD
                    bool hasPresetKeyword = !inputSignalStringList.filter(presetKeywordStr).isEmpty();
                    // TODO(csh): TBD

                    if (hasSheetKeyword == false && hasNotTriggerKeyword == false && hasPresetKeyword == false) {
                        // appendCurSheetData(tcNameStr, resultStr, caseStr, caseInputDataList, backupCurSheetIndexData);
                        ExcelDataManager::instance().data()->updateInputDataInfo(sheetIndex, tcNameStr, resultStr, caseStr,
                                                                                 caseInputDataList);
                    }
                    result = true;
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

        if (result == true) {
            // updateCurSheetData(sheetIndex, backupCurSheetIndexData);
            ExcelDataManager::instance().data()->writeExcelSheetData(sheetIndex);
        }
    }

    return result;
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
    ConvertKeywordInfo convertKeywordInfo;
    convertKeywordInfo.inputSignal = signal;
    QString inputValue = value;
    inputValue.remove(' ');
    QRegularExpression regex(",(?![^()]*\\))");
    QStringList parts = inputValue.split(regex);
    QString resultString;
    QString tempString;
    QString prevTempString;
    // qInfo() << "@@@@ inputSignal: " << convertKeywordInfo.inputSignal;
    // qInfo() << "@@@@ inputValue: " << inputValue;
    auto processEnum = [&](const QString& part, QStringList& enumList) {
        if (part.contains("timeout", Qt::CaseInsensitive)) {
            if (!SignalDataManager::instance().data()->isSignalValueEnum(signal, "MESSAGE_TIMEOUT").isEmpty()) {
                enumList.removeAll("MESSAGE_TIMEOUT");
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(signal, findVehicleSignalElseTimeoutCrcValue(signal, "TIMEOUT"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    signal, findVehicleSignalElseTimeoutCrcValue(signal, "TIMEOUT")));
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(signal, findVehicleSignalElseTimeoutCrcValue(signal, "ELSE"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    signal, findVehicleSignalElseTimeoutCrcValue(signal, "ELSE")));
            } else {
                enumList << QString("4294967296");
            }
        } else if (part.contains("crc", Qt::CaseInsensitive)) {
            if (!SignalDataManager::instance().data()->isSignalValueEnum(signal, "CRC_ERROR").isEmpty()) {
                enumList.removeAll("CRC_ERROR");
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(signal, findVehicleSignalElseTimeoutCrcValue(signal, "CRCERROR"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    signal, findVehicleSignalElseTimeoutCrcValue(signal, "CRCERROR")));
            } else if (!SignalDataManager::instance()
                            .data()
                            ->isSignalValueEnum(signal, findVehicleSignalElseTimeoutCrcValue(signal, "ELSE"))
                            .isEmpty()) {
                enumList.removeAll(SignalDataManager::instance().data()->isSignalValueEnum(
                    signal, findVehicleSignalElseTimeoutCrcValue(signal, "ELSE")));
            } else {
                enumList << QString("4294967296");
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
                tempResult += QString("4294967296");
            } else {
                for (const QString& enumString : tempString.split(',')) {
                    processEnum(enumString, enumList);
                }
                tempResult += enumList.join(", ");
            }
        } else if (part.contains('(')) {
            tempString.remove(QRegularExpression("[()]"));
            if (tempString.contains("timeout", Qt::CaseInsensitive)) {
                tempString = SignalDataManager::instance().data()->isSignalValueEnum(
                    signal, findVehicleSignalElseTimeoutCrcValue(signal, "TIMEOUT"));
            } else if (tempString.contains("crc", Qt::CaseInsensitive)) {
                tempString = SignalDataManager::instance().data()->isSignalValueEnum(
                    signal, findVehicleSignalElseTimeoutCrcValue(signal, "CRCERROR"));
            }
            tempResult += tempString;
        } else if (part.contains('!')) {
            tempString.remove('!');
            if (tempString.toDouble() || tempString == "0") {
                tempResult += QString("4294967296");
            } else {
                processEnum(tempString, enumList);
                tempResult += enumList.join(", ");
            }
        } else {
            tempResult += tempString;
        }
    };
    for (const QString& part : parts) {
        QStringList inputSignalEnumList = SignalDataManager::instance().data()->isSignalValueEnum(true, signal);
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
            for (const QString& leftValue : aaaa[0].split(", ")) {
                for (const QString& rightValue : aaaa[1].split(", ")) {
                    if (!leftValue.isEmpty() && !rightValue.isEmpty()) {
                        bool leftIsNumber, rightIsNumber;
                        double leftNum = leftValue.toDouble(&leftIsNumber);
                        double rightNum = rightValue.toDouble(&rightIsNumber);
                        if (!(leftValue == rightValue || (leftIsNumber && rightIsNumber && leftNum == rightNum))) {
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
            for (const QString& leftValue : aaaa[0].split(", ")) {
                for (const QString& rightValue : aaaa[1].split(", ")) {
                    bool leftIsNumber, rightIsNumber;
                    double leftNum = leftValue.toDouble(&leftIsNumber);
                    double rightNum = rightValue.toDouble(&rightIsNumber);
                    if (!(leftValue == rightValue || (leftIsNumber && rightIsNumber && leftNum == rightNum))) {
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
                       !signal.contains("SFC.Private.IGNElapsed.")) {
                convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::ValueChanged;
                if (inputSignalEnumList.isEmpty()) {
                    resultString += QString("0, 4294967296");
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
                if (signal.contains("SFC.Private.IGNElapsed.")) {
                    resultString += QString("D'");
                } else {
                    resultString += inputSignalEnumList.isEmpty() ? QString("4294967296") : inputSignalEnumList.join(", ");
                }
            } else if (tempString.contains('~')) {
                QStringList enumString = tempString.split('~');
                convertKeywordInfo.keywordType = ivis::common::KeywordTypeEnum::KeywordType::Range;
                if (enumString[0].toDouble() || enumString[0] == "0") {
                    resultString += QString("4294967296,%1,4294967296,%2,4294967296,%3,4294967296,%4")
                                        .arg(enumString[0])
                                        .arg(QString::number(enumString[0].toInt() + 1))
                                        .arg(QString::number(enumString[1].toInt() - 1))
                                        .arg(enumString[1]);
                    convertKeywordInfo.validInputData += QString("%1,%2,%3,%4")
                                                             .arg(enumString[0])
                                                             .arg(QString::number(enumString[0].toInt() + 1))
                                                             .arg(QString::number(enumString[1].toInt() - 1))
                                                             .arg(enumString[1]);
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
                    tempString = QString("4294967296");
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
                    tempString = SignalDataManager::instance().data()->isSignalValueEnum(
                        signal, findVehicleSignalElseTimeoutCrcValue(signal, "TIMEOUT"));
                } else if (tempString.contains("crc", Qt::CaseInsensitive)) {
                    tempString = SignalDataManager::instance().data()->isSignalValueEnum(
                        signal, findVehicleSignalElseTimeoutCrcValue(signal, "CRCERROR"));
                }
                resultString += tempString + ", ";
            } else if (tempString.contains('!')) {
                tempString.remove('!');
                if (tempString.toDouble() || tempString == "0") {
                    tempString = QString("4294967296");
                } else {
                    processEnum(tempString, inputSignalEnumList);
                    tempString = inputSignalEnumList.join(", ");
                }
                if (prevTempString != tempString) {
                    resultString += tempString + ", ";
                }
            } else {
                if (tempString.contains("timeout", Qt::CaseInsensitive)) {
                    tempString = SignalDataManager::instance().data()->isSignalValueEnum(
                        signal, findVehicleSignalElseTimeoutCrcValue(signal, "TIMEOUT"));
                } else if (tempString.contains("crc", Qt::CaseInsensitive)) {
                    tempString = SignalDataManager::instance().data()->isSignalValueEnum(
                        signal, findVehicleSignalElseTimeoutCrcValue(signal, "CRCERROR"));
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
    return convertKeywordInfo;
}

QPair<QStringList, QList<CaseDataInfo>> ConvertDataManager::generateCombinations(
    const QString& templateExpr, const QMap<QString, QStringList>& valueMap, const QStringList& keys, const QList<int>& keywords,
    const QStringList& inputValues, const QString& caseName, int index, QMap<QString, QString> current) {
    QPair<QStringList, QList<CaseDataInfo>> results;
    if (index == keys.size()) {
        QString expr = templateExpr;
        CaseDataInfo caseDataInfo;
        int count = 0;
        for (const auto& key : keys) {
            ConvertKeywordInfo info;
            info.inputSignal = key;
            info.inputValue = constructConvertKeywordDataInfo(keywords[count], inputValues[count]);
            info.validInputData = current[key].remove(' ');
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
            expr.remove(QRegularExpression("\\[cal\\]", QRegularExpression::CaseInsensitiveOption)).replace("math", "Math"));
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
        auto subResult = generateCombinations(templateExpr, valueMap, keys, keywords, inputValues, caseName, index + 1, next);
        results.first += subResult.first;
        results.second += subResult.second;
    }
    return results;
}

QList<ResultInfo> ConvertDataManager::interpretCalKeywordAndRedefineResultInfo(const ResultInfo& resultInfo) {
    QList<ResultInfo> resultList;
    ResultInfo tempResult;
    OutputDataInfo tempOutputDataInfo;
    for (auto& outputDataInfo : resultInfo.outputDataInfoList) {
        if (outputDataInfo.outputValue.contains("[cal]", Qt::CaseInsensitive)) {
            for (auto& caseDataInfo : resultInfo.caseDataInfoList) {
                QString exprTemplate = outputDataInfo.outputValue;
                QMap<QString, QStringList> valueMap;
                QStringList keys;
                QList<int> keywords;
                QStringList inputValues;
                for (const auto& keyword : caseDataInfo.convertInputDataInfo) {
                    QString key = keyword.inputSignal;
                    QString inputValue = keyword.inputValue;
                    QStringList values = keyword.validInputData.split(',');
                    valueMap[key] = values;
                    keys << key;
                    keywords << static_cast<int>(keyword.keywordType);
                    inputValues << inputValue;
                }
                QPair<QStringList, QList<CaseDataInfo>> results =
                    generateCombinations(exprTemplate, valueMap, keys, keywords, inputValues, caseDataInfo.caseName);
                for (int i = 0; i < results.first.size(); i++) {
                    ResultInfo tempResult;
                    if (caseDataInfo.caseName.contains("others", Qt::CaseInsensitive)) {
                        tempResult.resultName = resultInfo.resultName;
                    } else {
                        tempResult.resultName = resultInfo.resultName + " == " + results.first[i];
                    }
                    tempOutputDataInfo.outputSignal = outputDataInfo.outputSignal;
                    tempOutputDataInfo.initialize = outputDataInfo.initialize;
                    tempOutputDataInfo.outputValue = results.first[i];
                    tempResult.caseDataInfoList.clear();
                    tempResult.outputDataInfoList.clear();
                    tempResult.caseDataInfoList << results.second[i];
                    tempResult.outputDataInfoList << tempOutputDataInfo;
                    resultList << tempResult;
                }
            }
        } else {
            ResultInfo tempResultInfo = resultInfo;
            for (auto& caseDataInfo : tempResultInfo.caseDataInfoList) {
                for (auto& convertKeywordInfo : caseDataInfo.convertInputDataInfo) {
                    if (convertKeywordInfo.keywordType != ivis::common::KeywordTypeEnum::KeywordType::Invalid) {
                        convertKeywordInfo.inputValue = constructConvertKeywordDataInfo(
                            static_cast<int>(convertKeywordInfo.keywordType), convertKeywordInfo.inputValue);
                    }
                }
            }
            resultList << tempResultInfo;
        }
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
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
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
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
        qDebug() << "======================= [updateCurSheetData] ==========================================";
        qDebug() << "0. sheetIndex    : " << sheetIndex;
        qDebug() << "1. tcName        : " << std::get<0>(tmpSheetData);
        qDebug() << "2. resultName    : " << std::get<1>(tmpSheetData);
        qDebug() << "3. caseName      : " << std::get<2>(tmpSheetData);
        qDebug() << "4. inputDataInfo : " << std::get<3>(tmpSheetData);
#endif
    }
}

bool ConvertDataManager::convertInputDataKeyword() {
    bool result = false;

    return result;
}

// [Sheet] 키워드 해석 기능
QList<QPair<QString, QPair<QStringList, QStringList>>> ConvertDataManager::convertSheetKeyword(
    const QPair<QStringList, QStringList>& inputDataPairInfo, const int& currentSheetIndex) {
    QList<QPair<QString, QPair<QStringList, QStringList>>> sheetDataInfo;
    QStringList inputSignalStrList = inputDataPairInfo.first;
    QStringList inputDataStrList = inputDataPairInfo.second;

    if ((inputSignalStrList.isEmpty() == false && inputDataStrList.isEmpty() == false) &&
        (inputSignalStrList.size() == inputDataStrList.size())) {
        for (int index = 0; index < inputSignalStrList.size(); ++index) {
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "-------------------------------------------------------------------------------";
            qDebug() << "---> signal : " << inputSignalStrList.at(index);
            qDebug() << "--->   data : " << inputDataStrList.at(index);
#endif
            QString inputSignalStr = inputSignalStrList.at(index);
            QString inputDataStr = inputDataStrList.at(index);
            if (inputSignalStrList.at(index).contains("[Sheet]") == true) {
                inputSignalStr.remove("[Sheet]");
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                qDebug() << "----> [Sheet] keyword Exist!";
                qDebug() << "----> remove [Sheet] : " << inputSignalStr;
#endif
                // input signal : [Sheet]Private_A
                // input data   : ON, OFF... (2개 이상의 data 지원하기 위함)
                QStringList inputDataSplitStr = inputDataStr.remove(" ").split(",");
                for (int i = 0; i < inputDataSplitStr.size(); ++i) {
                    // QList<QPair<QString, QPair<QStringList, QStringList>>>
                    auto sheetSignalDataInfo = getSheetSignalDataInfo(inputSignalStr, inputDataSplitStr.at(i));

                    if (sheetSignalDataInfo.isEmpty() == false) {
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                        qDebug() << "-----> [Sheet] Info : " << sheetSignalDataInfo;
#endif
                        for (int i = 0; i < sheetSignalDataInfo.size(); ++i) {
                            // [CaseName, <InputSignalList, InputDataList>]
                            QString sheetCaseName = sheetSignalDataInfo.at(i).first;
                            QPair<QStringList, QStringList> sheetInputDataInfo = sheetSignalDataInfo.at(i).second;
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                            qDebug() << "====================================================================================";
                            qDebug() << "1. caseName      : " << sheetCaseName;
                            qDebug() << "2. [Sheet] inputSignalStrList : " << sheetInputDataInfo.first;
                            qDebug() << "3. [Sheet] inputDataInfo      : " << sheetInputDataInfo.second;
                            qDebug() << "4. [Excel] inputSignalStrList : " << inputSignalStrList;
                            qDebug() << "5. [Excel] inputDataInfo      : " << inputDataStrList;
#endif
                            auto tmpInputDataInfo = getMergedInputDataInfo(inputDataPairInfo, sheetInputDataInfo);
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                            qDebug() << "[[[Result]]] tmpInputDataInfo : " << tmpInputDataInfo;
#endif
                            sheetDataInfo.append(qMakePair(sheetCaseName, tmpInputDataInfo));
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
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
        qDebug() << "-----> Find Sheet Case List : " << caseStrList;
#endif
        for (int caseIndex = 0; caseIndex < caseStrList.size(); ++caseIndex) {
            QString caseNameStr = caseStrList.at(caseIndex);
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

    return sheetKeywordSignalDataInfo;
}

QPair<QStringList, QStringList> ConvertDataManager::getMergedInputDataInfo(const QPair<QStringList, QStringList>& origin,
                                                                           const QPair<QStringList, QStringList>& sheet) {
    QPair<QStringList, QStringList> retVal;

    QSet<QString> seen;  // 중복 확인용 Set
    QStringList mergedSignalList;
    QStringList mergedDataList;

    const QStringList& originSignalList = origin.first;
    const QStringList& originDataList = origin.second;
    const QStringList& sheetSignalList = sheet.first;
    const QStringList& sheetDataList = sheet.second;

    bool sheetInserted = false;

    for (int i = 0; i < originSignalList.size(); ++i) {
        const QString& signal = originSignalList[i];
        const QString& data = originDataList[i];

        // [Sheet]가 포함된 항목을 만나면 삭제하고 sheet 리스트 삽입
        if (signal.contains("[Sheet]")) {
            if (!sheetInserted) {
                // sheet 데이터를 현재 위치부터 삽입
                for (int j = 0; j < sheetSignalList.size(); ++j) {
                    if (!seen.contains(sheetSignalList[j])) {
                        seen.insert(sheetSignalList[j]);
                        mergedSignalList.append(sheetSignalList[j]);
                        mergedDataList.append(sheetDataList[j]);
                    }
                }
                sheetInserted = true;  // 한 번만 삽입
            }
            continue;  // 기존의 [Sheet] 포함된 원본 signal/data는 삭제
        }

        // 중복 방지 후 추가
        if (!seen.contains(signal)) {
            seen.insert(signal);
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

QStringList ConvertDataManager::deleteColumnRowData(const QStringList& rowData, const QList<int>& deleteColumnIndex) {
    QStringList deleteDataStringList = rowData;

    for (int index = 0; index < deleteColumnIndex.size(); index++) {
        deleteDataStringList[deleteColumnIndex.at(index)] = "";
    }

    return deleteDataStringList;
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

QString ConvertDataManager::constructKeywordCaseName(const QString& originCaseName, const QString& convertCaseName) {
    QString prefix;
    QString suffix;
    QString returnStr;

    bool isMergeKeywordExist =
        (getMergeKeywordType(convertCaseName) != static_cast<int>(ivis::common::MergeKeywordEnum::MergeKeywordType::NoMergeType));

    QString data = constructMergeKeywords("", originCaseName);
    if (isMergeKeywordExist == true) {
        // [Sheet] 키워드 사용하는 Case이름과 [Sheet] 키워드의 Case 이름을 조합하는 로직
        // e.g)
        // Private Sheet의 Case Name         ->  VALID1
        // Constant [Sheet]Private Case Name -> [MergeStart]Case1
        // 결과 : [MergeStart]Case1_VALID1
        prefix = convertCaseName.section("]", 0, 0) + "]";  // prefix : [MergeStart]
        suffix = convertCaseName.section("]", 1);           // suffix : Case Column String

        returnStr = prefix + data + "_" + suffix;  // case : [MergeStart] + Origin_Row_Case_String + Case_Column_String
    } else {
        prefix = data;
        suffix = convertCaseName;
        returnStr = prefix + "_" + suffix;
    }

    return returnStr;
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
