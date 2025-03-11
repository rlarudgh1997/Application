#include "GuiExcel.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include "GuiCenter.h"

#include <QMenu>
#include <QLabel>
#include <QTextEdit>
#include <QHeaderView>
#include <QClipboard>
#include <QMimeData>

// #define USE_EXCEL_ARROW_KEY

QSharedPointer<GuiExcel>& GuiExcel::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiExcel> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiExcel>(new GuiExcel(handler));
    }
    return gGui;
}

GuiExcel::GuiExcel(AbstractHandler* handler) : AbstractGui(handler), mGui(new Ui::GuiExcel) {
    mMainView = new QWidget(isHandler()->getScreen());
    mGui->setupUi(mMainView);
    mMenuRight = new QMenu(isHandler()->getScreen());
    updateDisplaySize();
}

void GuiExcel::drawDisplayDepth0() {
    updateDisplayVisible();

    mGui->TabWidgetTC->setVisible(false);
    // mGui->TabWidgetTC->raise();

    connect(mGui->CellInfoContent, &QLineEdit::returnPressed, [=]() {
        int currSheetIndex = getCurrSheetIndex();
        if (mExcelSheet[currSheetIndex]->currentItem()) {
            QString text = mGui->CellInfoContent->text();
            mExcelSheet[currSheetIndex]->currentItem()->setText(text);
        }

        mGui->CellInfoContent->clear();
        mGui->CellInfoContent->clearFocus();
        if (mExcelSheet[currSheetIndex]) {
            mExcelSheet[currSheetIndex]->setFocus();
        }
    });
    connect(mGui->TabWidget, &QTabWidget::tabBarClicked, [=](int index) {
        int currSheetIndex = getCurrSheetIndex();
        int currentIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription + index;
        if (currSheetIndex != currentIndex) {
            setCurrSheetIndex(currentIndex);
            updateCellContent(currentIndex, mModelIndex[currentIndex].first, mModelIndex[currentIndex].second);
        }
    });
}

void GuiExcel::drawDisplayDepth1() {
    qDebug() << "GuiExcel::drawDisplayDepth1()";
}

void GuiExcel::drawDisplayDepth2() {
    qDebug() << "GuiExcel::drawDisplayDepth2()";
}

void GuiExcel::updateDisplaySize() {
    QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    QSize margin = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin).toSize();
    QRect rect = isHandler()->getScreen()->geometry();

    if (margin.width() != 0) {
        rect.setX(margin.width());
        rect.setWidth(size.width() - margin.width());
    }
    if (margin.height() != 0) {
        rect.setY(margin.height());
        rect.setHeight(size.height() - margin.height());
    }
    mMainView->setGeometry(rect);

    rect.setY(20);
    rect.setHeight(size.height() - margin.height() - rect.y());
    mGui->TabWidget->setGeometry(rect);
    mGui->TabWidgetTC->setGeometry(rect);
}

void GuiExcel::updateDisplayVisible() {
    mMainView->setVisible(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool());
}

void GuiExcel::updateDrawDialog(const int& dialogType, const QVariantList& info) {
    if (mDialog.isNull()) {
        QRect rect = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo).toRect();
        mDialog = QSharedPointer<Dialog>(new Dialog(rect, isHandler()->getScreen()));

        connect(mDialog.data(), &QDialog::finished, [=]() {
            disconnect(mDialog.data(), nullptr, nullptr, nullptr);
            mDialog.reset();
        });
        connect(mDialog.data(), &Dialog::signalSelectListItem, [=](const QList<QPair<int, QString>>& selectItem) {
            int dialogType = mDialog.data()->getProperty(Dialog::DataTypeDialogType).toInt();
            QString selectValueEnum = QString();

            for (const auto& select : selectItem) {
                if (dialogType == Dialog::DialogTypeSelectValueResult) {
                    if (selectValueEnum.size() > 0) {
                        selectValueEnum.append(", ");
                    }
                    selectValueEnum.append(select.second);
                } else {
                    QStringList lineStr = select.second.split(":");
                    if (lineStr.size() != 2) {
                        continue;
                    }
                    QString temp = (getSfcSignal()) ? (lineStr.at(0)) : (lineStr.at(1));
                    if (getOutputState()) {
                        temp = (getSfcSignal()) ? (lineStr.at(1)) : (lineStr.at(0));
                    }
                    temp.remove("\"");

                    if (selectValueEnum.size() > 0) {
                        selectValueEnum.append(", ");
                    }
                    selectValueEnum.append(temp);
                }
            }
            if ((selectValueEnum.size() > 0) && (mSelectItem)) {
                mSelectItem->setText(selectValueEnum);
            }
            mDialog.data()->accept();
        });
        connect(mDialog.data(), &Dialog::signalSelectOption,
                [=](const bool& option1, const QList<QPair<QString, bool>>& option2) {
                    QString vehicleType = QString();
                    for (const auto& check : option2) {
                        if (check.second) {
                            if (vehicleType.size() > 0) {
                                vehicleType.append(", ");
                            }
                            vehicleType.append(check.first);
                        }
                    }
                    if ((vehicleType.size() > 0) && (mSelectItem)) {
                        mSelectItem->setText(vehicleType);
                    }
                });
        connect(mDialog.data(), &Dialog::signalAutoCompleteSelected, [=](const QString& text) {
            // int dialogType = mDialog.data()->getProperty(Dialog::DataTypeDialogType).toInt();
            if (mSelectItem) {
#if 1
                mSelectItem->setText(text);
#else
                QVariant keywordList = mDialog.data()->getProperty(Dialog::DataTypeAutoCompleteListKeyword);
                QString tempSignalName = text;
                for (const auto& keyword : keywordList.toStringList()) {
                    tempSignalName.remove(keyword);
                }

                QVariant tcNameList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName);
                QString singalName;
                if (tcNameList.toStringList().contains(tempSignalName)) {
                    singalName.append(KEYWORD_SHEET);
                }
                singalName.append(text);
                mSelectItem->setText(singalName);
#endif
            }
        });
    }

    if (dialogType != Dialog::DialogTypeInvalid) {
        mDialog.data()->drawDialog(dialogType, info);
    }
}

void GuiExcel::updateDialogAutoCompleteVehicle() {
    QVariantList info = QVariantList({
        QString("Select Vehicle"),
        QString(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeSelectVehicleType, info);
}

void GuiExcel::updateDialogAutoCompleteConfigName() {
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    QString currentCellText = (mSelectItem) ? (mSelectItem->text()) : (QString());
    ivis::common::getRemoved(currentCellText, mergeInfos);

    QVariantList info = QVariantList({
        QString("Select Config"),
        currentCellText,
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressConfigName).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeAutoCompleteNormal, info);
}

void GuiExcel::updateDialogValueEnum(const QVariantList& data) {
    if (data.size() != 2) {
        qDebug() << "Fail to value enum data size :" << data.size();
        return;
    }

    const QString VEHICLE_TYPE_ICV = QString("ICV");
    const QString VEHICLE_TYPE_EV = QString("EV");
    const QString VEHICLE_TYPE_FCEV = QString("FCEV");
    const QString VEHICLE_TYPE_PHEV = QString("PHEV");
    const QString VEHICLE_TYPE_HEV = QString("HEV");

    QString signalName = data.at(0).toString();
    int columnIndex = data.at(1).toInt();

    bool sfcSignal = (signalName.trimmed().startsWith("SFC.") || signalName.trimmed().startsWith("SFCFuel."));
    // bool vehicleSignal = signalName.trimmed().startsWith("Vehicle.");
    bool outputState = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));

    int dialogType = Dialog::DialogTypeSelectValueEnumInput;
    QStringList subTitle = QStringList({"Value Enum"});
    QVariant vehicleTypeList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType);
    QStringList valueEnum =
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataValueEnum).toStringList();
    QVariantList matchingTableList = QVariantList();
    if ((sfcSignal == false) && (outputState == false)) {
        matchingTableList.append(valueEnum);  // ValueEnum -> Dialog : Not Used
        QVariant matchingTableData = QVariant();
        for (const auto& vehicle : vehicleTypeList.toStringList()) {
            int propertyType = 0;
            if (vehicle.compare(VEHICLE_TYPE_ICV) == 0) {
                propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableICV;
            } else if (vehicle.compare(VEHICLE_TYPE_EV) == 0) {
                propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableEV;
            } else if (vehicle.compare(VEHICLE_TYPE_FCEV) == 0) {
                propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableFCEV;
            } else if (vehicle.compare(VEHICLE_TYPE_PHEV) == 0) {
                propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTablePHEV;
            } else if (vehicle.compare(VEHICLE_TYPE_HEV) == 0) {
                propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableHEV;
            } else {
                continue;
            }
            subTitle.append(vehicle);
            matchingTableData = isHandler()->getProperty(propertyType);
            matchingTableList.append(matchingTableData.toStringList());  // ICV, EV, FCEV, PHEV, HEV
        }
        int appMode = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppMode).toInt();
        dialogType = (appMode == ivis::common::AppModeEnum::AppModeTypePV) ? (Dialog::DialogTypeSelectMatchingTablePV)
                                                                           : (Dialog::DialogTypeSelectMatchingTableCV);
        subTitle.append("System");
        matchingTableData = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableSystem);
        matchingTableList.append(matchingTableData.toStringList());  // System
    } else if (outputState) {
        dialogType = Dialog::DialogTypeSelectValueEnumOutput;
    } else {
        dialogType = Dialog::DialogTypeSelectValueEnumInput;
    }
    setSfcSignal(sfcSignal);
    setOutputState(outputState);

    QVariantList info = QVariantList({
        QString("Select Data"),
        subTitle,
        valueEnum,
        QStringList(),
        matchingTableList,
        0,
    });
    updateDrawDialog(dialogType, info);
}

