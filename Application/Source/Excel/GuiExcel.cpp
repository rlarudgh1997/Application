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

    mUndoStack = new QUndoStack(isHandler()->getScreen());
    mUndoStack->setUndoLimit(100);
}

void GuiExcel::drawDisplayDepth0() {
    updateDisplayVisible();

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
    if (checkExcelSheet(sheetIndex)) {
        return false;
    }

    QList<QStringList> propertySheetData = isSheetData(sheetIndex, true, false);
    QList<QStringList> displaySheetData = isSheetData(sheetIndex, false, false);

    // qDebug() << "isSheetContentChanged :" << sheetIndex;
    // qDebug() << "\t Sheet Changed :" << (propertySheetData != displaySheetData);

    return (propertySheetData != displaySheetData);
}

bool GuiExcel::isDrawCheckBox(const int& sheetIndex, const int& columnIndex) {
    if (checkExcelSheet(sheetIndex)) {
        return false;
    }
    bool draw = ((sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) &&
                 (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) &&
                 (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)));
    return draw;
}

QList<QStringList> GuiExcel::isSheetData(const int& sheetIndex, const bool& readProperty, const bool& removeMerge) {
    if (checkExcelSheet(sheetIndex)) {
        return QList<QStringList>();
    }
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const int rowMax = mExcelSheet[sheetIndex]->rowCount();
    const int columnMax = mExcelSheet[sheetIndex]->columnCount();
    const auto mergeInfo = isDisplayMergeInfo(sheetIndex);

    QVariantList readSheetData;

    if (readProperty) {
        readSheetData = isHandler()->getProperty(sheetIndex).toList();
    } else {
        for (int rowIndex = 0; rowIndex < rowMax; ++rowIndex) {
            QStringList rowData;
            for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
                QString text = isCurrentCellText(sheetIndex, rowIndex, columnIndex);
                if (removeMerge == false) {
                    for (const auto& info : mergeInfo[columnIndex]) {
                        if (rowIndex == info.first) {
                            text.prepend(mergeStart);
                        } else if (rowIndex == info.second) {
                            text.prepend(mergeEnd);
                        } else if ((rowIndex > info.first) && (rowIndex < info.second)) {
                            text.prepend(merge);
                        } else {
                        }
                    }
                }
                rowData.append(text);
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
    qDebug() << "isSheetData :" << sheetIndex << removeMerge << ", SheetData :" << ((readProperty) ? ("Property") : ("Display"));
    for (const auto& rowDataList : sheetData) {
        qDebug() << "\t RowData :" << rowDataList;
    }
    qDebug() << "------------------------------------------------------------------------------------------";
    // for (const auto& key : mergeInfo.keys()) {
    //     qDebug() << "\t MergeInfo[" << key << "]  :" << mergeInfo[key];
    // }
    // qDebug() << "------------------------------------------------------------------------------------------";
#endif

    return sheetData;
}

QString GuiExcel::isCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex) {
    if (checkExcelSheet(sheetIndex)) {
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
    if (checkExcelSheet(sheetIndex)) {
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

    // TCName 병합 정보가 있는 경우 Check, GenType, VehicleType, Config 도 동일한 병합 정보로 구성 하도록 예외처리
    if ((sheetIndex >= ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates) &&
        (sheetIndex < ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs)) {
        if (mergeInfo.contains(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))) {
            QSet<QPair<int, int>> tcNameMergeInfo = mergeInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)];
            mergeInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)] = tcNameMergeInfo;
            mergeInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)] = tcNameMergeInfo;
            mergeInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)] = tcNameMergeInfo;
            mergeInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)] = tcNameMergeInfo;
        }
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
    if (checkExcelSheet(sheetIndex)) {
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

bool GuiExcel::checkExcelSheet(const int& sheetIndex) {
    if (mGui->TabWidget == nullptr) {
        qDebug() << "Fail to - Gui->TabWidget nullptr";
        return true;
    }

    if (mExcelSheet.size() == 0) {
        qDebug() << "Fail to - excel sheet was not created";
        return true;
    }

    if (mExcelSheet[sheetIndex] == nullptr) {
        qDebug() << "Fail to - excel sheet nullptr :" << sheetIndex;
        return true;
    }

    return false;
}

QVariantList GuiExcel::readSheetDisplay(const int& sheetIndex) {
    if (checkExcelSheet(sheetIndex)) {
        qDebug() << "Fail to display sheet nullptr : " << sheetIndex;
        return QVariantList();
    }

    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const auto mergeInfo = isDisplayMergeInfo(sheetIndex);

    // Construct : Current Display Sheet Data
    QList<QStringList> displaySheetData = isSheetData(sheetIndex, false, true);
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

void GuiExcel::syncSheetData(const int& sheetIndex, const QVariantList& undoRedoData) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    const bool readProperty = (undoRedoData.size() == 0);
    const int eventType = ivis::common::EventTypeEnum::EventTypeListDescription +
                          (sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    QVariantList sheetData = (readProperty) ? (readSheetDisplay(sheetIndex)) : (undoRedoData);
    // qDebug() << "syncSheetData :" << sheetIndex << readProperty << eventType << sheetData.size();

    if (readProperty) {
        // Bakcup : Edit Data, Current Data
        QList<QStringList> previousSheetData = isSheetData(sheetIndex, true, false);
        updateEditSheetData(sheetIndex, previousSheetData, sheetData);
    }

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

void GuiExcel::screenUpdateBlock(const int& sheetIndex, const bool& block) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    mExcelSheet[sheetIndex]->blockSignals(block);
    mExcelSheet[sheetIndex]->setUpdatesEnabled(!block);
    if (block == false) {
        mExcelSheet[sheetIndex]->viewport()->update();
    }
}

void GuiExcel::updateEditSheetData(const int& sheetIndex, const QList<QStringList>& previousData, const QVariantList& data) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }
    QList<QStringList> currentData;
    for (const auto& rowData : data) {
        currentData.append(rowData.toStringList());
    }
    if (previousData == currentData) {
        return;
    }

    const int rowIndex = mExcelSheet[sheetIndex]->currentRow();
    const int columnIndex = mExcelSheet[sheetIndex]->currentColumn();
    SheetData* sheetData = new SheetData(sheetIndex, rowIndex, columnIndex, previousData, currentData);
    connect(sheetData, &SheetData::signalSheetDataUndo, this, &GuiExcel::slotSheetDataUndo, Qt::UniqueConnection);
    mUndoStack->push(sheetData);
}

