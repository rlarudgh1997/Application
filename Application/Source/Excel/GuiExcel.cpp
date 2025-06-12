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

    const auto shortcutInfo = QMap<int, QString>({
        {ivis::common::ShortcutTypeEnum::ShortcutTypeCut, QString("Cut")},
        {ivis::common::ShortcutTypeEnum::ShortcutTypeCopy, QString("Copy")},
        {ivis::common::ShortcutTypeEnum::ShortcutTypePaste, QString("Paste")},
        {ivis::common::ShortcutTypeEnum::ShortcutTypeInsert, QString("Insert")},
        {ivis::common::ShortcutTypeEnum::ShortcutTypeDelete, QString("Delete")},
        {ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit, QString("Merge/Split")},
    });
    writeShortcutInfo(shortcutInfo);
}

void GuiExcel::drawDisplayDepth0() {
    updateDisplayVisible();

    connect(mGui->CellInfoContent, &QLineEdit::returnPressed, [=]() {
        int sheetIndex = getCurrSheetIndex();
        if (checkExcelSheet(sheetIndex) == false) {
            return;
        }
        QTableWidget* currentSheet = mExcelSheet[sheetIndex];
        if (currentSheet->currentItem() == nullptr) {
            return;
        }

        int rowIndex = currentSheet->currentRow();
        int columnIndex = currentSheet->currentColumn();
        QString text = mGui->CellInfoContent->text();

        if (isCheckState(sheetIndex, rowIndex, columnIndex) == static_cast<int>(ivis::common::CheckState::Invalid)) {
            currentSheet->currentItem()->setText(text);
        } else {
            currentSheet->currentItem()->setText("");
            currentSheet->currentItem()->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            currentSheet->currentItem()->setCheckState((text.isEmpty()) ? (Qt::Unchecked) : (Qt::Checked));
        }
        updateSheetDefaultFocus(sheetIndex, rowIndex, columnIndex);
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
        connect(mDialog.data(), &Dialog::signalScrollBarValueChanged, [=](const int& value) {
            setScrolBarValue(value);
        });
        connect(mDialog.data(), &Dialog::signalSelectRadioValue, [=](const QString& text) {
            updateSelectedCellItem(text);
        });
        connect(mDialog.data(), &Dialog::signalSelectListItem, [=](const QList<QPair<int, QString>>& selectItem) {
            int dialogType = mDialog.data()->getProperty(Dialog::DataTypeDialogType).toInt();
            if (dialogType == Dialog::DialogTypeSelectMoudleInfo) {
                QStringList selectModule;
                for (const auto& select : selectItem) {
                    selectModule.append(select.second);
                }
                if (selectModule.size() == 0) {
                    createSignal(ivis::common::EventTypeEnum::EventTypeSelectModuleError);
                } else {
                    createSignal(ivis::common::EventTypeEnum::EventTypeSelectModule, selectModule);
                }
            } else {
                QStringList selectValues;
                for (const auto& select : selectItem) {
                    if (dialogType == Dialog::DialogTypeSelectValueResult) {
                        selectValues.append(select.second);
                    } else {
                        QStringList lineStr = select.second.split(":");
                        if (lineStr.size() != 2) {
                            continue;
                        }

                        QString temp = getSfcSignal() ? lineStr.at(0) : lineStr.at(1);
                        if (getOutputState()) {
                            temp = getSfcSignal() ? lineStr.at(1) : lineStr.at(0);
                        }
                        temp.remove("\"");

                        selectValues.append(temp);
                    }
                }
                QString selectValueEnum = selectValues.join(", ");
                updateSelectedCellItem(selectValueEnum);
            }
            mDialog.data()->accept();
        });
        connect(mDialog.data(), &Dialog::signalSelectOption,
                [=](const bool& option1, const QList<QPair<QString, bool>>& option2) {
                    QStringList vehicleTypes;
                    for (const auto& check : option2) {
                        if (check.second) {
                            vehicleTypes.append(check.first);
                        }
                    }
                    QString vehicleType = vehicleTypes.join(", ");
                    updateSelectedCellItem(vehicleType);
                });
        connect(mDialog.data(), &Dialog::signalAutoCompleteSelected, [=](const QString& text) {
#if 1
            updateSelectedCellItem(text);
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
            updateSelectedCellItem(singalName);
#endif
        });
    }

    if (dialogType != Dialog::DialogTypeInvalid) {
        mDialog.data()->drawDialog(dialogType, info);
    }
}

void GuiExcel::updateDialogCycleMode(const QString& text) {
    auto cycleMode = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeManualCycleMode).toStringList();
    int currentIndex = (cycleMode.indexOf(text));
    int max = cycleMode.size();
    ivis::common::LIMIT(currentIndex, 0, max);

    QVariantList info = QVariantList({
        QString("Select Cycle Mode"),
        currentIndex,
        cycleMode,
    });
    updateDrawDialog(Dialog::DialogTypeCycleMode, info);
}