void GuiExcel::updateDialogTCNameResult(const QStringList& data) {
    QVariantList info = QVariantList({
        QString("Select Data"),
        QStringList({"Result"}),
        data,
        QStringList(),
        QStringList(),
        0,
    });
    updateDrawDialog(Dialog::DialogTypeSelectValueResult, info);
}

void GuiExcel::updateDialogSelectGenType() {
    QVariantList info = QVariantList({
        QString("Select Type"),
        QString(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeGenTypeList).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeGenType, info);
}

bool GuiExcel::isSheetContentChanged(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return false;
    }

    QVariantList readSheetData = isHandler()->getProperty(sheetIndex).toList();
    if (readSheetData.size() > 0) {
        readSheetData.remove(0);  // Delete : Title
    }
    QVariantList currentSheetData = QVariantList();
    QAbstractItemModel* model = mExcelSheet[sheetIndex]->model();
    for (int rowIndex = 0; rowIndex < model->rowCount(); ++rowIndex) {
        QStringList rowData = QStringList();
        for (int columnIndex = 0; columnIndex < model->columnCount(); ++columnIndex) {
            QModelIndex index = model->index(rowIndex, columnIndex);
            rowData.append(model->data(index).toString());
        }
        currentSheetData.append(rowData);
    }
    return (readSheetData != currentSheetData);
}

bool GuiExcel::isDrawCheckBox(const int& sheetIndex, const int& columnIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return false;
    }
    bool draw = ((sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) &&
                 (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) &&
                 (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)));
    return draw;
}

QList<QStringList> GuiExcel::isSheetData(const int& sheetIndex, const bool& removeMerge, const bool& readProperty) {
    if (chcekExcelSheet(sheetIndex)) {
        return QList<QStringList>();
    }
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const int rowMax = mExcelSheet[sheetIndex]->rowCount();
    const int columnMax = mExcelSheet[sheetIndex]->columnCount();

    QVariantList readSheetData;

    if (readProperty) {
        readSheetData = isHandler()->getProperty(sheetIndex).toList();
    } else {
        for (int rowIndex = 0; rowIndex < rowMax; ++rowIndex) {
            QStringList rowData;
            for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
                rowData.append(isCurrentCellText(sheetIndex, rowIndex, columnIndex));
            }
            readSheetData.append(rowData);
        }
    }

    QList<QStringList> sheetData;
    for (const auto& rowDataList : readSheetData) {
        QStringList rowData = rowDataList.toStringList();
        if (removeMerge) {
            for (auto& columnData : rowData) {
                if (columnData.contains(mergeStart)) {
                    columnData.remove(mergeStart);
                } else if ((columnData.contains(mergeEnd)) || (columnData.contains(merge))) {
                    columnData.clear();
                } else {
                }
            }
        }
        sheetData.append(rowData);
    }

#if 0
    qDebug() << "isSheetData :" << sheetIndex << removeMerge << readProperty;
    for (const auto& rowDataList : sheetData) {
        qDebug() << "\t RowData :" << rowDataList;
    }
    qDebug() << "------------------------------------------------------------------------------------------";
#endif

    return sheetData;
}

QString GuiExcel::isCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return QString();
    }
    if (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex) == nullptr) {
        mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(""));
    }
    QString text = mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->text();
    if (isDrawCheckBox(sheetIndex, columnIndex)) {
        bool checked = (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->checkState() == Qt::Checked);
        text = (checked) ? (QString("O")) : (QString(""));
    }
    return text;
}

QMap<int, QSet<QPair<int, int>>> GuiExcel::isPropertyMergeInfo(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return QMap<int, QSet<QPair<int, int>>>();
    }

    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    // const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const QVariantList sheetData = isHandler()->getProperty(sheetIndex).toList();

    QMap<int, QSet<QPair<int, int>>> mergeInfo;
    QMap<int, QPair<int, int>> foundRowInfo;
    int rowIndex = 0;

    for (const auto& rowDataList : sheetData) {
        QStringList rowData = rowDataList.toStringList();
        for (int columnIndex = 0; columnIndex < rowData.size(); ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            if (ivis::common::isContainsString(readColumnText, mergeStart)) {
                foundRowInfo[columnIndex] = QPair<int, int>(rowIndex, rowIndex);
                // qDebug() << "\t\t [" << columnIndex << "] Start :" << foundRowInfo[columnIndex] << readColumnText;
            } else if (ivis::common::isContainsString(readColumnText, mergeEnd)) {
                foundRowInfo[columnIndex] = QPair<int, int>(foundRowInfo[columnIndex].first, rowIndex);
                bool insertState = ((foundRowInfo[columnIndex].second - foundRowInfo[columnIndex].first) > 0);
                if (insertState)  {
                    // qDebug() << "\t\t [" << columnIndex << "] End   :" << foundRowInfo[columnIndex] << readColumnText;
                    mergeInfo[columnIndex].insert(foundRowInfo[columnIndex]);
                }
            } else {
            }
        }
        rowIndex++;
    }

#if 0
    qDebug() << "=========================================================================================";
    qDebug() << "isPropertyMergeInfo :" << sheetIndex;
    for (const auto& rowData : sheetData.toList()) {
        qDebug() << "\t RowData :" << rowData.toStringList();
    }
    qDebug() << "-----------------------------------------------------------------------------------------";
    for (const auto& key : mergeInfo.keys()) {
        qDebug() << "\t MergeInfo[" << key << "]  :" << mergeInfo[key];
    }
    qDebug() << "-----------------------------------------------------------------------------------------";
#endif

    return mergeInfo;
}