int GuiExcel::updateShortcutInsertDelete(const int& sheetIndex, const bool& insertState) {
    if (checkExcelSheet(sheetIndex)) {
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(mExcelSheet[sheetIndex]);
    if ((insertState == false) && (cellSelectedInfo.isEmpty())) {
        return (-1);
    }

    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowMax = cellSelectedInfo.getRowMax();
    const int columnStart = cellSelectedInfo.getColumnStart();

    // qDebug() << "updateShortcutInsertDelete :" << sheetIndex << insertState << rowStart << rowEnd << rowMax;

    for (int index = rowStart; index <= rowEnd; ++index) {
        if (insertState) {
            mExcelSheet[sheetIndex]->insertRow(rowStart);
            for (int columnIndex = 0; columnIndex < mExcelSheet[sheetIndex]->columnCount(); ++columnIndex) {
                QTableWidgetItem* item = new QTableWidgetItem();
                if (isDrawCheckBox(sheetIndex, columnIndex)) {
                    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                    item->setCheckState(Qt::Unchecked);
                }
                mExcelSheet[sheetIndex]->setItem(rowStart, columnIndex, item);
            }
            if (rowMax == 0) {
                updateSheetDefaultFocus(sheetIndex, 0, columnStart);
            }
        } else {
            mExcelSheet[sheetIndex]->removeRow(rowStart);
            if (mExcelSheet[sheetIndex]->rowCount() == 0) {
                setSheetCheckState(sheetIndex, false);
            } else {
                updateSheetDefaultFocus(sheetIndex, (rowStart - 1), columnStart);
            }
        }
    }

    return columnStart;
}

int GuiExcel::updateShortcutMergeSplit(const int& sheetIndex) {
    if (checkExcelSheet(sheetIndex)) {
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(mExcelSheet[sheetIndex]);
    if (cellSelectedInfo.isEmpty()) {
        return (-1);
    }

    const int columnStart = cellSelectedInfo.getColumnStart();
    const int columnCount = cellSelectedInfo.getColumnCount();
    bool notSupport = (columnCount > 1) || (columnStart >= static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));

    if (notSupport) {  // 병합은 Column 기준 1개만 지원
        qDebug() << "Not support : multilple slected column.";
        createSignal(ivis::common::EventTypeEnum::EventTypeWarningMergeSplit,
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
        return (-1);
    }

    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowCount = cellSelectedInfo.getRowCount();
    // const int columnEnd = cellSelectedInfo.getColumnEnd();
    const int rowSpan = mExcelSheet[sheetIndex]->rowSpan(rowStart, columnStart);
    const int columnSpan = mExcelSheet[sheetIndex]->columnSpan(rowStart, columnStart);
    const bool mergedState = ((rowSpan > 1) || (columnSpan > 1));
    const auto mergeInfo = isDisplayMergeInfo(sheetIndex);

    int checkRangeIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
    if (columnStart == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)) {
        checkRangeIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName);
    } else if (columnStart == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
        checkRangeIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result);
    } else {
    }

    if ((mergedState == false) && (checkRangeIndex != static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
        notSupport = false;
        for (const auto& merge : mergeInfo[checkRangeIndex]) {
            if ((rowStart >= merge.first) && (rowEnd <= merge.second)) {
                notSupport = false;    // 병합 가능 : TCName, Result 병합된 범위에 포함되면
                break;
            }
            if ((rowStart <= merge.second) && (rowEnd >= merge.first)) {
                notSupport = true;    // 병합 불가 : rowStart, rowEnd 중 하나가 Name, Result 병합된 범위와 겹치면
            }
        }
        if (notSupport) {
            qDebug() << "Not support : not merged based on TCName or Result.";
            createSignal(ivis::common::EventTypeEnum::EventTypeWarningMergeSplit, checkRangeIndex);
            return (-1);
        }
    }

    // Column : Check ~ Config 는 동일한 병합 정보로 구성되어야함
    QList<int> columnList;
    if ((sheetIndex > ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) &&
        (sheetIndex < ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) &&
        (columnStart < static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))) {
        for (int index = 0; index < static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result); ++index) {
            columnList.append(index);
        }
    } else {
        columnList.append(columnStart);
    }

    // qDebug() << "===============================================================================================";
    // qDebug() << "updateShortcutMergeSplit :" << sheetIndex;
    // qDebug() << "\t Row         :" << rowStart << rowEnd << rowCount;
    // qDebug() << "\t Column      :" << columnStart << columnEnd << columnCount;
    // qDebug() << "\t MergedState :" << mergedState << rowSpan << columnSpan;
    // qDebug() << "\t MergeInfo   :" << mergeInfo[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)];
    // qDebug() << "\t ColumnList  :" << columnList;
    // qDebug() << "\t -----------------------------------------------------------------------------------------";

    for (const auto& columnIndex : columnList) {
        mExcelSheet[sheetIndex]->setSpan(rowStart, columnIndex, ((mergedState) ? (1) : (rowCount)), 1);
    }
    updateSheetDefaultFocus(sheetIndex, rowStart, columnStart);

    return columnStart;
}