void GuiExcel::updateDialogAutoCompleteVehicle() {
    QVariantList info = QVariantList({
        QString("Select Vehicle"),
        QString(),
        false,
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeSelectVehicleType, info);
}

void GuiExcel::updateDialogAutoCompleteConfigName() {
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    QString currentCellText = mGui->CellInfoContent->text();
    ivis::common::getRemoved(currentCellText, mergeInfos);

    // qDebug() << "updateDialogAutoCompleteConfigName";
    // qDebug() << "\t Config :" << isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressConfigName);

    QVariantList info = QVariantList({
        QString("Select Config"),
        currentCellText,
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressConfigName).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeAutoCompleteNormal, info);
}

void GuiExcel::updateDialogAutoCompleteSignal(const int& sheetIndex, const int& columnIndex) {
    QVariantList keywordTypeInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeKeywordTypeInfo).toList();
    QStringList keywordList;

    if ((sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) &&
        (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) &&
        (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn) &&
        (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC) &&
        (columnIndex < keywordTypeInfo.size())) {
        keywordList = keywordTypeInfo.at(columnIndex).toStringList();
    }

    QStringList useOnlykeywordList;
    if ((sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) &&
        (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) &&
        (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn)) {
        useOnlykeywordList =
            keywordTypeInfo.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)).toStringList();
    }
    // qDebug() << "updateDialogAutoCompleteSignal :" << columnIndex;
    // qDebug() << "\t Keyword :" << keyworedIndex << keywordTypeInfo.size() << keywordList.size();

    QString currentCellText = mGui->CellInfoContent->text();
    ivis::common::getRemoved(currentCellText, keywordList);

    auto sfcList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC);
    auto vsmList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM);
    auto tcNameList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressTCName);
    auto dependentList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressDependentName);

    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC)) {
        tcNameList.clear();
        dependentList.clear();
    }

    QVariantList info = QVariantList({
        QString("Auto Complete"),
        currentCellText,
        sfcList.toStringList(),
        vsmList.toStringList(),
        tcNameList.toStringList(),
        dependentList.toStringList(),
        keywordList,
        useOnlykeywordList
    });
    updateDrawDialog(Dialog::DialogTypeAutoComplete, info);
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

void GuiExcel::updateDialogSelectModule() {
    QVariantList info = QVariantList({
        QString("Select Module"),
        QStringList({"Module"}),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList).toStringList(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule).toStringList(),
        QVariantList(),
        getScrolBarValue(),
    });
    updateDrawDialog(Dialog::DialogTypeSelectMoudleInfo, info);
}

bool GuiExcel::isSheetContentChanged(const int& sheetIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
        return false;
    }

    QList<QStringList> propertySheetData = isSheetData(sheetIndex, true, false);
    QList<QStringList> displaySheetData = isSheetData(sheetIndex, false, false);

    // qDebug() << "isSheetContentChanged :" << sheetIndex;
    // qDebug() << "\t Sheet Changed :" << (propertySheetData != displaySheetData);

    return (propertySheetData != displaySheetData);
}

int GuiExcel::isCheckState(const int& sheetIndex, const int& rowIndex, const int& columnIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
        return static_cast<int>(ivis::common::CheckState::Invalid);
    }

    bool checkVisible = false;
    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn)) {
        checkVisible = false;
    } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC) {
        checkVisible = ((columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::RunnableOpt)) ||
                      (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::ConfigOpt)) ||
                      // (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleMode)) ||
                      (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleOption)));
    } else {
        checkVisible = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
    }

    if (checkVisible == false) {
        return static_cast<int>(ivis::common::CheckState::Invalid);
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    QTableWidgetItem* currentItem = currentSheet->item(rowIndex, columnIndex);
    if (currentItem == nullptr) {
        return static_cast<int>(ivis::common::CheckState::Invalid);
    }

    // qDebug() << sheetIndex <<".isCheckState[" << rowIndex << columnIndex << "] :" << currentItem->checkState()
    //          << currentItem->text();
    if ((currentItem->checkState() == Qt::Checked) || (currentItem->text().size() > 0)) {
        return static_cast<int>(ivis::common::CheckState::Checked);
    } else {
        return static_cast<int>(ivis::common::CheckState::Unchecked);
    }
}