QMap<int, QList<QPair<int, int>>> GuiExcel::isDisplayMergeInfo(const int& sheetIndex, const QVariantList& info) {
    if (chcekExcelSheet(sheetIndex)) {
        return QMap<int, QList<QPair<int, int>>>();
    }

    int rowStart = 0;
    int columnStart = 0;
    int rowCount = mExcelSheet[sheetIndex]->rowCount();
    int columnCount = mExcelSheet[sheetIndex]->columnCount();
    bool adjustState = true;

    if (info.size() == 5) {    // info({rowStart, columnStart, rowCount, columnCount, adjustState})
        rowStart = info.at(0).toInt();
        columnStart = info.at(1).toInt();
        rowCount = info.at(2).toInt();
        columnCount = info.at(3).toInt();
        adjustState = info.at(4).toBool();
    }

    QMap<int, QSet<QPair<int, int>>> mergeInfoTemp;
    for (int columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
        for (int rowIndex = 0; rowIndex < rowCount; ) {
            int mergeCount = mExcelSheet[sheetIndex]->rowSpan((rowStart + rowIndex), (columnStart + columnIndex));
            if (mergeCount > 1) {
                int mergeRowStart = (adjustState) ? (rowStart + rowIndex) : (rowIndex);
                int mergeRowEnd = mergeRowStart + mergeCount - 1;
                mergeInfoTemp[(columnIndex + columnStart)].insert(qMakePair(mergeRowStart, mergeRowEnd));
                // qDebug() << "\t\t ReadInfo[" << columnIndex << "] :" << mergeRowStart << "~" << mergeRowEnd;
            }
            rowIndex += mergeCount;
        }
    }

    // Sorting : MergeInfo first value
    QMap<int, QList<QPair<int, int>>> mergeInfo;
    for (const auto& key : mergeInfoTemp.keys()) {
        QList<QPair<int, int>> sortDataList = mergeInfoTemp[key].values();
        std::sort(sortDataList.begin(), sortDataList.end(), [] (const QPair<int, int>& value1, const QPair<int, int>& value2) {
            return value1.first < value2.first;
        });
        mergeInfo[key].append(sortDataList);
    }

#if 0   // 여러개(Row 기준) 이어지지 않은 셀을 선택할 경우 예외 처리 위한 코드 : 현재 미사용
    // Construct : Span Meger Info
    QMap<int, QList<QPair<int, int>>> spanMergeInfo;
    auto tempMergeInfo = isDisplayMergeInfo(sheetIndex, QVariantList({rowStart, columnStart, rowCount, columnCount, false}));
    for (const auto& key : tempMergeInfo.keys()) {
        QList<QPair<int, int>> tempInfo;
        int spanRowStart = 0;
        for (const auto& infoData : tempMergeInfo[key]) {
            int spanRowCount = infoData.second - infoData.first;
            int spanRowEnd = spanRowStart + spanRowCount;

            QPair<int, int> infoTemp = qMakePair(spanRowStart, spanRowEnd);
            spanMergeInfo[key].append(infoTemp);
            spanRowStart = spanRowEnd + 1;

            qDebug() << "\t SpanUpdate[" << key << "]  :" << infoData << "->" << infoTemp;
        }
    }
    qDebug() << "-----------------------------------------------------------------------------------------";
    for (const auto& key : clearMergeInfo.keys()) {
        qDebug() << "\t SpanMergeInfo[" << key << "]  :" << clearMergeInfo[key];
    }
    qDebug() << "-----------------------------------------------------------------------------------------";
#endif

#if 0
    qDebug() << "isDisplayMergeInfo :" << sheetIndex << info;
    for (const auto& key : mergeInfo.keys()) {
        qDebug() << "\t MergeInfo[" << key << "]  :" << mergeInfo[key];
    }
    qDebug() << "-----------------------------------------------------------------------------------------";
#endif

    return mergeInfo;
}

bool GuiExcel::chcekExcelSheet(const int& sheetIndex) {
    if (mExcelSheet.size() == 0) {
        qDebug() << "Fail to - excel sheet was not created";
        return true;
    }

    if (sheetIndex == 0) {
        return false;
    }

    if (mExcelSheet[sheetIndex] == nullptr) {
        qDebug() << "Fail to - excel sheet nullptr :" << sheetIndex;
        return true;
    }

    return false;
}

QVariantList GuiExcel::readSheetDisplay(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        qDebug() << "Fail to display sheet nullptr : " << sheetIndex;
        return QVariantList();
    }

    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const auto mergeInfo = isDisplayMergeInfo(sheetIndex, QVariantList());

    // Construct : Current Display Sheet Data
    QList<QStringList> displaySheetData = isSheetData(sheetIndex, false, false);
    const int columnMax = (displaySheetData.size() == 0) ? (0) : (displaySheetData.at(0).size());

    // Property 기반 병합 정보를 가지고 현재 화면상에 표시된 셀 병합 정보/텍스트 갱신
    for (const auto& columnIndex : mergeInfo.keys()) {
        if (columnIndex >= columnMax) {
            qDebug() << "Column index out of range :" << columnIndex << columnMax;
            continue;
        }
        for (const auto& info : mergeInfo[columnIndex]) {
            const int mergeRowStart = info.first;
            if (mergeRowStart >= displaySheetData.size()) {
                qDebug() << "No matching merge start info :" << mergeRowStart;
                continue;
            }
            const QString readText = displaySheetData[mergeRowStart][columnIndex];   // Merge Start Index Cell Text
            const int mergeRowNormal = mergeRowStart + 1;
            const int mergeRowEnd = qMin(info.second, (displaySheetData.size() - 1));

            // MergeInfo - Start + Text
            displaySheetData[mergeRowStart][columnIndex] = QString(mergeStart + readText);
            // MergeInfo - Normal
            for (int mergeRowIndex = mergeRowNormal; mergeRowIndex < mergeRowEnd; ++mergeRowIndex) {
                displaySheetData[mergeRowIndex][columnIndex] = QString(merge + readText);
            }
            // MergeInfo - End
            displaySheetData[mergeRowEnd][columnIndex] = QString(mergeEnd + readText);
        }
    }

    // Convert : QList<QStringList> -> QVariantList
    QVariantList sheetData;
    for (const auto& rowDataList : displaySheetData) {
        sheetData.append(rowDataList);
    }

#if 0
    qDebug() << "==================================================================================================";
    qDebug() << "readSheetDisplay :"<< sheetIndex << sheetData.size();
    int index = 0;
    for (const auto& rowDataList : sheetData.toList()) {
        qDebug() << "\t RowData[" << index++ << "] :" << rowDataList.toStringList();
    }
    qDebug() << "==================================================================================================\n";
#endif

    return sheetData;
}

void GuiExcel::syncSheetData(const int& sheetIndex, const bool& readProperty) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    const int eventType = ivis::common::EventTypeEnum::EventTypeListDescription +
                          (sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    QVariantList sheetData = readSheetDisplay(sheetIndex);

    qDebug() << "syncSheetData :" << sheetIndex << readProperty << eventType << sheetData.size();
    createSignal(eventType, QVariantList({sheetIndex, sheetData}));
}

void GuiExcel::syncAutoComplete(const int& sheetIndex, const int& columnIndex, const QVariant& data) {
    int eventType = ivis::common::EventTypeEnum::EventTypeInvalid;
    QVariantList eventValue = QVariantList();
    bool support = false;

    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription: {
            eventType = ivis::common::EventTypeEnum::EventTypeAutoInputDescriptionInfo;
            eventValue = QVariantList({data});
            support = true;
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs: {
            eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData;
            eventValue = QVariantList({sheetIndex, columnIndex});
            support = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates:
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetTelltales:
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConstants:
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetEvents:
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetSounds:
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetInters:
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetOutputs: {
            eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData;
            eventValue = QVariantList({sheetIndex, columnIndex});
            support = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
            break;
        }
        default: {
            break;
        }
    }

    if (support) {
        createSignal(eventType, eventValue);
    }
}

int GuiExcel::updateShortcutInsertDelete(const int& sheetIndex, const bool& insert) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(mExcelSheet[sheetIndex]);
    if ((insert == false) && (cellSelectedInfo.isEmpty())) {
        return (-1);
    }

    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int columnStart = cellSelectedInfo.getColumnStart();

    qDebug() << "updateShortcutInsertDelete :" << sheetIndex << insert << rowStart << rowEnd;

    for (int index = rowStart; index <= rowEnd; ++index) {
        if (insert) {
            mExcelSheet[sheetIndex]->insertRow(rowStart);
            for (int columnIndex = 0; columnIndex < mExcelSheet[sheetIndex]->columnCount(); ++columnIndex) {
                QTableWidgetItem* item = new QTableWidgetItem();
                if (isDrawCheckBox(sheetIndex, columnIndex)) {
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                    item->setCheckState(Qt::Unchecked);
                }
                mExcelSheet[sheetIndex]->setItem(rowStart, columnIndex, item);
            }
        } else {
            mExcelSheet[sheetIndex]->removeRow(rowStart);
            if (mExcelSheet[sheetIndex]->rowCount() == 0) {
                setSheetCheckState(sheetIndex, false);
            }
        }
    }

    return columnStart;
}