int GuiExcel::updateShortcutCopy(const int& sheetIndex, const bool& cutState) {
    if (checkExcelSheet(sheetIndex)) {
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

    // qDebug() << "===============================================================================================";
    // qDebug() << "updateShortcutCopy :" << sheetIndex << cutState;
    // qDebug() << "\t Row    :" << rowStart << rowEnd << rowCount;
    // qDebug() << "\t Column :" << columnStart << columnEnd << columnCount;
    // qDebug() << "\t Max    :" << rowMax << columnMax;
    // qDebug() << "\t -----------------------------------------------------------------------------------------";

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
    if (checkExcelSheet(sheetIndex)) {
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

    // qDebug() << "===============================================================================================";
    // qDebug() << "updateShortcutPaste :" << sheetIndex << copySheetIndex;
    // qDebug() << "\t Row           :" << rowStart << rowEnd << rowCount;
    // qDebug() << "\t Column        :" << columnStart << columnEnd << columnCount;
    // qDebug() << "\t Max           :" << rowMax << columnMax << clipboardDataMax;
    // qDebug() << "\t ClipboardData :" << clipboardData.size() << clipboardData;
    // qDebug() << "\t CopyRow       :" << copyRowStart << copyRowEnd << copyRowCount;
    // qDebug() << "\t CopyColumn    :" << copyColumnStart << copyColumnEnd << copyColumnCount;
    // qDebug() << "\t -----------------------------------------------------------------------------------------";

    // Clear : Cut Selected Cell Backgorund/Text/Merge Info
    if (checkExcelSheet(copySheetIndex) == false) {
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
        const auto copyMergeInfo = isDisplayMergeInfo(copySheetIndex);
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
            QString text = rowDataList[columnIndex];
            mExcelSheet[sheetIndex]->setItem(cellRowIndex, cellColumnIndex, new QTableWidgetItem(text));
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
    syncSheetData(sheetIndex);
    if (sheetIndex != copySheetIndex) {
        syncSheetData(copySheetIndex);
    }

    // Update : AutoComplete Info
    for (const auto& columnIndex : columnIndexList) {
        syncAutoComplete(sheetIndex, columnIndex);
    }

    setCellEditSkip(false);

    return columnStart;
}

void GuiExcel::updateShortcutUndoRedo(const int& sheetIndex, const bool& undoState) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }
    if (mUndoStack == nullptr) {
        return;
    }

    // qDebug() << "updateShortcutUndoRedo :" << sheetIndex << undoState << mUndoStack->count();
    if (undoState) {
        mUndoStack->undo();
    } else {
        mUndoStack->redo();
    }
}

void GuiExcel::updateClearClipboard(const CellSelectedInfo& clearInfo, const bool& cellHighlight) {
    int sheetIndex = clearInfo.getSheetIndex();
    if (checkExcelSheet(sheetIndex)) {
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
    if (checkExcelSheet(sheetIndex)) {
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

void GuiExcel::updateSheetHeaderAdjust(const int& sheetIndex, const bool& resizeColumn, const int& columnIndex) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    if (resizeColumn) {
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
    }

    if (columnIndex >= 0) {
        mExcelSheet[sheetIndex]->resizeColumnToContents(columnIndex);
    }

    mExcelSheet[sheetIndex]->resizeRowsToContents();
}

void GuiExcel::updateSheetDefaultFocus(const int& sheetIndex, const int& row, const int& column) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    auto mergeInfo = isDisplayMergeInfo(sheetIndex);
    int rowStart = (row < 0) ? (0) : (row);
    int rowEnd = 0;

    // qDebug() << "updateSheetDefaultFocus :" << column << rowStart << rowEnd;
    for (const auto& merge : mergeInfo[column]) {
        if ((rowStart >= merge.first) && (rowStart <= merge.second)) {
            rowStart = merge.first;
            rowEnd = merge.second;
            // qDebug() << "\t MergeInfo[" << column << "]  :" << merge.first << merge.second;
            break;
        }
    }

    mExcelSheet[sheetIndex]->clearSelection();
    mExcelSheet[sheetIndex]->setCurrentCell(rowStart, column);
    if (rowEnd > rowStart) {    // 병합된 셀 범위
        for (int index = rowStart; index <= rowEnd; ++index) {
            if (mExcelSheet[sheetIndex]->item(index, column) == nullptr) {
                mExcelSheet[sheetIndex]->setItem(index, column, new QTableWidgetItem(""));
            }
            mExcelSheet[sheetIndex]->item(index, column)->setSelected(true);
        }
    }
    mExcelSheet[sheetIndex]->setFocus();
}

void GuiExcel::updateCellContent(const int& sheetIndex, const int& row, const int& column) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    mModelIndex[sheetIndex] = QPair<int, int>(row, column);
    // qDebug() << "updateCellContent :" << sheetIndex << mModelIndex[sheetIndex];

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

void GuiExcel::updateCheckState(const int& sheetIndex, const int& columnIndex) {
    if (checkExcelSheet(sheetIndex)) {
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
    screenUpdateBlock(sheetIndex, true);
    for (int rowIndex = 0; rowIndex < mExcelSheet[sheetIndex]->rowCount(); ++rowIndex) {
        if (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex) == nullptr) {
            continue;
        }
        Qt::CheckState checkState = (allCheck) ? (Qt::CheckState::Unchecked) : (Qt::CheckState::Checked);
        mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->setCheckState(checkState);
    }
    screenUpdateBlock(sheetIndex, false);
    setSheetCheckState(sheetIndex, (!allCheck));
}

void GuiExcel::updateSheetProperty(const int& sheetIndex, const int& viewSheetIndex) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }
    ivis::common::CheckTimer checkTimer;

    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    // const QVariantList sheetData = readSheetProperty(sheetIndex);
    const QList<QStringList> sheetData = isSheetData(sheetIndex, true, true);

    // Update : Sheet -> Block
    screenUpdateBlock(sheetIndex, true);
    int rowIndex = 0;
    mExcelSheet[sheetIndex]->setRowCount(sheetData.size());
    for (const auto& rowDataList : sheetData) {
        int columnIndex = 0;
        for (const auto& columnData : rowDataList) {
            QTableWidgetItem* item = new QTableWidgetItem();
            if (isDrawCheckBox(sheetIndex, columnIndex)) {
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                item->setCheckState((columnData.isEmpty() == false) ? (Qt::Checked) : (Qt::Unchecked));
            } else {
                item->setText(columnData);
            }
            mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, item);
            columnIndex++;
        }
        rowIndex++;
    }
    // Update : Sheet
    screenUpdateBlock(sheetIndex, false);

    // Update : Merge Info
    const auto& mergeInfo = isPropertyMergeInfo(sheetIndex);
    mExcelSheet[sheetIndex]->clearSpans();
    for (const auto& columnIndex : mergeInfo.keys()) {
        for (const auto& info : mergeInfo[columnIndex]) {
            int mergeRowStart = info.first;
            int mergeRowEnd = info.second - mergeRowStart + 1;
            mExcelSheet[sheetIndex]->setSpan(mergeRowStart, columnIndex, mergeRowEnd, 1);
        }
    }

    // Update : Current Sheet Index
    int currentIndex = viewSheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    mGui->TabWidget->setCurrentIndex(currentIndex);
}