QList<QStringList> GuiExcel::isSheetData(const int& sheetIndex, const bool& readProperty, const bool& removeMerge) {
    if (checkExcelSheet(sheetIndex) == false) {
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
    if (checkExcelSheet(sheetIndex) == false) {
        return QString();
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    if ((rowIndex >= currentSheet->rowCount()) || (columnIndex >= currentSheet->columnCount())) {
        return QString();
    }

    QTableWidgetItem* currentItem = currentSheet->item(rowIndex, columnIndex);
    if (currentItem == nullptr) {
        currentItem = new QTableWidgetItem("");
        currentSheet->setItem(rowIndex, columnIndex, currentItem);
    }

    QString text = currentItem->text();
    int checkState = isCheckState(sheetIndex, rowIndex, columnIndex);
    if (checkState != static_cast<int>(ivis::common::CheckState::Invalid)) {
        text = (checkState == static_cast<int>(ivis::common::CheckState::Checked)) ? (QString("O")) : (QString(""));
    }
    return text;
}

QMap<int, QSet<QPair<int, int>>> GuiExcel::isPropertyMergeInfo(const int& sheetIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
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
    if (checkExcelSheet(sheetIndex) == false) {
        return QMap<int, QList<QPair<int, int>>>();
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    int rowStart = 0;
    int columnStart = 0;
    int rowCount = currentSheet->rowCount();
    int columnCount = currentSheet->columnCount();
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
            int mergeCount = currentSheet->rowSpan((rowStart + rowIndex), (columnStart + columnIndex));
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
    if ((sheetIndex < ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex >= ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax)) {
        // qDebug() << "Fail to - sheet index invalid :" << sheetIndex;
        return false;
    }

    if (mGui->TabWidget == nullptr) {
        qDebug() << "Fail to - Gui->TabWidget nullptr";
        return false;
    }

    if (mExcelSheet.size() == 0) {
        qDebug() << "Fail to - excel sheet was not created";
        return false;
    }

    if (mExcelSheet[sheetIndex] == nullptr) {
        qDebug() << "Fail to - excel sheet nullptr :" << sheetIndex;
        return false;
    }

    return true;
}

QVariantList GuiExcel::readSheetDisplay(const int& sheetIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
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
    if (checkExcelSheet(sheetIndex) == false) {
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

void GuiExcel::refreshAutoCompleteData(const int& sheetIndex, const int& rowIndex, const int& columnIndex) {
    int eventType = ivis::common::EventTypeEnum::EventTypeInvalid;
    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Test)) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteNameDescription;
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName)) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteName;
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData)) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData;
            } else {
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::TCName)) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteName;
            } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputData)) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData;
            } else {
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC: {
            if ((columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionValue)) ||
                (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputValue))||
                (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitValue))||
                (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputValue))) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData;
            }
            break;
        }
        default: {
            if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteName;
            } else if ((columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) ||
                       (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue))) {
                eventType = ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData;
            } else {
            }
            break;
        }
    }

    if (eventType != ivis::common::EventTypeEnum::EventTypeInvalid) {
        // qDebug() << "refreshAutoCompleteData :" << sheetIndex << rowIndex << columnIndex << eventType;
        createSignal(eventType, QVariantList({sheetIndex, rowIndex, columnIndex}));
    }
}

void GuiExcel::parsingShortcutType(const int& sheetIndex, const QPoint& position) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    QAction* selectAction = nullptr;
    if (mMenuRight) {
        selectAction = mMenuRight->exec(currentSheet->mapToGlobal(QPoint(position.x() + 20, position.y() + 5)));
    }
    if (selectAction == nullptr) {
        return;
    }

    const QString selectedActionText = selectAction->text();
    const auto shortcutInfo = readShortcutInfo();
    for (const auto& shortcutType : shortcutInfo.keys()) {
        QString shortcutName = shortcutInfo[shortcutType];
        if (selectedActionText == shortcutName) {
            updateDisplayShortcut(shortcutType);
            break;
        }
    }
}

void GuiExcel::screenUpdateBlock(const int& sheetIndex, const bool& block) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];

    currentSheet->blockSignals(block);
    currentSheet->setUpdatesEnabled(!block);
    if (block == false) {
        currentSheet->viewport()->update();
    }
}

void GuiExcel::updateEditSheetData(const int& sheetIndex, const QList<QStringList>& previousData, const QVariantList& data) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }
    QList<QStringList> currentData;
    for (const auto& rowData : data) {
        currentData.append(rowData.toStringList());
    }
    if (previousData == currentData) {
        return;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];

    const int rowIndex = currentSheet->currentRow();
    const int columnIndex = currentSheet->currentColumn();
    SheetData* sheetData = new SheetData(sheetIndex, rowIndex, columnIndex, previousData, currentData);
    connect(sheetData, &SheetData::signalSheetDataUndo, this, &GuiExcel::slotSheetDataUndo, Qt::UniqueConnection);
    mUndoStack->push(sheetData);
}

int GuiExcel::updateShortcutInsertDelete(const int& sheetIndex, const bool& insertState) {
    if (checkExcelSheet(sheetIndex) == false) {
        return (-1);
    }
    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    const CellSelectedInfo cellSelectedInfo(currentSheet);
    if ((insertState == false) && (cellSelectedInfo.isEmpty())) {
        return (-1);
    }

    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowMax = cellSelectedInfo.getRowMax();
    const int columnStart = cellSelectedInfo.getColumnStart();
    const int columnMax = cellSelectedInfo.getColumnMax();

    // qDebug() << "updateShortcutInsertDelete :" << sheetIndex << insertState << rowStart << rowEnd << rowMax;

    for (int rowIndex = rowStart; rowIndex <= rowEnd; ++rowIndex) {
        if (insertState) {
            currentSheet->insertRow(rowStart);
            for (int columnIndex = 0; columnIndex < currentSheet->columnCount(); ++columnIndex) {
                updateCurrentCellText(sheetIndex, rowStart, columnIndex, QString());
            }
            if (rowMax == 0) {
                updateSheetDefaultFocus(sheetIndex, 0, columnStart);
            }
        } else {
            currentSheet->removeRow(rowStart);
            if (currentSheet->rowCount() == 0) {
                setSheetCheckState(sheetIndex, false);
            } else {
                updateSheetDefaultFocus(sheetIndex, (rowStart - 1), columnStart);
            }
        }
    }
    for (int columnIndex = columnStart; columnIndex < columnMax; ++columnIndex) {
        updateSheetHorizontalHeaderFixed(sheetIndex, columnIndex, 0);
    }

    return columnStart;
}