int GuiExcel::updateShortcutMergeSplit(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(mExcelSheet[sheetIndex]);
    if (cellSelectedInfo.isEmpty()) {
        return (-1);
    }

    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowCount = cellSelectedInfo.getRowCount();
    const int columnStart = cellSelectedInfo.getColumnStart();
    // const int columnEnd = cellSelectedInfo.getColumnEnd();
    const int columnCount = cellSelectedInfo.getColumnCount();

    if (columnCount > 1) {  // 병합은 Column 기준 1개만 지원
        qDebug() << "Not support : multilple slected column";
        createSignal(ivis::common::EventTypeEnum::EventTypeWarningMergeSplit);
        return (-1);
    }

    const int rowSpan = mExcelSheet[sheetIndex]->rowSpan(rowStart, columnStart);
    const int columnSpan = mExcelSheet[sheetIndex]->columnSpan(rowStart, columnStart);
    const bool mergedState = (rowSpan > 1 || columnSpan > 1);

    qDebug() << "updateShortcutMergeSplit :" << sheetIndex << columnStart << columnCount << rowStart << rowCount;
    qDebug() << "\t MergedState :" << mergedState << rowSpan << columnSpan;

    if (mergedState) {
        for (int rowIndex = rowStart; rowIndex < rowEnd; ++rowIndex) {
            mExcelSheet[sheetIndex]->setSpan(rowIndex, columnStart, 1, 1);
        }
    } else {
        mExcelSheet[sheetIndex]->setSpan(rowStart, columnStart, rowCount, columnCount);
    }

    return columnStart;
}

int GuiExcel::updateShortcutCopy(const int& sheetIndex, const bool& cutState) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }

    CellSelectedInfo cellSelectedInfo(mExcelSheet[sheetIndex]);
    if (cellSelectedInfo.isEmpty()) {
        return (-1);
    }

    if (cellSelectedInfo.isNotSupport()) {
        createSignal(ivis::common::EventTypeEnum::EventTypeWarningCopyCut);
        return (-1);
    }

    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowCount = cellSelectedInfo.getRowCount();
    const int columnStart = cellSelectedInfo.getColumnStart();
    const int columnEnd = cellSelectedInfo.getColumnEnd();
    const int columnCount = cellSelectedInfo.getColumnCount();
    const int rowMax = cellSelectedInfo.getRowMax();
    const int columnMax = cellSelectedInfo.getColumnMax();

    const auto& sheetData = readSheetDisplay(sheetIndex);

    qDebug() << "=====================================================================================================";
    qDebug() << "updateShortcutCopy :" << sheetIndex << cutState;
    qDebug() << "\t Row    :" << rowStart << rowEnd << rowCount;
    qDebug() << "\t Column :" << columnStart << columnEnd << columnCount;
    qDebug() << "\t Max    :" << rowMax << columnMax;
    qDebug() << "\t -----------------------------------------------------------------------------------------";

    // Construct : SheetIndex, ClearData
    cellSelectedInfo.setSheetIndex(sheetIndex);
    cellSelectedInfo.setClearData(cutState);

    // Construct : Selected Cell Read Text
    QMap<int, QStringList> copyDataList;
    QMap<int, QList<int>> indexList;
    for (const auto& currCell : cellSelectedInfo.getModelIndexs()) {
        int cellRowIndex = currCell.row();
        int cellColumnIndex = currCell.column();
        int rowIndex = cellRowIndex - rowStart;
        int columnIndex = cellColumnIndex - columnStart;

        QVariantList rowDataList = sheetData.toList();
        if (cellRowIndex >= rowDataList.size()) {
            continue;
        }
        QStringList columnDataList = rowDataList.at(cellRowIndex).toStringList();
        if (cellColumnIndex >= columnDataList.size()) {
            continue;
        }
        QString readText = currCell.data().toString();    // 현재 화면에 표시 되는 시트에서 텍스트 추출
        // QString readText = columnDataList.at(cellColumnIndex);    // 프로퍼티에 저장 되어있는 시트에서 텍스트 추출
        copyDataList[rowIndex].append(readText);
        indexList[columnIndex].append(rowIndex);
    }

    // Check : Cell Selected Warning
    bool notSupport = false;
    int confirmColumnCount = 0;
    for (const auto& columnIndex : indexList.keys()) {
        int confirmRowIndex = 0;
        for (const auto& rowIndex : indexList[columnIndex]) {
            if (confirmRowIndex != rowIndex) {
                notSupport = true;
                break;
            }
            confirmRowIndex++;
        }
        if (confirmColumnCount != columnIndex) {
            notSupport = true;
            break;
        }
        confirmColumnCount++;
    }
    if (notSupport) {
        updateClearClipboard(cellSelectedInfo, false);
        createSignal(ivis::common::EventTypeEnum::EventTypeWarningCopyCut);
        return (-1);
    }

    // Draw : Normal/Select Cell Background Info
    updateClearClipboard(cellSelectedInfo, cutState);

    // Construct : Clipboard Data
    QString clipboardData;
    for (const auto& key : copyDataList.keys()) {
        QStringList rowData = copyDataList[key];
        clipboardData.append(QString("%1\n").arg(rowData.join("\t")));
        // qDebug() << "\t CopyData :" << rowData.join(", ");
    }
    clipboardData.resize(clipboardData.size() - 1);    // 마지막 개행문자 제거
    QGuiApplication::clipboard()->setText(clipboardData);

    // Construct : Clear Meger Info
    auto clearMergeInfo = isDisplayMergeInfo(sheetIndex, QVariantList({rowStart, columnStart, rowCount, columnCount, true}));
    cellSelectedInfo.writeClearMergeInfo(clearMergeInfo);
    auto spanMergeInfo = isDisplayMergeInfo(sheetIndex, QVariantList({rowStart, columnStart, rowCount, columnCount, false}));
    cellSelectedInfo.writeSpanMergeInfo(spanMergeInfo);

    // Update : Copy Info
    setCopyInfo(cellSelectedInfo);

    return columnStart;
}

