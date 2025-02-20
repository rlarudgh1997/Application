#include "ConvertDataManager.h"

#include "CommonEnum.h"
#include "CommonUtil.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"

#include "ExcelData.h"
#include "ExcelUtil.h"
#include "ExcelDataManager.h"
#include "SignalDataManager.h"

// #define ENABLE_CONFIG_DEBUG_LOG
// #define ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG

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
            if (ExcelUtil::instance().data()->writeExcelSheet(filePath, true)) {
                QString dirPath = ExcelUtil::instance().data()->systemCall(false, filePath);
                if (dirPath.size() > 0) {
                    qDebug() << "\t [GenTC] Convert Excel Save :" << dirPath;
                }
            }
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_001");
    }

    if (appendConvertConfigSignalSet() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_Config";
            if (ExcelUtil::instance().data()->writeExcelSheet(filePath, true)) {
                QString dirPath = ExcelUtil::instance().data()->systemCall(false, filePath);
                if (dirPath.size() > 0) {
                    qDebug() << "\t [GenTC] Convert Excel Save :" << dirPath;
                }
            }
        }
        checkTimer.check("excuteConvertDataManager : Convert.excel_Config");
    }

    // NOTE(csh): 최종 signal 조합 set 구성(row data append) + 002 excel 파일 생성
    if (appendConvertAllTCSignalSet() == true) {
        if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSaveConvertExcel).toBool()) {
            QVariant filePath = ivis::common::APP_PWD() + "/Convert.excel_002";
            if (ExcelUtil::instance().data()->writeExcelSheet(filePath, true)) {
                QString dirPath = ExcelUtil::instance().data()->systemCall(false, filePath);
                if (dirPath.size() > 0) {
                    qDebug() << "\t [GenTC] Convert Excel Save :" << dirPath;
                }
            }
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
    constructConvertKeywordDataInfo(keywordTypeInfoListForNonSheet);

    qDebug() << "\n=========================================================================================================\n\n";
    return result;
}

bool ConvertDataManager::convertKeywordData() {
    bool result = false;

    if (convertInputSignalKeyword() == true) {
        result = true;
    }

    // 추 후 하기 API로 변경 예정
    // if (convertInputDataKeyword() == true) {
    //     result = true;
    // }
    const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    const QList<int> columnListForNonSheetKeyword = QList({
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData),
        static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal),
    });
    QMap<int, QList<KeywordInfo>> keywordTypeInfoListForNonSheet =
        constructKeywordTypeInfoList(convertStart, convertEnd, columnListForNonSheetKeyword);
    constructConvertKeywordDataInfo(keywordTypeInfoListForNonSheet);

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

void ConvertDataManager::constructConvertKeywordDataInfo(QMap<int, QList<KeywordInfo>>& keywordTypeInfoList) {
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
#if defined(ENABLE_DEBUG_LOG_KEYWORD)
                        qDebug() << "[Not] keyword Type    : " << keyword.isKeyword();
                        qDebug() << "[Not] keyword Data    : " << keyword.isText();
                        qDebug() << "[Not] Keyword RowData : " << convertRowData;
                        qDebug() << "[Not] Not Used Enum   : " << notKeywordConvertDataList;
#endif
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

#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs;
    auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
    ExcelDataManager::instance().data()->updateExcelData(sheetIndex, sheetData);
#else
    ExcelDataManager::instance().data()->resetExcelData(true);
#endif

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

#if !defined(USE_EXCEL_DATA_MANAGER_OLD)
    ExcelDataManager::instance().data()->resetExcelData(true);
#endif

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
        auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        ExcelDataManager::instance().data()->updateExcelData(sheetIndex, sheetData);
#endif

        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs))) {
            qDebug() << "Not support sheet : 1" << sheetIndex;
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
            QList<QStringList> currentSheetData = ExcelDataManager::instance().data()->isSheetDataInfo(sheetIndex);
            if (currentSheetData.size() > 0) {
                QVariantList tmpSheetData;
                for (auto& data : currentSheetData) {
                    tmpSheetData.append(data);
                }
                ExcelData::instance().data()->setSheetData(sheetIndex, tmpSheetData);
            } else {
                // no data changed.
#if defined(ENABLE_CONFIG_DEBUG_LOG)
                qDebug("[appendConvertConfigSignalSet] No Config Data (sheet index : %d) -> No need to create excel file",
                       sheetIndex);
#endif
            }
        }
    }

    return result;
}

bool ConvertDataManager::appendConvertAllTCSignalSet() {
    bool result = true;

    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) + 1;
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax);

#if !defined(USE_EXCEL_DATA_MANAGER_OLD)
    ExcelDataManager::instance().data()->resetExcelData(true);
#endif

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
        auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        ExcelDataManager::instance().data()->updateExcelData(sheetIndex, sheetData);