int GuiExcel::updateShortcutMergeSplit(const int& sheetIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
        return (-1);
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    const CellSelectedInfo cellSelectedInfo(currentSheet);
    if (cellSelectedInfo.isEmpty()) {
        return (-1);
    }

    const int columnStart = cellSelectedInfo.getColumnStart();
    const int columnCount = cellSelectedInfo.getColumnCount();
    bool notSupport = false;

    if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC) {
        notSupport = (columnStart >= static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleMode));
    } else {
        notSupport = (columnStart >= static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
    }
    notSupport = (columnCount > 1);

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
    const int rowSpan = currentSheet->rowSpan(rowStart, columnStart);
    const int columnSpan = currentSheet->columnSpan(rowStart, columnStart);
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
    } else if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC) &&
        (columnStart < static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionSignal))) {
        for (int index = 0; index < static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionSignal); ++index) {
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
        currentSheet->setSpan(rowStart, columnIndex, ((mergedState) ? (1) : (rowCount)), 1);
    }
    updateSheetDefaultFocus(sheetIndex, rowStart, columnStart);
    // updateSheetHeaderAdjust(sheetIndex, false, columnStart);

    return columnStart;
}

int GuiExcel::updateShortcutCopy(const int& sheetIndex, const bool& cutState) {
    if (checkExcelSheet(sheetIndex) == false) {
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

        QString text = currCell.data().toString();    // 선택한 셀에서 텍스트 추출
        if (isCheckState(sheetIndex, cellRowIndex, cellColumnIndex) != static_cast<int>(ivis::common::CheckState::Invalid)) {
            text = isCurrentCellText(sheetIndex, cellRowIndex, cellColumnIndex);
        }
        copyDataList[rowIndex].append(text);
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
    if (checkExcelSheet(sheetIndex) == false) {
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

    // Construct Info : Current Selected Cell Info
    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowCount = cellSelectedInfo.getRowCount();
    const int columnStart = cellSelectedInfo.getColumnStart();
    const int columnEnd = cellSelectedInfo.getColumnEnd();
    const int columnCount = cellSelectedInfo.getColumnCount();
    const int rowMax = cellSelectedInfo.getRowMax();
    const int columnMax = cellSelectedInfo.getColumnMax();

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

    // Clipboard Data
    QString clipboardText = QApplication::clipboard()->mimeData()->text();
    if (clipboardText.endsWith("\n")) {
        clipboardText.chop(1);
    }
    const QStringList clipboardData = clipboardText.split("\n");
    const int clipboardDataMax = clipboardData.size() + rowStart;  // Row : 0(Header), 1 ~ Count(Item)

    // Update : Row Count (붙여넣기시 Row 마지막을 넘는 경우 Row Count 증가)
    if (clipboardDataMax >= rowMax) {
        updateRowMax(sheetIndex, rowMax, clipboardDataMax);
    }

    // qDebug() << "===============================================================================================";
    // qDebug() << "updateShortcutPaste :" << sheetIndex << copySheetIndex;
    // qDebug() << "\t Row           :" << rowStart << rowEnd << rowCount;
    // qDebug() << "\t Column        :" << columnStart << columnEnd << columnCount;
    // qDebug() << "\t Max           :" << rowMax << columnMax << clipboardDataMax;
    // qDebug() << "\t CopyRow       :" << copyRowStart << copyRowEnd << copyRowCount;
    // qDebug() << "\t CopyColumn    :" << copyColumnStart << copyColumnEnd << copyColumnCount;
    // qDebug() << "\t ClipboardData :" << clipboardData.size() << clipboardData;
    // qDebug() << "\t -----------------------------------------------------------------------------------------";

    // Clear : Cut Selected Cell Backgorund/Text/Merge Info
    if (checkExcelSheet(copySheetIndex) == false) {
        qDebug() << "Data copied from an external program.";
    } else {
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
    QMap<int, int> updateAutoCompleteInfo;
    for (int cellRowIndex = 0; cellRowIndex < clipboardData.size(); ++cellRowIndex) {
        QStringList rowDataList = clipboardData[cellRowIndex].split("\t");
        for (int cellColumnIndex = 0; cellColumnIndex < rowDataList.size(); ++cellColumnIndex) {
            int rowIndex = (rowStart + cellRowIndex);
            int columnIndex = (columnStart + cellColumnIndex);
            QString text = rowDataList[cellColumnIndex];
            updateCurrentCellText(sheetIndex, rowIndex, columnIndex, text);
            updateSheetHeaderAdjust(sheetIndex, false, columnIndex);

            bool update = false;    // 붙여넣기 후 자동완성 정보 갱신이 필요한 컬럼을 구성
            if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
                update = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Test));
            } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
                update = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
            } else {
                update = (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
            }
            if (update) {
                updateAutoCompleteInfo[sheetIndex] = columnIndex;
            }
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
    for (const auto& key : updateAutoCompleteInfo.keys()) {
        qDebug() << "Refresh Autocomplete data after pasting :" << key << rowStart << updateAutoCompleteInfo[key];
        refreshAutoCompleteData(key, rowStart, updateAutoCompleteInfo[key]);
    }

    return columnStart;
}

void GuiExcel::updateShortcutUndoRedo(const int& sheetIndex, const bool& undoState) {
    if (checkExcelSheet(sheetIndex) == false) {
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
    if (checkExcelSheet(sheetIndex) == false) {
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
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    mExcelSheet[sheetIndex]->setRowCount(changeRowMax);

    // qDebug() << "updateRowMax :" << sheetIndex << rowMax << changeRowMax << mExcelSheet[sheetIndex]->rowCount();

    for (int rowIndex = rowMax; rowIndex < changeRowMax; ++rowIndex) {
        updateCurrentCellText(sheetIndex, rowIndex, 0, QString());
    }
}

void GuiExcel::updateSheetHeaderAdjust(const int& sheetIndex, const bool& resizeColumn, const int& columnIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    if (resizeColumn) {
        currentSheet->resizeColumnsToContents();
    }

    if (columnIndex >= 0) {
        currentSheet->resizeColumnToContents(columnIndex);
    }

    currentSheet->resizeRowsToContents();
    // qDebug() << "updateSheetHeaderAdjust :" << sheetIndex << resizeColumn << columnIndex;
}

void GuiExcel::updateSheetHorizontalHeaderFixed(const int& sheetIndex, const int& columnIndex, const int& width) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }
    if (getColumnHeaderResize()) {
        return;  // resizeSection() 호출로 인한 무한 루프 방지
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    int defaultWidth = 64;

    for (int row = 0; row < currentSheet->rowCount(); ++row) {
        QTableWidgetItem* item = currentSheet->item(row, columnIndex);
        if (item) {
            QFontMetrics font(item->font());
            int itemContentWidth = 0;
            for (const auto& text : item->text().split("\n")) {
                itemContentWidth = std::max(itemContentWidth, font.horizontalAdvance(text));
            }
            defaultWidth = std::max(defaultWidth, itemContentWidth) + 1;
        }
    }
    // int columnWidth = currentSheet->columnWidth(columnIndex);
    // int sectionSize = currentSheet->horizontalHeader()->sectionSizeHint(columnIndex);

    if (width < defaultWidth) {
        setColumnHeaderResize(true);
        currentSheet->horizontalHeader()->resizeSection(columnIndex, defaultWidth);
        setColumnHeaderResize(false);
    }
}

void GuiExcel::updateSheetDefaultFocus(const int& sheetIndex, const int& row, const int& column) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
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

    currentSheet->clearSelection();
    currentSheet->setCurrentCell(rowStart, column);
    if (rowEnd > rowStart) {    // 병합된 셀 범위
        for (int index = rowStart; index <= rowEnd; ++index) {
            if (currentSheet->item(index, column) == nullptr) {
                currentSheet->setItem(index, column, new QTableWidgetItem(""));
            }
            currentSheet->item(index, column)->setSelected(true);
        }
    }
    currentSheet->setFocus();

    // Clear Focus : Current Cell Content
    mGui->CellInfoContent->clearFocus();
}

void GuiExcel::updateSelectedCellItem(const QString& text) {
    if (mSelectItem == nullptr) {
        return;
    }
    if (text.size() == 0) {
        return;
    }

    mSelectItem->setText(text);
    mGui->CellInfoContent->setText(text);
    qDebug() << "updateSelectedCellItem :" << text;
}

void GuiExcel::updateCellContent(const int& sheetIndex, const int& rowIndex, const int& columnIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    mModelIndex[sheetIndex] = QPair<int, int>(rowIndex, columnIndex);
    int currSheetIndex = getCurrSheetIndex();
    // qDebug() << "updateCellContent :" << sheetIndex << mModelIndex[sheetIndex] << sheetIndex << currSheetIndex;

    if (currSheetIndex == sheetIndex) {
        QString text = isCurrentCellText(sheetIndex, rowIndex, columnIndex);
        // qDebug() << "\t Info :" << currSheetIndex << sheetIndex << rowIndex << columnIndex << text;
        mGui->CellInfoContent->setText(text);
        mSelectItem = updateCurrentCellText(sheetIndex, rowIndex, columnIndex, text);
    }
}

QTableWidgetItem* GuiExcel::updateCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex,
                                                  const QString& text) {
    if (checkExcelSheet(sheetIndex) == false) {
        return nullptr;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    if ((rowIndex >= currentSheet->rowCount()) || (columnIndex >= currentSheet->columnCount())) {
        return nullptr;
    }

    QTableWidgetItem* currentItem = currentSheet->item(rowIndex, columnIndex);
    if (currentItem == nullptr) {
        currentItem = new QTableWidgetItem();
        currentSheet->setItem(rowIndex, columnIndex, currentItem);
    }
    currentItem->setText(text);

    int checkState = isCheckState(sheetIndex, rowIndex, columnIndex);
    if (checkState != static_cast<int>(ivis::common::CheckState::Invalid)) {
        currentItem->setText("");
        currentItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        currentItem->setCheckState((checkState == static_cast<int>(ivis::common::CheckState::Checked) ?
                                   (Qt::Checked) : (Qt::Unchecked)));
    }

    // qDebug() << "updateCurrentCellText :" << sheetIndex << rowIndex << columnIndex << text << currentItem->text();
    return currentItem;
}

void GuiExcel::updateCheckState(const int& sheetIndex, const int& columnIndex, const bool& confirmSheetCheck) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }
    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn)) {
        return;
    } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC) {
        if ((columnIndex != static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::RunnableOpt)) &&
            (columnIndex != static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::ConfigOpt)) &&
            // (columnIndex != static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleMode)) &&
            (columnIndex != static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleOption))) {
            return;
        }
    } else {
        if (columnIndex != static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)) {
            return;
        }
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    if (currentSheet->rowCount() == 0) {
        return;
    }

    bool allCheck = true;
    if (confirmSheetCheck) {
        const auto mergeInfo = isDisplayMergeInfo(sheetIndex);
        for (int rowIndex = 0; rowIndex < currentSheet->rowCount(); ++rowIndex) {
            int checkState = isCheckState(sheetIndex, rowIndex, columnIndex);
            if (checkState != static_cast<int>(ivis::common::CheckState::Checked)) {
                allCheck = false;
                break;
            }
            for (const auto& merge : mergeInfo[columnIndex]) {
                if ((rowIndex >= merge.first) && (rowIndex <= merge.second)) {
                    rowIndex = merge.second;
                    break;
                }
            }
        }
    } else {
        allCheck = getSheetCheckState(sheetIndex);
    }

    screenUpdateBlock(sheetIndex, true);
    for (int rowIndex = 0; rowIndex < currentSheet->rowCount(); ++rowIndex) {
        QTableWidgetItem* currentItem = currentSheet->item(rowIndex, columnIndex);
        if (currentItem == nullptr) {
            continue;
        }
        // currentItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        currentItem->setCheckState((allCheck) ? (Qt::CheckState::Unchecked) : (Qt::CheckState::Checked));
    }
    screenUpdateBlock(sheetIndex, false);
    setSheetCheckState(sheetIndex, (!allCheck));

    syncSheetData(sheetIndex);
}