int GuiExcel::updateShortcutPaste(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }

    if ((QApplication::clipboard()->mimeData() == nullptr) || (QApplication::clipboard()->mimeData()->hasText() == false)) {
        qDebug() << "Clipboard data is invalid";
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(mExcelSheet[sheetIndex]);
    // if (cellSelectedInfo.isEmpty()) {    // 셀이 하나도 없는 경우에도 붙여넣기 추가 가능
    //     return (-1);
    // }

    setCellEditSkip(true);

    // Construct Info : Current Selected Cell Info
    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowCount = cellSelectedInfo.getRowCount();
    const int columnStart = cellSelectedInfo.getColumnStart();
    const int columnEnd = cellSelectedInfo.getColumnEnd();
    const int columnCount = cellSelectedInfo.getColumnCount();
    const int rowMax = cellSelectedInfo.getRowMax();
    const int columnMax = cellSelectedInfo.getColumnMax();
    const QStringList clipboardData = QApplication::clipboard()->mimeData()->text().split("\n");
    const int clipboardDataMax = clipboardData.size() + rowStart;  // Row : 0(Header), 1 ~ Count(Item)

    // Construct Info : Copy/Cut Selected Cell Info
    const CellSelectedInfo copyInfo = getCopyInfo();
    const int copyRowStart = copyInfo.getRowStart();
    const int copyRowEnd = copyInfo.getRowEnd();
    const int copyRowCount = copyInfo.getRowCount();
    const int copyColumnStart = copyInfo.getColumnStart();
    const int copyColumnEnd = copyInfo.getColumnEnd();
    const int copyColumnCount = copyInfo.getColumnCount();
    const int copyRowMax = copyInfo.getRowMax();
    const int copyColumnMax = copyInfo.getColumnMax();

    const int copySheetIndex = copyInfo.getSheetIndex();
    const bool clearData = copyInfo.getClearData();
    const auto clearMergeInfo = copyInfo.readClearMergeInfo();
    const auto spanMergeInfo = copyInfo.readSpanMergeInfo();

    // Update : Row Count (붙여넣기시 Row 마지막을 넘는 경우 Row Count 증가)
    if (clipboardDataMax >= rowMax) {
        updateRowMax(sheetIndex, rowMax, clipboardDataMax);
    }

    qDebug() << "=====================================================================================================";
    qDebug() << "updateShortcutPaste :" << sheetIndex << copySheetIndex;
    qDebug() << "\t Row           :" << rowStart << rowEnd << rowCount;
    qDebug() << "\t Column        :" << columnStart << columnEnd << columnCount;
    qDebug() << "\t Max           :" << rowMax << columnMax << clipboardDataMax;
    qDebug() << "\t ClipboardData :" << clipboardData.size() << clipboardData;
    qDebug() << "\t CopyRow       :" << copyRowStart << copyRowEnd << copyRowCount;
    qDebug() << "\t CopyColumn    :" << copyColumnStart << copyColumnEnd << copyColumnCount;
    qDebug() << "\t -----------------------------------------------------------------------------------------";

    // Clear : Cut Selected Cell Backgorund/Text/Merge Info
    if (chcekExcelSheet(copySheetIndex) == false) {
        if (clearData) {
            for (int columnIndex = copyColumnStart; columnIndex <= copyColumnEnd; ++columnIndex) {
                for (int rowIndex = copyRowStart; rowIndex <= copyRowEnd; ++rowIndex) {
                    if (mExcelSheet[copySheetIndex]->item(rowIndex, columnIndex) == nullptr) {
                        continue;
                    }
                    mExcelSheet[copySheetIndex]->item(rowIndex, columnIndex)->setBackground(QColor(255, 255, 255));
                    mExcelSheet[copySheetIndex]->item(rowIndex, columnIndex)->setText("");
                }
            }
        }

        // Update : Merge Cell Info
        const auto copyMergeInfo = isDisplayMergeInfo(copySheetIndex, QVariantList());
        mExcelSheet[copySheetIndex]->clearSpans();
        for (const auto& mergeColumnIndex : copyMergeInfo.keys()) {
            for (const auto& merge : copyMergeInfo[mergeColumnIndex]) {
                int mergeRowStart = merge.first;
                int mergeRowCount = merge.second - mergeRowStart + 1;

                if (clearData) {
                    auto clearInfo = clearMergeInfo[mergeColumnIndex];
                    // qDebug() << "\t ClearMergeInfo[" << mergeColumnIndex << "] :" << clearInfo.contains(merge) << merge;
                    if (clearInfo.contains(merge)) {
                        continue;
                    }
                }
                mExcelSheet[copySheetIndex]->setSpan(mergeRowStart, mergeColumnIndex, mergeRowCount, 1);
            }
        }
    }

    // Update : Paste Text Info
    QSet<int> columnIndexList;
    for (int rowIndex = 0; rowIndex < clipboardData.size(); rowIndex++) {
        QStringList rowDataList = clipboardData[rowIndex].split("\t");
        for (int columnIndex = 0; columnIndex < rowDataList.size(); columnIndex++) {
            int cellRowIndex = (rowStart + rowIndex);
            int cellColumnIndex = (columnStart + columnIndex);
            if (cellColumnIndex >= columnMax) {
                continue;
            }
            QString setText = rowDataList[columnIndex];
            // if (setText.compare(mEmptyString) == false) {    // 복사시 빈칸 정보([Empty])는 공백으로 복사
            //     setText.clear();
            // }
            mExcelSheet[sheetIndex]->setItem(cellRowIndex, cellColumnIndex, new QTableWidgetItem(setText));
            columnIndexList.insert(cellColumnIndex);
        }
    }

    // Update : Paste Span Merge Info
    for (const auto& key : spanMergeInfo.keys()) {
        int mergeColumnIndex = columnStart + key - copyColumnStart;
        for (const auto& merge : spanMergeInfo[key]) {
            int mergeRowStart = merge.first + rowStart;
            int mergeRowCount = merge.second - merge.first + 1;
            mExcelSheet[sheetIndex]->setSpan(mergeRowStart, mergeColumnIndex, mergeRowCount, 1);
            // qDebug() << "\t SpanMerge[" << mergeColumnIndex << "] :" << mergeRowStart << mergeRowCount;
        }
    }

    // Update : Sheet Data
    syncSheetData(sheetIndex, false);
    if (sheetIndex != copySheetIndex) {
        syncSheetData(copySheetIndex, false);
    }

    // Update : AutoComplete Info
    for (const auto& columnIndex : columnIndexList) {
        syncAutoComplete(sheetIndex, columnIndex);
    }

    setCellEditSkip(false);

    return columnStart;
}

void GuiExcel::updateClearClipboard(const CellSelectedInfo& clearInfo, const bool& cellHighlight) {
    int sheetIndex = clearInfo.getSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    const int rowStart = clearInfo.getRowStart();
    const int rowEnd = clearInfo.getRowEnd();
    const int rowMax = clearInfo.getRowMax();
    const int columnStart = clearInfo.getColumnStart();
    const int columnEnd = clearInfo.getColumnEnd();
    const int columnMax = clearInfo.getColumnMax();

    for (int rowIndex = 0; rowIndex < rowMax; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QColor setColor(255, 255, 255);
            if ((rowIndex >= rowStart) && (rowIndex <= rowEnd) && (columnIndex >= columnStart) && (columnIndex <= columnEnd) &&
                (cellHighlight)) {
                setColor = QColor(200, 200, 200);
            }
            mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->setBackground(setColor);
        }
    }

    setCopyInfo(CellSelectedInfo());
    QGuiApplication::clipboard()->clear();
}

void GuiExcel::updateRowMax(const int& sheetIndex, const int& rowMax, const int& changeRowMax) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    mExcelSheet[sheetIndex]->setRowCount(changeRowMax);

    // qDebug() << "updateRowMax :" << sheetIndex << rowMax << changeRowMax << mExcelSheet[sheetIndex]->rowCount();
    if (isDrawCheckBox(sheetIndex, 0)) {
        for (int rowIndex = rowMax; rowIndex < changeRowMax; ++rowIndex) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            mExcelSheet[sheetIndex]->setItem(rowIndex, 0, item);
        }
    }
}

void GuiExcel::updateSheetHeaderAdjust(const int& sheetIndex, const bool& resizeColumn) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if (resizeColumn) {
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
    }
    mExcelSheet[sheetIndex]->resizeRowsToContents();
}

void GuiExcel::updateSheetDefaultFocus(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if (mExcelSheet[sheetIndex]->item(row, column) == nullptr) {
        mExcelSheet[sheetIndex]->setItem(row, column, new QTableWidgetItem(""));
    }

    if (mExcelSheet[sheetIndex]->item(row, column)) {
        mExcelSheet[sheetIndex]->item(row, column)->setSelected(true);
        // mExcelSheet[sheetIndex]->setCurrentItem(mExcelSheet[sheetIndex]->item(row, column));
        mExcelSheet[sheetIndex]->setFocus();
    }
}

void GuiExcel::updateCellContent(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    mModelIndex[sheetIndex] = QPair<int, int>(row, column);
    // qDebug() << "Click[" << sheetIndex << "] :" << mModelIndex[sheetIndex].first << mModelIndex[sheetIndex].second;

    QString text = QString();
    if ((mExcelSheet[sheetIndex]->item(row, column)) && (mExcelSheet[sheetIndex]->rowCount() > 0)) {
        text = mExcelSheet[sheetIndex]->item(row, column)->text();
    }

    int currSheetIndex = getCurrSheetIndex();
    if (currSheetIndex == sheetIndex) {
        // qDebug() << "Info :" <<  currSheetIndex << sheetIndex << text << row << column;
        mGui->CellInfoContent->setText(text);
    }
}

void GuiExcel::updateSheetProperty(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    // const QVariantList sheetData = readSheetProperty(sheetIndex);
    const QList<QStringList> sheetData = isSheetData(sheetIndex, true, true);

    int rowIndex = 0;

    for (const auto& rowDataList : sheetData) {
        int columnIndex = 0;
        for (const auto& columnData : rowDataList) {
            if (isDrawCheckBox(sheetIndex, columnIndex)) {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                item->setCheckState((columnData.size() > 0) ? (Qt::Checked) : (Qt::Unchecked));
                mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, item);
            } else {
                mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(columnData));
            }
            columnIndex++;
        }
        rowIndex++;
    }

    // Update : Merge Info
    const auto& mergeInfo = isPropertyMergeInfo(sheetIndex);
    for (const auto& columnIndex : mergeInfo.keys()) {
        for (const auto& info : mergeInfo[columnIndex]) {
            int mergeRowStart = info.first;
            int mergeRowEnd = info.second - mergeRowStart + 1;
            mExcelSheet[sheetIndex]->setSpan(mergeRowStart, columnIndex, mergeRowEnd, 1);
        }
    }
}