void GuiExcel::updateInitExcelSheet() {
    setCurrSheetIndex(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);

    for (const auto& action : mMenuActionItem) {
        delete action;
    }
    mMenuActionItem.clear();
    mMenuRight->clear();
    mModelIndex.clear();
    mUndoStack->clear();
    mGui->CellInfoContent->clear();

    for (const auto& sheet : mExcelSheet) {
        // disconnect(sheet);
        // disconnect(sheet, &QTableWidget::itemSelectionChanged, nullptr, nullptr);
        disconnect(sheet, nullptr, nullptr, nullptr);
        sheet->clear();
        delete sheet;
    }
    mExcelSheet.clear();

    for (int index = 0; index < mGui->TabWidget->count(); index++) {
        QWidget* currentWidget = mGui->TabWidget->widget(index);
        if (currentWidget) {
            mGui->TabWidget->removeTab(mGui->TabWidget->indexOf(currentWidget));
            delete currentWidget;
        }
    }
    mGui->TabWidget->clear();
}

void GuiExcel::updateDescriptionInfo(const int& sheetIndex, const int& row) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    QString moduleName =
        (mExcelSheet[sheetIndex]->item(row, 0) == nullptr) ? ("") : (mExcelSheet[sheetIndex]->item(row, 0)->text());
    QString version = (mExcelSheet[sheetIndex]->item(row, 1) == nullptr) ? ("") : (mExcelSheet[sheetIndex]->item(row, 1)->text());
    QString description =
        (mExcelSheet[sheetIndex]->item(row, 2) == nullptr) ? ("") : (mExcelSheet[sheetIndex]->item(row, 2)->text());
    if ((moduleName.size() == 0) || (version.size() > 0) || (description.size() > 0)) {
        // qDebug() << "Fail to text :" << moduleName.size() << version.size() << description.size();
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
    if (checkExcelSheet(sheetIndex)) {
        return;
    }
    int rowIndex = mExcelSheet[sheetIndex]->currentRow();
    int columnIndex = mExcelSheet[sheetIndex]->currentColumn();
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();

    // qDebug() << "updateDisplayKey :" << keyValue << columnIndex << rowIndex << rowMax << columnMax;

    switch (keyValue) {
        case ivis::common::KeyTypeEnum::KeyInputValueCancel: {
            updateClearClipboard(getCopyInfo(), false);
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueDelete: {
            QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
            if (modelIndexs.size() > 0) {
                for (const auto& model : modelIndexs) {
                    if (mExcelSheet[sheetIndex]->item(model.row(), model.column()) == nullptr) {
                        mExcelSheet[sheetIndex]->setItem(model.row(), model.column(), new QTableWidgetItem(""));
                    }
                    mExcelSheet[sheetIndex]->item(model.row(), model.column())->setText("");
                }
                updateCellContent(sheetIndex, rowIndex, columnIndex);
            }
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueOK: {
            mExcelSheet[sheetIndex]->closePersistentEditor(mExcelSheet[sheetIndex]->item(rowIndex, columnIndex));
            auto mergeInfo = isDisplayMergeInfo(sheetIndex);
            for (const auto& merge : mergeInfo[columnIndex]) {
                if ((rowIndex >= merge.first) && (rowIndex <= merge.second)) {
                    rowIndex = merge.second;
                    break;
                }
            }
            rowIndex++;
            if (rowIndex < rowMax) {
                updateSheetDefaultFocus(sheetIndex, rowIndex, columnIndex);
                updateCellContent(sheetIndex, rowIndex, columnIndex);
            }
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueUp:
        case ivis::common::KeyTypeEnum::KeyInputValueDown:
        case ivis::common::KeyTypeEnum::KeyInputValueLeft:
        case ivis::common::KeyTypeEnum::KeyInputValueRight: {
            mExcelSheet[sheetIndex]->closePersistentEditor(mExcelSheet[sheetIndex]->item(rowIndex, columnIndex));
            updateCellContent(sheetIndex, rowIndex, columnIndex);
            break;
        }
        default: {
            break;
        }
    }
}

void GuiExcel::updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column) {
    if (checkExcelSheet(sheetIndex)) {
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

    if (columnIndex >= 0) {    // Cell Input Data Validation
        QString signal = (mExcelSheet[sheetIndex]->item(row, columnIndex) == nullptr)
                             ? (QString())
                             : (mExcelSheet[sheetIndex]->item(row, columnIndex)->text());
        QVariantList cellDataInfo = QVariantList({signal, text, sheetIndex, row, column});
        createSignal(ivis::common::EventTypeEnum::EventTypeCellDataValidation, cellDataInfo);
    }

    updateSheetHeaderAdjust(sheetIndex, false, column);  // 편집시 화면 변경 되는 이슈
    syncSheetData(sheetIndex);
    if (updateAutoCompleteData) {
        createSignal(ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData, QVariantList({sheetIndex, column}));
    }
}

void GuiExcel::updateDisplayAutoComplete(const int& sheetIndex, const int& row, const int& column) {
    if (checkExcelSheet(sheetIndex)) {
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
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

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

    QStringList genTypeNames("");    // GenTypeName index = 0(Invalid) 와 genType 값을 동일하게 구성하기 위해서
    genTypeNames.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeGenTypeList).toStringList());
    QString currGenTypeName = (genType < genTypeNames.size()) ? (genTypeNames.at(genType)) : ("");

    for (int sheetIndex = startIndex; sheetIndex <= endIndex; ++sheetIndex) {
        if (checkExcelSheet(sheetIndex)) {
            continue;
        }

        screenUpdateBlock(sheetIndex, true);
        for (int rowIndex = 0; rowIndex < mExcelSheet[sheetIndex]->rowCount(); ++rowIndex) {
            if (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex) == nullptr) {
                continue;
            }
            mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->setText(currGenTypeName);
        }
        screenUpdateBlock(sheetIndex, false);
        syncSheetData(sheetIndex);
    }
}

void GuiExcel::updateDisplayTCCheck(const int& allCheck) {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetOutputs;

    for (int sheetIndex = startIndex; sheetIndex <= endIndex; ++sheetIndex) {
        setSheetCheckState(sheetIndex, (!allCheck));
#if 1
        updateCheckState(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
        syncSheetData(sheetIndex);
#else
        if (checkExcelSheet(sheetIndex)) {
            continue;
        }
        emit mExcelSheet[sheetIndex]->horizontalHeader()->sectionClicked(
            static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
#endif
    }
}

void GuiExcel::updateDisplayShortcut(const int& shortcutType) {
    int sheetIndex = getCurrSheetIndex();
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    int columIndex = (-1);
    setCellEditSkip(true);
    switch (shortcutType) {
        case ivis::common::ShortcutTypeEnum::ShortcutTypeInsert:
        case ivis::common::ShortcutTypeEnum::ShortcutTypeDelete: {
            bool insertState = (shortcutType == ivis::common::ShortcutTypeEnum::ShortcutTypeInsert);
            columIndex = updateShortcutInsertDelete(sheetIndex, insertState);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit: {
            columIndex = updateShortcutMergeSplit(sheetIndex);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeCopy:
        case ivis::common::ShortcutTypeEnum::ShortcutTypeCut: {
            bool cutState = (shortcutType == ivis::common::ShortcutTypeEnum::ShortcutTypeCut);
            updateShortcutCopy(sheetIndex, cutState);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypePaste: {
            columIndex = updateShortcutPaste(sheetIndex);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeUndo:
        case ivis::common::ShortcutTypeEnum::ShortcutTypeRedo: {
            bool undoState = (shortcutType == ivis::common::ShortcutTypeEnum::ShortcutTypeUndo);
            updateShortcutUndoRedo(sheetIndex, undoState);
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
        syncSheetData(sheetIndex);
        updateSheetHeaderAdjust(sheetIndex, false, columIndex);    // Resize Header Size : Row(O), Column(X)
        createSignal(ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData, QVariantList({sheetIndex, columIndex}));
    }
    setCellEditSkip(false);
}

void GuiExcel::updateDisplayExcelSheet() {
    qDebug() << (QString(100, '>').toLatin1().data());
    bool excelOpen = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
    qDebug() << "Excel Update :" << ((excelOpen) ? ("Open") : ("New")) << ", Size :" << mExcelSheet.size();

    // Init
    updateInitExcelSheet();

    // Create : Action Item
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCut] = mMenuRight->addAction("Cut");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCopy] = mMenuRight->addAction("Copy");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypePaste] = mMenuRight->addAction("Paste");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert] = mMenuRight->addAction("Insert");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeDelete] = mMenuRight->addAction("Delete");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit] = mMenuRight->addAction("Merge/Split");
    // mMenuRight->setActiveAction(mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCut]);

    // Draw : Sheet
    QStringList sheetName = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName).toStringList();
    QStringList descTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle).toStringList();
    QStringList configTitle =
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle).toStringList();
    QStringList otherTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle).toStringList();
    QVariantList listRowCount = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount).toList();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    QList<int> sheetIndexList;

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
        mExcelSheet[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mGui->TabWidget->addTab(mExcelSheet[sheetIndex], sheet);

        // Check State Init
        setSheetCheckState(sheetIndex, false);

        // Draw Sheet Data
        updateSheetProperty(sheetIndex, ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
        updateCellContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
        updateSheetDefaultFocus(sheetIndex, 0, 0);

        // Resize - Cell Width/Height
        updateSheetHeaderAdjust(sheetIndex, true, 0);
        sheetIndexList.append(sheetIndex);
        sheetIndex++;
    }
    // Connect : Signal/Slot
    for (const auto& sheetIndex : sheetIndexList) {
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
            updateSheetDefaultFocus(sheetIndex, row, column);
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
            updateCheckState(sheetIndex, logicalIndex);
        });
    }
    qDebug() << (QString(100, '<').toLatin1().data());
}

void GuiExcel::slotSheetDataUndo(const int& sheetIndex, const int& row, const int& column, const QList<QStringList>& data) {
    if (checkExcelSheet(sheetIndex)) {
        return;
    }

    // qDebug() << "slotSheetDataUndo :" << sheetIndex << row << column << data.size();

    // ivis::common::CheckTimer checkTimer;
    QVariantList undoRedoData;
    for (const auto& rowDataList : data) {
        undoRedoData.append(rowDataList);
    }
    syncSheetData(sheetIndex, undoRedoData);
    // checkTimer.check("slotSheetDataUndo : syncSheetData");

    updateSheetProperty(sheetIndex, sheetIndex);
    updateCellContent(sheetIndex, row, column);
    updateSheetDefaultFocus(sheetIndex, row, column);
    updateSheetHeaderAdjust(sheetIndex, false, column);
    // checkTimer.check("slotSheetDataUndo : updateSheetProperty");
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
            updateDisplayShortcut(value.toInt());
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeKey: {
            updateDisplayKey(value.toInt());
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