void GuiExcel::updateSheetProperty(const int& sheetIndex, const int& viewSheetIndex) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }
    ivis::common::CheckTimer checkTimer;

    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    // const QVariantList sheetData = readSheetProperty(sheetIndex);
    const QList<QStringList> sheetData = isSheetData(sheetIndex, true, true);

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];

    // Update : Sheet -> Block
    screenUpdateBlock(sheetIndex, true);
    int rowIndex = 0;
    currentSheet->setRowCount(sheetData.size());
    for (const auto& rowDataList : sheetData) {
        int columnIndex = 0;
        for (const auto& text : rowDataList) {
            updateCurrentCellText(sheetIndex, rowIndex, columnIndex, text);
            columnIndex++;
        }
        rowIndex++;
    }
    // Update : Sheet
    screenUpdateBlock(sheetIndex, false);

    // Update : Merge Info
    const auto& mergeInfo = isPropertyMergeInfo(sheetIndex);
    currentSheet->clearSpans();
    for (const auto& columnIndex : mergeInfo.keys()) {
        for (const auto& info : mergeInfo[columnIndex]) {
            int mergeRowStart = info.first;
            int mergeRowEnd = info.second - mergeRowStart + 1;
            currentSheet->setSpan(mergeRowStart, columnIndex, mergeRowEnd, 1);
        }
    }

    // Update : Current Sheet Index
    int currentIndex = viewSheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    mGui->TabWidget->setCurrentIndex(currentIndex);
}