void GuiExcel::updateSheetNew(const int& sheetIndex, const int& rowMax, const int& columnMax) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    // qDebug() << "updateSheetNew :" << sheetIndex << rowMax << columnMax;
    for (int rowIndex = 0; rowIndex < rowMax; ++rowIndex) {
        for (int columnIndex = 0; columnIndex < rowMax; columnIndex++) {
            if (isDrawCheckBox(sheetIndex, columnIndex)) {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                item->setCheckState(Qt::Unchecked);
                mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, item);
                // qDebug() << "Item :" << rowIndex << columnIndex;
            }
        }
    }
}

void GuiExcel::updateInitExcelSheet() {
    setCurrSheetIndex(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    mModelIndex.clear();

    for (const auto& action : mMenuActionItem) {
        delete action;
    }
    mMenuActionItem.clear();
    mMenuRight->clear();

    for (const auto& sheet : mExcelSheet) {
        // disconnect(sheet);
        // disconnect(sheet, &QTableWidget::itemSelectionChanged, nullptr, nullptr);
        disconnect(sheet, nullptr, nullptr, nullptr);
        sheet->clear();
        delete sheet;
    }
    mExcelSheet.clear();

    mGui->CellInfoContent->clear();
    for (int index = 0; index < mGui->TabWidget->count(); index++) {
        QWidget* currentWidget = mGui->TabWidget->widget(index);
        if (currentWidget) {
            mGui->TabWidget->removeTab(mGui->TabWidget->indexOf(currentWidget));
            delete currentWidget;
        }
    }
    mGui->TabWidget->clear();

    // qDebug() << "\t ExcelSheet : Init";
}

void GuiExcel::updateDescriptionInfo(const int& sheetIndex, const int& row) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    QString moduleName = mExcelSheet[sheetIndex]->item(row, 0)->text();
    QString version = (mExcelSheet[sheetIndex]->item(row, 1) == nullptr) ? ("") : (mExcelSheet[sheetIndex]->item(row, 1)->text());
    QString description =
        (mExcelSheet[sheetIndex]->item(row, 2) == nullptr) ? ("") : (mExcelSheet[sheetIndex]->item(row, 2)->text());
    if ((moduleName.size() == 0) || (version.size() > 0) || (description.size() > 0)) {
        qDebug() << "Fail to text :" << (moduleName.size() == 0) << (version.size() > 0) << (description.size() > 0);
        return;
    }

    syncAutoComplete(sheetIndex, sheetIndex, moduleName);
}

void GuiExcel::updateAutoCompleteSignal(const bool& description, const int& columnIndex) {
    QVariantList keywordTypeInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeKeywordTypeInfo).toList();
    QStringList keywordList;
    int keyworedIndex = columnIndex;

#if defined(USE_SHEET_COLUMN_OLD)
    if (description) {
        if (keyworedIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal)) {
            keyworedIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal);
        } else if (keyworedIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Data)) {
            keyworedIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data);
        } else {
        }
    }
#endif

    if (keyworedIndex < keywordTypeInfo.size()) {
        keywordList = keywordTypeInfo.at(keyworedIndex).toStringList();
    }

    // qDebug() << "updateAutoCompleteSignal :" << description << columnIndex;
    // qDebug() << "\t Keyword :" << keyworedIndex << keywordTypeInfo.size() << keywordList.size();

    QString currentCellText = (mSelectItem) ? (mSelectItem->text()) : (QString());
    for (const auto& keyword : keywordList) {
        currentCellText.remove(keyword);
    }

    QVariantList info = QVariantList({
        QString("Auto Complete"),
        currentCellText,
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC).toStringList(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM).toStringList(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName).toStringList(),
        keywordList,
    });
    updateDrawDialog(Dialog::DialogTypeAutoComplete, info);
}

void GuiExcel::updateDisplayKey(const int& keyValue) {
    int sheetIndex = getCurrSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if (keyValue == Qt::Key_Escape) {
        updateClearClipboard(getCopyInfo(), false);
        return;
    }

    if (keyValue == Qt::Key::Key_Delete) {
        QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
        if (modelIndexs.size() > 0) {
            for (const auto& model : modelIndexs) {
                mExcelSheet[sheetIndex]->setItem(model.row(), model.column(), new QTableWidgetItem(""));
            }
        }
        return;
    }

    bool inputKeyOK = (keyValue == ivis::common::KeyTypeEnum::KeyInputValueOK);
    int row = mExcelSheet[sheetIndex]->currentRow();
    int column = mExcelSheet[sheetIndex]->currentColumn();
    bool editState = mExcelSheet[sheetIndex]->isPersistentEditorOpen(mExcelSheet[sheetIndex]->item(row, column));
    // qDebug() << "editState :" << editState << ", inputKeyOK :" << inputKeyOK;

    if ((editState) && (inputKeyOK)) {
        mExcelSheet[sheetIndex]->closePersistentEditor(mExcelSheet[sheetIndex]->item(row, column));
    } else if ((editState == false) && (inputKeyOK == false)) {
        mExcelSheet[sheetIndex]->openPersistentEditor(mExcelSheet[sheetIndex]->item(row, column));
    } else {
    }

    if (inputKeyOK) {
#if defined(USE_EXCEL_ARROW_KEY)
        updateDisplayArrowKey(keyValue);
#else
        int rowMax = mExcelSheet[sheetIndex]->rowCount();
        ivis::common::LIMIT_P(row, 1, 0, rowMax);
        mModelIndex[sheetIndex] = QPair<int, int>(row, column);
        mExcelSheet[sheetIndex]->setCurrentCell(row, column);
#endif
    }
}

void GuiExcel::updateDisplayArrowKey(const int& keyValue) {
#if defined(USE_EXCEL_ARROW_KEY)
    int sheetIndex = getCurrSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    int row = mModelIndex[sheetIndex].first;
    int column = mModelIndex[sheetIndex].second;
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    int keySate = 0;

    QString log = QString("Arrow[%1] : %2 %3").arg(sheetIndex).arg(row).arg(column);
    switch (keyValue) {
        case Qt::Key::Key_Left: {
            ivis::common::LIMIT_M(column, 1, 0, columnMax);
            break;
        }
        case Qt::Key::Key_Right: {
            ivis::common::LIMIT_P(column, 1, 0, columnMax);
            break;
        }
        case Qt::Key::Key_Up: {
            keySate = 1;
            ivis::common::LIMIT_M(row, 1, 0, rowMax);
            break;
        }
        case Qt::Key::Key_Down: {
            keySate = 2;
            ivis::common::LIMIT_P(row, 1, 0, rowMax);
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueOK: {
            ivis::common::LIMIT_P(row, 1, 0, rowMax);
            mExcelSheet[sheetIndex]->setCurrentCell(row, column);
            break;
        }
        default: {
            break;
        }
    }

    QMap<int, QList<QPair<int, int>>> mergeInfo = mMergeInfo[sheetIndex].isMergeInfo();
    QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
    while (iter.hasNext()) {
        iter.next();
        int mergeColumnIndex = iter.key();
        if (column != mergeColumnIndex) {
            continue;
        }

        qDebug() << "**********************************************************\n";
        for (const auto& v : iter.value()) {
            int mergeRowStart = v.first;
            int mergeRowCount = v.second;
            int mergeRowEnd = mergeRowStart + mergeRowCount;
            qDebug() << "Row Range :" << mergeRowStart << "<" << row << "<" << mergeRowEnd;

            if ((row > mergeRowStart) && (row < mergeRowEnd)) {
                // 병합 셀인 경우 병합 셀 다음의 인덱스로
                row = (keySate == 2) ? (mergeRowEnd) : (mergeRowStart);
                qDebug() << "\t\t 1 Row :" << row;
            }
            // if (row > rowMax) {
            //     // 마지막 병합 셀인 경우 최대값이 아니라 병합 셀의 시작 값으로
            //     row = rowMax;
            //     qDebug() << "\t\t 2 Row :" << row;
            // }
        }
    }

    mModelIndex[sheetIndex] = QPair<int, int>(row, column);
    log.append(QString(" -> %1, %2").arg(row).arg(column));
    qDebug() << log;
    updateCellContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
#else
    mGui->CellInfoContent->clear();
#endif
}

void GuiExcel::updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if (isSheetContentChanged(sheetIndex) == false) {
        qDebug() << "The excel sheet has not been modified.";
        return;
    }

    if (getCellEditSkip()) {
        // qDebug() << "Skip cell editing event handling.";
        return;
    }

    QString text = mExcelSheet[sheetIndex]->item(row, column)->text();
    int columnIndex = (-1);
    bool updateAutoCompleteData = false;
    // qDebug() << "updateDisplayCellDataInfo :" << sheetIndex << row << column << text;

    if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
        if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Test)) {
            updateDescriptionInfo(sheetIndex, row);
#if defined(USE_SHEET_COLUMN_OLD)
        } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Data)) {
            columnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal);
        } else {
#else
        }