#endif

        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs))) {
            qDebug() << "Not support sheet : 2" << sheetIndex;
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

                    auto inputList =
                        ExcelDataManager::instance().data()->isInputDataWithoutCaseList(sheetIndex, tcNameStr, resultStr,
                                                                                        caseStr);
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
#if defined(ENABLE_ALL_TC_SIGNAL_SET_LOG)
                qDebug() << "############################################################################################";
#endif
            }
        }
        QList<QStringList> currentSheetData = ExcelDataManager::instance().data()->isSheetDataInfo(sheetIndex);
        QVariantList tmpSheetData;
        for (auto& data : currentSheetData) {
            tmpSheetData.append(data);
        }
        ExcelData::instance().data()->setSheetData(sheetIndex, tmpSheetData);
    }

    return result;
}

bool ConvertDataManager::convertInputSignalKeyword() {
    bool result = false;

    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax);

#if !defined(USE_EXCEL_DATA_MANAGER_OLD)
    ExcelDataManager::instance().data()->resetExcelData(false);
#endif

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        int convertSheetIndex = sheetIndex -
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) +
                                static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription);
        if ((sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription)) ||
            (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs))) {
            qDebug() << "Not support sheet : 3" << sheetIndex;
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
            auto originSheetData = ExcelData::instance().data()->getSheetData(sheetIndex);
            ExcelData::instance().data()->setSheetData(convertSheetIndex, originSheetData);
#endif
            continue;
        }
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
        auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        ExcelDataManager::instance().data()->updateExcelData(sheetIndex, sheetData);
#endif

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
                QStringList caseStrList = ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcNameStr, resultStr);

#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                qDebug() << "tcNameStr       : " << tcNameStr;
                qDebug() << "Result          : " << resultStr;
                qDebug() << "Case List       : " << caseStrList;
#endif
                for (int caseIdx = 0; caseIdx < caseStrList.size(); ++caseIdx) {
                    QString caseStr = caseStrList.at(caseIdx);
                    QPair<QStringList, QStringList> caseInputDataList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcNameStr, resultStr, caseStr, false);
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                    qDebug() << ""
                                "--------------------------------------------------------------------------------------------";
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
                            appendCurSheetData(tcNameStr, resultStr, caseNameStr, sheetInputPairData, backupCurSheetIndexData);
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
                        appendCurSheetData(tcNameStr, resultStr, caseStr, caseInputDataList, backupCurSheetIndexData);
                    }
                    result = true;
                }
            }
        }

        if (result == true) {
            updateCurSheetData(sheetIndex, backupCurSheetIndexData);
            QList<QStringList> currentSheetData = ExcelDataManager::instance().data()->isSheetDataInfo(sheetIndex);
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
            qDebug() << "[[Set Sheet Data] currentSheetData : " << currentSheetData;
#endif
            if (currentSheetData.size() > 0) {
                QVariantList tmpSheetData;
                for (auto& data : currentSheetData) {
                    tmpSheetData.append(data);
                }
                ExcelData::instance().data()->setSheetData(convertSheetIndex, tmpSheetData);
            } else {
                // no data changed.
#if defined(ENABLE_INPUT_SIGNAL_KEYWORD_DEBUG_LOG)
                qDebug("[convertInputSignalKeyword] No Data Changed (sheet index : %d) -> No need to create excel file",
                       convertSheetIndex);
#endif
            }
        }
    }

    return result;
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

void ConvertDataManager::updateCurSheetData(const int& sheetIndex,
    const QList<std::tuple<QString, QString, QString, QPair<QStringList, QStringList>>>& retCurSheetData) {
    for (int i = 0; i < retCurSheetData.size(); ++i) {
        auto tmpSheetData = retCurSheetData.at(i);
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

#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    auto sheetData = ExcelData::instance().data()->getSheetData(currentSheetIndex).toList();
    ExcelDataManager::instance().data()->updateExcelData(currentSheetIndex, sheetData);
#else
    ExcelDataManager::instance().data()->resetExcelData(false);
#endif

    return sheetDataInfo;
}

// List = [CaseName1, (InputSignalList, InputDataList)] + [CaseName2, (InputSignalList, InputDataList)] + ...
QList<QPair<QString, QPair<QStringList, QStringList>>> ConvertDataManager::getSheetSignalDataInfo(const QString& name,
                                                                                                  const QString& data) {
    const int startIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) + 1;
    const int endIndex = static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax);
    QList<QPair<QString, QPair<QStringList, QStringList>>> sheetKeywordSignalDataInfo;

#if !defined(USE_EXCEL_DATA_MANAGER_OLD)
    ExcelDataManager::instance().data()->resetExcelData(false);
#endif

    // Private ~ Output Sheet Loop
    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        if (sheetIndex == static_cast<int>(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs)) {
            qDebug() << "Not support sheet : 4" << sheetIndex;
            continue;
        }
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
        auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        ExcelDataManager::instance().data()->updateExcelData(sheetIndex, sheetData);
#endif

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