void GuiExcel::updateInitExcelSheet() {
    setCurrSheetIndex(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    mModelIndex.clear();

    if (mUndoStack) {
        mUndoStack->clear();
    }

    if (mGui->CellInfoContent) {
        mGui->CellInfoContent->clear();
    }

    for (const auto& sheet : mExcelSheet.values()) {
        if (sheet == nullptr) {
            continue;
        }
        disconnect(sheet, nullptr, nullptr, nullptr);
        sheet->clear();
        delete sheet;
    }
    mExcelSheet.clear();

    if (mMenuRight) {
        for (const auto& action : mMenuActionItem.values()) {
            if (action == nullptr) {
                continue;
            }
            delete action;
        }
        mMenuActionItem.clear();
        mMenuRight->clear();
    }

    if (mGui->TabWidget) {
        for (int index = 0; index < mGui->TabWidget->count(); index++) {
            QWidget* currentWidget = mGui->TabWidget->widget(index);
            if (currentWidget == nullptr) {
                continue;
            }
            mGui->TabWidget->removeTab(mGui->TabWidget->indexOf(currentWidget));
            delete currentWidget;
        }
        mGui->TabWidget->clear();
    }
}

void GuiExcel::updateDisplayKey(const int& keyValue) {
    int sheetIndex = getCurrSheetIndex();
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }
    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    int rowIndex = currentSheet->currentRow();
    int columnIndex = currentSheet->currentColumn();
    int rowMax = currentSheet->rowCount();
    int columnMax = currentSheet->columnCount();
    QTableWidgetItem* currentItem = currentSheet->item(rowIndex, columnIndex);
    if (currentItem == nullptr) {
        return;
    }

    // qDebug() << "updateDisplayKey :" << keyValue << columnIndex << rowIndex << rowMax << columnMax;

    switch (keyValue) {
        case ivis::common::KeyTypeEnum::KeyInputValueCancel: {
            updateClearClipboard(getCopyInfo(), false);
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueDelete: {
            QModelIndexList modelIndexs = currentSheet->selectionModel()->selectedIndexes();
            if (modelIndexs.size() > 0) {
                setCellEditSkip(true);
                for (const auto& model : modelIndexs) {
                    int currRowIndex = model.row();
                    int currColumnIndex = model.column();
                    updateCurrentCellText(sheetIndex, currRowIndex, currColumnIndex, QString());
                }
                updateCellContent(sheetIndex, rowIndex, columnIndex);
                setCellEditSkip(false);
                syncSheetData(sheetIndex);
            }
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueOK: {
            // currentSheet->closePersistentEditor(currentItem);
            auto mergeInfo = isDisplayMergeInfo(sheetIndex);
            for (const auto& merge : mergeInfo[columnIndex]) {
                if ((rowIndex >= merge.first) && (rowIndex <= merge.second)) {
                    rowIndex = merge.second;
                    break;
                }
            }
            rowIndex++;
            if (rowIndex < rowMax) {
                updateCellContent(sheetIndex, rowIndex, columnIndex);
                updateSheetDefaultFocus(sheetIndex, rowIndex, columnIndex);
            }
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueUp:
        case ivis::common::KeyTypeEnum::KeyInputValueDown:
        case ivis::common::KeyTypeEnum::KeyInputValueLeft:
        case ivis::common::KeyTypeEnum::KeyInputValueRight: {
            // currentSheet->closePersistentEditor(currentItem);
            updateCellContent(sheetIndex, rowIndex, columnIndex);
            break;
        }
        default: {
            break;
        }
    }
}

void GuiExcel::updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    if (isSheetContentChanged(sheetIndex) == false) {
        // qDebug() << "The excel sheet has not been modified.";
        return;
    }

    if (getCellEditSkip()) {
        // qDebug() << "Skip cell editing event handling.";
        return;
    }

    updateSheetHeaderAdjust(sheetIndex, false, column);
    syncSheetData(sheetIndex);

    bool supportAutoComplete = false;    // 자동완성 데이터 재구성
    bool checkValidation = false;    // 입력 데이터 유효성 검사

    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription: {
            supportAutoComplete = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Description::Test));
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs: {
            supportAutoComplete = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
            checkValidation = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn: {
            supportAutoComplete = (column == static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::TCName));
            checkValidation = (column == static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputData));
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC: {
            checkValidation = ((column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionValue)) ||
                               (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputValue)) ||
                               (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitValue)) ||
                               (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputValue)));
            break;
        }
        default: {
            supportAutoComplete = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
            checkValidation = ((column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) ||
                               (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)));
            break;
        }
    }

    // qDebug() << "updateDisplayCellDataInfo :" << sheetIndex << row << column;

    if (supportAutoComplete) {
        refreshAutoCompleteData(sheetIndex, row, column);
    }
    if (checkValidation) {
        QVariantList currentCellInfo = QVariantList({sheetIndex, row, column});
        createSignal(ivis::common::EventTypeEnum::EventTypeCheckValidation, currentCellInfo);
    }
}