#endif
    } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
        if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName)) {
            updateAutoCompleteData = true;
        }
    } else {
        if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
            columnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
        } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)) {
            columnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config);
#if defined(USE_SHEET_COLUMN_OLD)
        } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data)) {
            columnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal);
#endif
        } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)) {
            updateAutoCompleteData = true;
        } else {
        }
    }

    if (columnIndex >= 0) {
        QString signal = (mExcelSheet[sheetIndex]->item(row, columnIndex) == nullptr)
                             ? (QString())
                             : (mExcelSheet[sheetIndex]->item(row, columnIndex)->text());
        QVariantList cellDataInfo = QVariantList({signal, text, sheetIndex, row, column});
        createSignal(ivis::common::EventTypeEnum::EventTypeUpdateCellDataInfo, cellDataInfo);
    }

    updateSheetHeaderAdjust(sheetIndex, false);  // 편집시 화면 변경 되는 이슈
    syncSheetData(sheetIndex, true);
    if (updateAutoCompleteData) {
        createSignal(ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData, QVariantList({sheetIndex, column}));
    }
}

void GuiExcel::updateDisplayAutoComplete(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    bool notSupport = false;
    bool supportDataInfo = false;
    bool supportGenType = false;
    bool supportVehicleType = false;
    bool supportConfigName = false;
    if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
        notSupport = ((column == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Test)) ||
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Version)) ||
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Description)));
#if defined(USE_SHEET_COLUMN_OLD)
        supportDataInfo = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Data));
#endif
    } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
        notSupport = ((column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName)) ||
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::AndGroup)));
        supportDataInfo = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
    } else {
        notSupport = ((column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)) ||
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)) ||
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)) ||
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) ||
#if defined(USE_SHEET_COLUMN_OLD)
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::NegativeTest)) ||
#endif
                      (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize)));
        supportDataInfo = ((column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) ||
#if defined(USE_SHEET_COLUMN_OLD)
                           (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data)) ||
#endif
                           (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)));
        supportGenType = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
        supportVehicleType = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
        supportConfigName = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config));
    }

    if (notSupport) {
        qDebug() << "Fail to not support auto complete :" << sheetIndex << column;
        return;
    }

    mSelectItem = mExcelSheet[sheetIndex]->item(row, column);
    if (mSelectItem == nullptr) {
        mExcelSheet[sheetIndex]->setItem(row, column, new QTableWidgetItem(""));
        mSelectItem = mExcelSheet[sheetIndex]->item(row, column);
    }

    if (supportDataInfo) {
        int signalIndex = 0;
        if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
#if defined(USE_SHEET_COLUMN_OLD)
            signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal);
#endif
        } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
            if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal);
            }
        } else {
            if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config);
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal);
            } else {
#if defined(USE_SHEET_COLUMN_OLD)
                signalIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal);
#endif
            }
        }

        QString vehicleType = QString();
        int vehicleTypeIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType);
        if (mExcelSheet[sheetIndex]->item(row, vehicleTypeIndex) != nullptr) {
            vehicleType = mExcelSheet[sheetIndex]->item(row, vehicleTypeIndex)->text();
        }

        if (vehicleType.isEmpty()) {
            QVariant vehicleTypeList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType);
            for (const auto& vehicle : vehicleTypeList.toStringList()) {
                if (vehicleType.size() > 0) {
                    vehicleType.append(", ");
                }
                vehicleType.append(vehicle);
            }
        }

        QString signalName = (mExcelSheet[sheetIndex]->item(row, signalIndex) == nullptr)
                             ? (QString())
                             : (mExcelSheet[sheetIndex]->item(row, signalIndex)->text());

        QVariantList inputData = QVariantList({signalName, vehicleType, column, signalIndex});
        createSignal(ivis::common::EventTypeEnum::EventTypeAutoCompleteSuggestions, inputData);
    } else if (supportGenType) {
        updateDialogSelectGenType();
    } else if (supportVehicleType) {
        updateDialogAutoCompleteVehicle();
    } else if (supportConfigName) {
        updateDialogAutoCompleteConfigName();
    } else {
        bool description = (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
        updateAutoCompleteSignal(description, column);
    }
    mExcelSheet[sheetIndex]->clearFocus();
}

void GuiExcel::updateDisplayAutoInputDescrtion() {
    int sheetIndex = getCurrSheetIndex();
    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    if (modelIndexs.size() == 0) {
        qDebug() << "Select cell count : 0";
        return;
    }
    QVariant descrtionInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAutoInputDescriptionInfo);
    int row = modelIndexs.at(0).row();
    int column = modelIndexs.at(0).column() + 1;
    int index = 0;
    for (const auto& info : descrtionInfo.toList()) {
        mExcelSheet[sheetIndex]->setItem(row, column + index++, new QTableWidgetItem(info.toString()));
    }
}

void GuiExcel::updateDisplayGenType(const int& genType) {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetOutputs;
    const int columnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType);

    QStringList genTypeNames("");    // GenTypeName index 와 genType 값을 동일하게 구성하기 위해서
    genTypeNames.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeGenTypeList).toStringList());
    QString currGenTypeName = (genType < genTypeNames.size()) ? (genTypeNames.at(genType)) : ("");

    for (int sheetIndex = startIndex; sheetIndex <= endIndex; ++sheetIndex) {
        if (chcekExcelSheet(sheetIndex) || (mExcelSheet[sheetIndex]->rowCount() == 0)) {
            continue;
        }
        for (int rowIndex = 0; rowIndex < mExcelSheet[sheetIndex]->rowCount(); ++rowIndex) {
            if (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex) == nullptr) {
                continue;
            }
            mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->setText(currGenTypeName);
        }
        // updateSheetHeaderAdjust(sheetIndex, true);  // 편집시 화면 변경 되는 이슈
        syncSheetData(sheetIndex, true);
    }
}