void GuiExcel::updateDisplayAutoComplete(const int& sheetIndex, const int& row, const int& column) {
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    ivis::common::AutoCompleteEnum::AutoComplete autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Invalid;
    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription: {
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs: {
            if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Signal;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Data;
            } else {
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn: {
            if (column == static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputSignal)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Signal;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputData)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Data;
            } else {
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC: {
            if (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::VehicleType)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::VehicleType;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleMode)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::CycleMode;
            } else if ((column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionSignal)) ||
                (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputSignal)) ||
                (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitSignal)) ||
                (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputSignal))) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Signal;
            } else if ((column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionValue)) ||
                (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputValue)) ||
                (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitValue)) ||
                (column == static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputValue))) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Data;
            } else {
            }
            break;
        }
        default: {
            if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::GenType;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::VehicleType;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::ConfigName;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Signal;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Signal;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Data;
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
                autoCompleteType = ivis::common::AutoCompleteEnum::AutoComplete::Data;
            } else {
            }
            break;
        }
    }

    // qDebug() << "updateDisplayAutoComplete";
    // qDebug() << "\t Info :" << sheetIndex << row << column;
    // qDebug() << "\t Type :" << autoCompleteType;

    switch (autoCompleteType) {
        case ivis::common::AutoCompleteEnum::AutoComplete::GenType: {
            updateDialogSelectGenType();
            break;
        }
        case ivis::common::AutoCompleteEnum::AutoComplete::VehicleType: {
            updateDialogAutoCompleteVehicle();
            break;
        }
        case ivis::common::AutoCompleteEnum::AutoComplete::ConfigName: {
            updateDialogAutoCompleteConfigName();
            break;
        }
        case ivis::common::AutoCompleteEnum::AutoComplete::Signal: {
            updateDialogAutoCompleteSignal(sheetIndex, column);
            break;
        }
        case ivis::common::AutoCompleteEnum::AutoComplete::Data: {
            refreshAutoCompleteData(sheetIndex, row, column);
            break;
        }
        case ivis::common::AutoCompleteEnum::AutoComplete::CycleMode: {
            updateDialogCycleMode(isCurrentCellText(sheetIndex, row, column));
            break;
        }
        default: {
            break;
        }
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    currentSheet->clearFocus();

    updateClearClipboard(getCopyInfo(), false);
}

void GuiExcel::updateDisplayAutoInputDescrtion() {
    int sheetIndex = getCurrSheetIndex();
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }

    QTableWidget* currentSheet = mExcelSheet[sheetIndex];
    QModelIndexList modelIndexs = currentSheet->selectionModel()->selectedIndexes();
    if (modelIndexs.size() == 0) {
        qDebug() << "Select cell count : 0";
        return;
    }
    QVariant descrtionInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAutoInputDescriptionInfo);
    int rowIndex = modelIndexs.at(0).row();
    int column = modelIndexs.at(0).column() + 1;
    int index = 0;
    for (const auto& info : descrtionInfo.toList()) {
        int columnIndex = (column + index);
        updateCurrentCellText(sheetIndex, rowIndex, columnIndex, info.toString());
        updateSheetHorizontalHeaderFixed(sheetIndex, columnIndex, 0);
        // currentSheet->setItem(rowIndex, columnIndex, new QTableWidgetItem(info.toString()));
        index++;
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
        if (checkExcelSheet(sheetIndex) == false) {
            continue;
        }

        QTableWidget* currentSheet = mExcelSheet[sheetIndex];
        screenUpdateBlock(sheetIndex, true);
        for (int rowIndex = 0; rowIndex < currentSheet->rowCount(); ++rowIndex) {
            if (currentSheet->item(rowIndex, columnIndex) == nullptr) {
                continue;
            }
            currentSheet->item(rowIndex, columnIndex)->setText(currGenTypeName);
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
        updateCheckState(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check), false);
    }
}

void GuiExcel::updateDisplayShortcut(const int& shortcutType) {
    int sheetIndex = getCurrSheetIndex();
    if (checkExcelSheet(sheetIndex) == false) {
        return;
    }
    // qDebug() << "updateDisplayShortcut :" << shortcutType;

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
        syncSheetData(sheetIndex);
        updateSheetHeaderAdjust(sheetIndex, false);
    }
    setCellEditSkip(false);
}

void GuiExcel::updateDisplayExcelSheet() {
    qDebug() << (QString(100, '>').toLatin1().data());
    const int excelView = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelView).toInt();
    const int startSheet = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    const int endSheet = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;
    const int sheetMax = (endSheet - startSheet);

    qDebug() << "Update Gui Excel :" << excelView << sheetMax << mExcelSheet.size();
    if (mExcelSheet.size() > sheetMax) {
        qDebug() << "\t Exceeded the standard sheet size :" << sheetMax << "->" << mExcelSheet.size();
    }

    // Init
    updateInitExcelSheet();

    // Create : Right Menu(Action Item)
    if (mMenuRight) {
        const auto shortcutInfo = readShortcutInfo();
        for (const auto& shortcutName : shortcutInfo.values()) {
            QAction* action = mMenuRight->addAction(shortcutName);
            // if (shortcutName == "Cut") {
            //     action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
            //     mMenuRight->setActiveAction(action);
            // }
        }
    }

    // Draw : Sheet
    auto sheetName = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName);
    auto descTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle);
    auto configTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelConfigTitle);
    auto dependentOnTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDependentOnTitle);
    auto manualTCTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelManualTCTitle);
    auto otherTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle);
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    QList<int> sheetIndexList;

    for (const auto& sheet : sheetName.toStringList()) {
        QStringList contentTitle;
        if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
            contentTitle = descTitle.toStringList();
        } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
            contentTitle = configTitle.toStringList();
        } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn) {
            contentTitle = dependentOnTitle.toStringList();
        } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC) {
            contentTitle = manualTCTitle.toStringList();
        } else {
            contentTitle = otherTitle.toStringList();
        }
        int columnMax = contentTitle.size();
        // qDebug() << "sheet[" << columnMax << "] :" << contentTitle;

        // Draw Sheet
        mExcelSheet[sheetIndex] = new QTableWidget(0, columnMax);
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
        if (checkExcelSheet(sheetIndex) == false) {
            return;
        }
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            updateDisplayCellDataInfo(sheetIndex, row, column);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellClicked, [=](int row, int column) {
            updateCellContent(sheetIndex, row, column);
            updateSheetDefaultFocus(sheetIndex, row, column);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellDoubleClicked, [=](int row, int column) {
            updateDisplayAutoComplete(sheetIndex, row, column);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint& pos) {
            parsingShortcutType(sheetIndex, pos);
        });
        connect(mExcelSheet[sheetIndex]->horizontalHeader(), &QHeaderView::sectionDoubleClicked, [=](int logicalIndex) {
            updateCheckState(sheetIndex, logicalIndex, true);
        });
        connect(mExcelSheet[sheetIndex]->horizontalHeader(), &QHeaderView::sectionResized,
                [=](int logicalIndex, int oldSize, int newSize) {
            updateSheetHorizontalHeaderFixed(sheetIndex, logicalIndex, newSize);
        });
    }
    qDebug() << (QString(100, '<').toLatin1().data());
}

void GuiExcel::slotSheetDataUndo(const int& sheetIndex, const int& row, const int& column, const QList<QStringList>& data) {
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
        case ivis::common::PropertyTypeEnum::PropertyTypeExcelView: {
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
        case ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule: {
            updateDialogSelectModule();
            break;
        }
        default: {
            break;
        }
    }
}