void GuiExcel::updateDisplayTCCheck(const int& allCheck) {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetOutputs;

    for (int sheetIndex = startIndex; sheetIndex <= endIndex; ++sheetIndex) {
        setSheetCheckState(sheetIndex, (!allCheck));
#if 1
        updateDisplaySheetCheckState(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
#else
        emit mExcelSheet[sheetIndex]->horizontalHeader()->sectionClicked(
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
#endif
    }
}

void GuiExcel::updateDisplayReceiveKeyFocus() {
    int sheetIndex = getCurrSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    mExcelSheet[sheetIndex]->setFocus();  // Sheet 활성화 상태에서 화면 전화시 focus 설정 되도록
}

void GuiExcel::updateDisplayShortcut(const int& shortcutType) {
    int sheetIndex = getCurrSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    setCellEditSkip(true);

    int columIndex = (-1);
    switch (shortcutType) {
        case ivis::common::ShortcutTypeEnum::ShortcutTypeInsert: {
            columIndex = updateShortcutInsertDelete(sheetIndex, true);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeDelete: {
            columIndex = updateShortcutInsertDelete(sheetIndex, false);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit: {
            columIndex = updateShortcutMergeSplit(sheetIndex);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeCopy: {
            updateShortcutCopy(sheetIndex, false);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeCut: {
            updateShortcutCopy(sheetIndex, true);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypePaste: {
            columIndex = updateShortcutPaste(sheetIndex);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeUndo: {
            qDebug() << "\t Function implementation required : Undo";
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeRedo: {
            qDebug() << "\t Function implementation required : Redo";
            break;
        }
        default: {
            qDebug() << "Not support shortcut type :" << shortcutType;
            break;
        }
    }

    if (columIndex >= 0) {
        // Data Sync : Gui -> Control (아래 순서 보장 되어야함. 병합 정보 바탕으로 자동완성 재구성)
        // 1. syncSheetData()
        // 2. createSignal(EventTypeUpdateAutoCompleteData)
        syncSheetData(sheetIndex, false);
        updateSheetHeaderAdjust(sheetIndex, false);    // Resize Header Size : Row(O), Column(X)
        createSignal(ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData, QVariantList({sheetIndex, columIndex}));
    }
    setCellEditSkip(false);
}

void GuiExcel::updateDisplaySheetCheckState(const int& sheetIndex, const int& columnIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }
    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs)) {
        return;
    }
    if (columnIndex != static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)) {
        return;
    }
    if (mExcelSheet[sheetIndex]->rowCount() == 0) {
        return;
    }

    bool allCheck = getSheetCheckState(sheetIndex);
    for (int rowIndex = 0; rowIndex < mExcelSheet[sheetIndex]->rowCount(); ++rowIndex) {
        if (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex) == nullptr) {
            continue;
        }
        Qt::CheckState checkState = (allCheck) ? (Qt::CheckState::Unchecked) : (Qt::CheckState::Checked);
        mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->setCheckState(checkState);
    }
    setSheetCheckState(sheetIndex, (!allCheck));
}

void GuiExcel::updateDisplayExcelSheet() {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    bool excelOpen = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
    qDebug() << "Excel Update :" << ((excelOpen) ? ("Open") : ("New")) << ", Size :" << mExcelSheet.size();

    // Clear - Table, TableWidget, CellInfo, Menu, Action
    updateInitExcelSheet();
    // qDebug() << "\t Excel : init";

    // Create : Action Item
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCut] = mMenuRight->addAction("Cut");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCopy] = mMenuRight->addAction("Copy");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypePaste] = mMenuRight->addAction("Paste");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert] = mMenuRight->addAction("Insert");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeDelete] = mMenuRight->addAction("Delete");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit] = mMenuRight->addAction("Merge/Split");
    // qDebug() << "\t Excel : action item";

    // Draw - Sheet
    QStringList sheetName = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName).toStringList();
    QStringList descTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle).toStringList();
    QStringList configTitle =
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle).toStringList();
    QStringList otherTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle).toStringList();
    QVariantList listRowCount = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount).toList();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    // qDebug() << "\t Excel : read data";

    for (const auto& sheet : sheetName) {
        QStringList contentTitle;
        if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
            contentTitle = descTitle;
        } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
            contentTitle = configTitle;
        } else {
            contentTitle = otherTitle;
        }
        int rowMax = listRowCount.at(sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription).toInt();
        int columnMax = contentTitle.size();

        // Draw Sheet
        mExcelSheet[sheetIndex] = new QTableWidget(rowMax, columnMax);
        mExcelSheet[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        // mExcelSheet[sheetIndex]->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
        mExcelSheet[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mGui->TabWidget->addTab(mExcelSheet[sheetIndex], sheet);
        // qDebug() << "\t\t Sheet[" << sheetIndex << "] : draw sheet";

        // Check State Init
        setSheetCheckState(sheetIndex, false);

        // Draw Sheet Data
        if (excelOpen) {
            updateSheetProperty(sheetIndex);
        } else {
            updateSheetNew(sheetIndex, rowMax, columnMax);
        }
        updateSheetDefaultFocus(sheetIndex, 0, 0);
        updateCellContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
        // qDebug() << "\t\t Sheet[" << sheetIndex << "] : draw sheet data";

        connect(mExcelSheet[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            // qDebug() << "1 cellChanged :" << row << column;
            updateDisplayCellDataInfo(sheetIndex, row, column);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellDoubleClicked, [=](int row, int column) {
            // qDebug() << "2 cellDoubleClicked :" << row << column;
            updateDisplayAutoComplete(sheetIndex, row, column);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellClicked, [=](int row, int column) {
            // qDebug() << "3 cellClicked :" << row << column;
            updateCellContent(sheetIndex, row, column);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint& pos) {
            QAction* selectAction = mMenuRight->exec(mExcelSheet[sheetIndex]->mapToGlobal(QPoint((pos.x() + 20), (pos.y() + 5))));
            int shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInvalid;

            if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCut]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeCut;
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCopy]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeCopy;
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypePaste]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypePaste;
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInsert;
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeDelete]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeDelete;
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit;
            } else {
                qDebug() << "Fail to menu right selection action item";
                return;
            }

            updateDisplayShortcut(shortcutType);
        });
        connect(mExcelSheet[sheetIndex]->horizontalHeader(), &QHeaderView::sectionClicked, [=](int logicalIndex) {
            updateDisplaySheetCheckState(sheetIndex, logicalIndex);
        });
#if 0
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
            qDebug() << "4 itemClicked[" << sheetIndex << "] :" << item;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemActivated, [=](QTableWidgetItem *item) {
            qDebug() << "5 itemActivated[" << sheetIndex << "] :" << item;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemEntered, [=](QTableWidgetItem *item) {
            qDebug() << "6 itemEntered[" << sheetIndex << "] :" << item;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellEntered, [=](int row, int column) {
            qDebug() << "7 cellEntered[" << sheetIndex << "] :" << row << column;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellActivated, [=](int row, int column) {
            qDebug() << "8 cellActivated[" << sheetIndex << "] :" << row << column;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemSelectionChanged, [=]() {
            qDebug() << "9 itemSelectionChanged[" << sheetIndex << "]";
            QString text = QString();
            if (mExcelSheet[sheetIndex]->currentItem()) {
                text = mExcelSheet[sheetIndex]->currentItem()->text();
            }
            mGui->CellInfoContent->setText(text);
        });
#if 0
        connect(mExcelSheet[sheetIndex]->verticalHeader(), &QHeaderView::sectionClicked, [=](int logicalIndex) {
            qDebug() << "2. sectionClicked_V[" << sheetIndex << "] :" << logicalIndex;
        });
        connect(mExcelSheet[sheetIndex]->horizontalHeader(), &QHeaderView::sectionResized,
                                                                [=](int logicalIndex, int oldSize, int newSize) {
            qDebug() << "11. sectionResized_V[" << sheetIndex << "] :" << logicalIndex << oldSize << newSize;
        });
        connect(mExcelSheet[sheetIndex]->verticalHeader(), &QHeaderView::sectionResized,
                                                                [=](int logicalIndex, int oldSize, int newSize) {
            qDebug() << "12. sectionResized_V[" << sheetIndex << "] :" << logicalIndex << oldSize << newSize;
        });
#endif
#endif

        // Resize - Cell Width/Height
        updateSheetHeaderAdjust(sheetIndex, true);
        sheetIndex++;
    }

    // qDebug() << "\t Excel : complete";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

void GuiExcel::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth: {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize: {
            updateDisplaySize();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible: {
            updateDisplayVisible();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen: {
            updateDisplayExcelSheet();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeShortcutType: {
            int shortcutType = value.toInt();
            updateDisplayShortcut(shortcutType);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeKey: {
            int keyValue = value.toInt();
            updateDisplayKey(keyValue);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeArrowKey: {
            int keyValue = value.toInt();
            updateDisplayArrowKey(keyValue);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeReceiveKeyFocus: {
            updateDisplayReceiveKeyFocus();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeValueEnum: {
            updateDialogValueEnum(value.toList());
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult: {
            updateDialogTCNameResult(value.toStringList());
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeAutoInputDescriptionInfo: {
            updateDisplayAutoInputDescrtion();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeGenType: {
            updateDisplayGenType(value.toInt());
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTCCheck: {
            updateDisplayTCCheck(value.toInt());
            break;
        }
        default: {
            break;
        }
    }
}
