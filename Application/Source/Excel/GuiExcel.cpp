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
            currSheetIndex = currentIndex;
            updateCellInfoContent(currentIndex, mModelIndex[currentIndex].first, mModelIndex[currentIndex].second);
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
    mDialog.data()->drawDialog(dialogType, info);
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

QVariantList GuiExcel::readSheetProperty(const int& sheetIndex, const QVariantList& readIndexInfo, QString& allString) {
    if (chcekExcelSheet(sheetIndex)) {
        qDebug() << "Fail to excel sheet nullptr : " << sheetIndex << ", size :" << mExcelSheet.size();
        return QVariantList();
    }

    QVariantList sheetData = QVariantList();
    int rowStart = 0;
    int columnStart = 0;
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    bool readShortcut = (readIndexInfo.size() > 0);

    if (readShortcut) {
        if (readIndexInfo.size() != 4) {
            qDebug() << "Fail to read index info size :" << readIndexInfo.size();
            return QVariantList();
        }
        rowStart = readIndexInfo.at(0).toInt();
        columnStart = readIndexInfo.at(1).toInt();
        rowMax = readIndexInfo.at(2).toInt();
        columnMax = readIndexInfo.at(3).toInt();
        // qDebug() << "Read Cell Info :" << rowStart << columnStart << rowMax << columnMax;
    }

    // Read Row Data
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const QMap<int, QSet<QPair<int, int>>> mergeInfo = isSheetMergeInfo(sheetIndex, false);

    for (int rowIndex = rowStart; rowIndex < rowMax; rowIndex++) {
        QStringList rowData = QStringList();
        for (int columnIndex = columnStart; columnIndex < columnMax; columnIndex++) {
            QString readText = ((mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)) == nullptr) ? (QString()) :
                                (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->text());
            if (isDrawCheckBox(sheetIndex, columnIndex)) {
                bool appendState = ((mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)) == nullptr) ? (false) :
                                    (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->checkState() == Qt::Checked);
                if (appendState) {
                    readText = QString("O");
                }
            }
            allString.append(QString("%1%2").arg((rowData.size() == 0) ? ("") : ("\t")).arg(readText));

            for (const auto& info : mergeInfo[columnIndex]) {
                if ((rowIndex < info.first) || (rowIndex > info.second)) {
                    continue;
                }
                QString prefixMerge;
                if (rowIndex == info.first) {
                    prefixMerge = mergeStart;
                } else if (rowIndex == info.second) {
                    prefixMerge = mergeEnd;
                } else {
                    prefixMerge = merge;
                }
                readText.prepend(prefixMerge);
                // qDebug() << "\t MergeText[" << columnIndex << rowIndex << "] :"<< info << readText;
            }
            rowData.append(readText);
        }
        sheetData.append(rowData);
        allString.append("\n");
    }

#if 1
    qDebug() << "==================================================================================================";
    qDebug() << "readSheetProperty :"<< sheetIndex << rowMax << columnMax << sheetData.size();
    for (const auto& rowDataList : sheetData.toList()) {
        qDebug() << "\t RowData :" << rowDataList.toStringList();
    }
    qDebug() << "==================================================================================================\n";
#endif

    return sheetData;
}

QVariantList GuiExcel::readSheetDisplay(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        qDebug() << "Fail to display sheet nullptr : " << sheetIndex;
        return QVariantList();
    }

    qDebug() << "********************************************************************************************";
    qDebug() << "readSheetDisplay :"<< sheetIndex;

    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();

    QMap<int, QSet<QPair<int, int>>> mergeInfo = isSheetMergeInfo(sheetIndex, true);

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
    QVariantList sheetData;

    if (readProperty) {
        QString allString;
        sheetData = readSheetProperty(sheetIndex, QVariantList(), allString);
    } else {
        sheetData = readSheetDisplay(sheetIndex);
    }

    qDebug() << "syncSheetData :" << sheetIndex << readProperty << eventType << sheetData.size();
    createSignal(eventType, QVariantList({sheetIndex, sheetData}));
}


bool GuiExcel::isSheetChanged(const int& sheetIndex) {
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

int GuiExcel::isMergeCell(const int& sheetIndex, const int& columnIndex, const int& rowStart) {
    if (chcekExcelSheet(sheetIndex)) {
        return 0;
    }
    return mMergeInfo[sheetIndex].isMergeCellCount(columnIndex, rowStart);
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

bool GuiExcel::updateMergeInfo(const bool& erase, const int& sheetIndex, const int& columnIndex, const int& rowStart,
                               const int& rowEnd) {
    if (chcekExcelSheet(sheetIndex)) {
        return false;
    }

    // qDebug() << "updateMergeInfo :" << erase << "," << sheetIndex << columnIndex << rowStart << rowEnd;
    if ((erase) || (mMergeInfo[sheetIndex].isContains(columnIndex, rowStart, rowEnd))) {
        mMergeInfo[sheetIndex].erase(columnIndex, rowStart, rowEnd);
        return (erase == false);
    }

    mMergeInfo[sheetIndex].insert(columnIndex, rowStart, rowEnd);
    return false;
}

QMap<int, QList<QPair<int, int>>> GuiExcel::findMergeInfo(const QMap<int, QVariantList>& sheetData) {
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    // const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();

    QMap<int, QList<QPair<int, int>>> mergeInfo = QMap<int, QList<QPair<int, int>>>();
    QMapIterator<int, QVariantList> iter(sheetData);
    while (iter.hasNext()) {
        iter.next();
        int rowIndex = 0;
        int mergeColumnIndex = iter.key();
        int mergeRowStart = (-1);
        int mergeRowEnd = (-1);

        for (const auto& v : iter.value()) {
            QString readText = v.toString();
            QStringList startText = readText.split(mergeStart);
            QStringList endText = readText.split(mergeEnd);
            // QStringList mergeText = readText.split(merge);

            if (startText.size() == 2) {
                mergeRowStart = rowIndex;
                mergeRowEnd = (-1);
            } else if (endText.size() == 2) {
                mergeRowEnd = rowIndex - mergeRowStart + 1;
            } else {
            }
            rowIndex++;

            if ((mergeRowStart >= 0) && (mergeRowEnd >= 0)) {
                mergeInfo[mergeColumnIndex].append(QPair<int, int>(mergeRowStart, mergeRowEnd));
                mergeRowStart = (-1);
                mergeRowEnd = (-1);
            }
        }
    }
    return mergeInfo;
}

void GuiExcel::constructMergeInfo(const int& sheetIndex, const QMap<int, QVariantList>& sheetData) {
    QMap<int, QList<QPair<int, int>>> mergeInfo = findMergeInfo(sheetData);
    QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
    while (iter.hasNext()) {
        iter.next();
        int columnIndex = iter.key();
        for (const auto& v : iter.value()) {
            updateMergeInfo(false, sheetIndex, columnIndex, v.first, v.second);
        }
    }
}

void GuiExcel::constructMergeSplitInfo(const QMap<int, QVariantList>& sheetData, const int& rowStart, const int& columnStart) {
    QMapIterator<int, QList<QPair<int, int>>> iter(findMergeInfo(sheetData));
    mClearMergeInfo.clear();
    mCopyMergeInfo.clear();

    while (iter.hasNext()) {
        iter.next();
        int mergeColumnIndex = iter.key();
        for (const auto& v : iter.value()) {
            int mergeRowStart = v.first;
            int mergeRowEnd = v.second;
            int splitColumnIndex = columnStart + mergeColumnIndex;
            int splitRowStart = rowStart + mergeRowStart;
            int splitRowEnd = mergeRowEnd;
            mClearMergeInfo.insert(splitColumnIndex, splitRowStart, splitRowEnd);
            mCopyMergeInfo.insert(mergeColumnIndex, mergeRowStart, mergeRowEnd);
        }
    }
}

void GuiExcel::updateDisplaySplitCell(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    mExcelSheet[sheetIndex]->clearSpans();
}

void GuiExcel::updateDisplayMergeCell(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    // qDebug() << "updateDisplayMergeCell :" << sheetIndex;

    QMap<int, QList<QPair<int, int>>> mergeInfo = mMergeInfo[sheetIndex].isMergeInfo();
    QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    while (iter.hasNext()) {
        iter.next();
        int mergeColumnIndex = iter.key();
        for (const auto& v : iter.value()) {
            if (mExcelSheet[sheetIndex]) {
                int mergeRowStart = v.first;
                int mergeRowEnd = v.second;

                if (mergeColumnIndex >= columnMax) {
                    // qDebug() << "\t SkipMergeCell   :" << sheetIndex << mergeColumnIndex << mergeRowStart << mergeRowEnd
                    //          << ", ColumnMax :" << columnMax;
                    continue;
                }

                for (int clearRowIndex = mergeRowStart + 1; clearRowIndex < (mergeRowStart + mergeRowEnd); clearRowIndex++) {
                    mExcelSheet[sheetIndex]->setItem(clearRowIndex, mergeColumnIndex, new QTableWidgetItem(""));
                }
                // qDebug() << "\t UpdateMergeCell :" << sheetIndex << mergeColumnIndex << mergeRowStart << mergeRowEnd;
                mExcelSheet[sheetIndex]->setSpan(mergeRowStart, mergeColumnIndex, mergeRowEnd, 1);
            }
        }
    }
}

QMap<int, QSet<QPair<int, int>>> GuiExcel::isSheetMergeInfo(const int& sheetIndex, const bool& refreshInfo) {
    if (chcekExcelSheet(sheetIndex)) {
        return QMap<int, QSet<QPair<int, int>>>();
    }
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();

    QMap<int, QSet<QPair<int, int>>> mergeInfo;
    QMap<int, QPair<int, int>> foundRowInfo;
    int rowIndex = 0;
    QVariantList sheetData = isHandler()->getProperty(sheetIndex).toList();

    // Construct MergeInfo : Property Data
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
    qDebug() << "isSheetMergeInfo :" << sheetIndex << mergeInfo.size();
    // for (const auto& rowData : sheetData.toList()) {
    //     qDebug() << "\t RowData :" << rowData.toStringList();
    // }
    // qDebug() << "-----------------------------------------------------------------------------------------";
    for (const auto& key : mergeInfo.keys()) {
        qDebug() << "\t MergeInfo[" << key << "]  :" << mergeInfo[key];
    }
    qDebug() << "-----------------------------------------------------------------------------------------";
#endif

    if (refreshInfo) {    // Update MergeInfo : Current Display Gui Sheet Merge Info
        const int rowMax = sheetData.toList().size();
        const int columnMax = (rowMax > 0) ? (sheetData.toList().at(0).toStringList().size()) : (0);

        mergeInfo.clear();

        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QSet<QPair<int, int>> updateMergeInfo;
            for (int rowIndex = 0; rowIndex < rowMax; ) {
                int value = mExcelSheet[sheetIndex]->rowSpan(rowIndex, columnIndex);
                int end = (rowIndex + value - 1);
                if (value > 1) {
                    updateMergeInfo.insert(qMakePair(rowIndex, end));
                    // qDebug() << "\t\t ReadInfo[" << columnIndex << "] :" << rowIndex << "~" << end;
                }
                rowIndex = (rowIndex + value);
            }

            if (updateMergeInfo.size() > 0) {
                mergeInfo[columnIndex] = updateMergeInfo;
            }
        }

#if 0
        qDebug() << "-----------------------------------------------------------------------------------------";
        for (const auto& key : mergeInfo.keys()) {
            qDebug() << "\t UpdateInfo[" << key << "] :" << mergeInfo[key];
        }
        qDebug() << "=========================================================================================\n\n";
#endif
    }

    return mergeInfo;
}

int GuiExcel::updateDisplayInsertDelete(const int& sheetIndex, const bool& insert) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }
    const QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    if (modelIndexs.isEmpty()) {
        qDebug() << "Selected Cell is empty";
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(modelIndexs);
    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();

    const int columnStart = cellSelectedInfo.getColumnStart();

    qDebug() << "updateDisplayInsertDelete :" << sheetIndex << insert << rowStart << rowEnd;

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

int GuiExcel::updateDisplayMergeSplit(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }
    const QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    if (modelIndexs.isEmpty()) {
        qDebug() << "Selected Cell is empty";
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(modelIndexs);
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

    qDebug() << "updateDisplayMergeSplit :" << sheetIndex << columnStart << columnCount << rowStart << rowCount;
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

int GuiExcel::updateDisplayCopy(const int& sheetIndex, const bool& cutState) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }

    const QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    if (modelIndexs.isEmpty()) {
        qDebug() << "Selected Cell is empty";
        return (-1);
    }

    const CellSelectedInfo cellSelectedInfo(modelIndexs);
    const int rowStart = cellSelectedInfo.getRowStart();
    const int rowEnd = cellSelectedInfo.getRowEnd();
    const int rowCount = cellSelectedInfo.getRowCount();
    const int columnStart = cellSelectedInfo.getColumnStart();
    const int columnEnd = cellSelectedInfo.getColumnEnd();
    const int columnCount = cellSelectedInfo.getColumnCount();

    qDebug() << "updateDisplayCopy :" << sheetIndex << cutState << cellSelectedInfo.getModelIndexs().size();
    qDebug() << "\t Row            :" << rowStart << rowEnd << rowCount;
    qDebug() << "\t Column         :" << columnStart << columnEnd << columnCount;

    if (cellSelectedInfo.getNotSupport()) {
        createSignal(ivis::common::EventTypeEnum::EventTypeWarningCopyCut);
        return (-1);
    }

    // int rowStart = modelIndexs.first().row();
    // int rowEnd = rowStart;
    // int columnStart = modelIndexs.first().column();
    // int columnEnd = columnStart;

    // QMap<int, QPair<int, int>> rowRangeList;
    // for (const auto& currCell : modelIndexs) {
    //     int row = currCell.row();
    //     int column = currCell.column();

    //     rowStart = qMin(rowStart, row);
    //     rowEnd = qMax(rowEnd, row);
    //     columnStart = qMin(columnStart, column);
    //     columnEnd = qMax(columnEnd, column);

    //     auto& range = rowRangeList[column];
    //     if (!rowRangeList.contains(column)) {
    //         range = qMakePair(row, row);
    //     } else {
    //         range.first = qMin(range.first, row);
    //         range.second = qMax(range.second, row);
    //     }
    // }

    // bool notSupport = false;
    // auto& rowRangeInfo = rowRangeList[columnStart];
    // for (const auto& key : rowRangeList.keys()) {
    //     if (rowRangeInfo != rowRangeList[key]) {
    //         notSupport = true;
    //         break;
    //     }
    //     // qDebug() << "\t RowInfo[" << key << "] :" << rowRangeList[key] << rowRangeInfo;
    //     rowRangeInfo = rowRangeList[key];
    // }
    // if (notSupport) {
    //     createSignal(ivis::common::EventTypeEnum::EventTypeWarningCopyCut);
    //     return;
    // }

    // int rowCount = (rowEnd - rowStart + 1);
    // int columnCount = (columnEnd - columnStart + 1);

    const auto& sheetData = readSheetDisplay(sheetIndex);
    QList<QStringList> copyDataList(rowCount, QStringList(columnCount, "[Empty]"));

    for (const auto& currCell : cellSelectedInfo.getModelIndexs()) {
        int cellRowIndex = currCell.row();
        int cellColumnIndex = currCell.column();
        int rowIndex = cellRowIndex - rowStart;
        int columnInex = cellColumnIndex - columnStart;

        QVariantList rowDataList = sheetData.toList();
        if (cellRowIndex >= rowDataList.size()) {
            continue;
        }
        QStringList columnDataList = rowDataList.at(cellRowIndex).toStringList();
        if (cellColumnIndex >= columnDataList.size()) {
            continue;
        }
        // QString readText = currCell.data().toString();    // 선택 셀에서 텍스트 추출시 사용
        QString readText = columnDataList.at(cellColumnIndex);
        copyDataList[rowIndex][columnInex] = readText;
        // qDebug() << "\t Read[" << rowIndex << columnInex << "] :" << readText;
    }

    QString clipboardData;
    for (const auto& rowData : copyDataList) {
        clipboardData.append(QString("%1\n").arg(rowData.join("\t")));
        // qDebug() << "\t CopyData :" << rowData.join(", ");
    }

    qDebug() << "\t ClipboardData :" << clipboardData.size() << clipboardData;
    QGuiApplication::clipboard()->setText(clipboardData);

    return columnStart;
}

int GuiExcel::updateDisplayPaste(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return (-1);
    }

    if ((QApplication::clipboard()->mimeData() == nullptr) || (QApplication::clipboard()->mimeData()->hasText() == false)) {
        qDebug() << "Clipboard data invalid";
        return (-1);
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    int selectCellCount = modelIndexs.size();
    if ((selectCellCount == 0)) {
        qDebug() << "Fail to select cell - count :" << selectCellCount;
        return (-1);
    }


    setCellEditSkip(true);

    QStringList clipboardData = QApplication::clipboard()->mimeData()->text().split("\n");
    int rowStart = (selectCellCount == 1) ? (modelIndexs.at(0).row()) : (0);
    int columnStart = (selectCellCount == 1) ? (modelIndexs.at(0).column()) : (0);
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    int rowCount = clipboardData.size() + rowStart;  // Row : 0(Header), 1 ~ Count(Item)

    if (rowCount >= rowMax) {
        mExcelSheet[sheetIndex]->setRowCount(rowCount);
    }


    qDebug() << "\n\n";
    qDebug() << "=====================================================================================================";
    qDebug() << "clipboardData :" << selectCellCount << clipboardData.size() << clipboardData;


    for (int rowIndex = 0; rowIndex < clipboardData.size(); rowIndex++) {
        QStringList rowDataList = clipboardData[rowIndex].split("\t");
        for (int columnIndex = 0; columnIndex < rowDataList.size(); columnIndex++) {
            int itemRowIndex = (rowStart + rowIndex);
            int itemColumnIndex = (columnStart + columnIndex);
            if (itemColumnIndex >= columnMax) {
                continue;
            }
            QString setText = rowDataList[columnIndex];
            if (setText.compare("[Empty]") == false) {
                setText.clear();
            }
            mExcelSheet[sheetIndex]->setItem(itemRowIndex, itemColumnIndex, new QTableWidgetItem(setText));
        }
    }
    // for (const auto& updateIndex : updateSheetList) {
    //     syncSheetData(updateIndex.toInt(), true);
    //     createSignal(ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData, QVariantList({sheetIndex, updateIndex}));
    // }

    setCellEditSkip(false);

    return columnStart;
}

void GuiExcel::updateDisplaySheetHeaderAdjust(const int& sheetIndex, const bool& resizeColumn) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if (resizeColumn) {
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
    }
    mExcelSheet[sheetIndex]->resizeRowsToContents();
}

void GuiExcel::updateDisplaySheetText(const int& sheetIndex) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    QMap<int, QVariantList> newSheetData = QMap<int, QVariantList>();
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    QVariantList sheetData = isHandler()->getProperty(sheetIndex).toList();

    // qDebug() << "====================================================================================================";
    // qDebug() << "updateDisplaySheetText :" << sheetIndex;
    // qDebug() << sheetIndex << ". SheetData :" << sheetData.size() <<sheetData;

    for (int rowIndex = 0; rowIndex < sheetData.size(); rowIndex++) {
        QStringList rowDataList = sheetData[rowIndex].toStringList();
        for (int columnIndex = 0; columnIndex < rowDataList.size(); columnIndex++) {
            QString readText = rowDataList[columnIndex];
            QStringList startText = readText.split(mergeStart);
            QStringList endText = readText.split(mergeEnd);
            QStringList mergeText = readText.split(merge);
            QString setText = QString();

            newSheetData[columnIndex].append(readText);
            if (startText.size() == 2) {
                setText = startText.at(1);
            } else if (endText.size() == 2) {
                setText = endText.at(1);
            } else {
                if (mergeText.size() == 2) {
                    setText = mergeText.at(1);
                } else {
                    setText = readText;
                }
            }
            // qDebug() << "Text[" << rowIndex << columnIndex <<"] :" << readText << "->" << setText;

            if (isDrawCheckBox(sheetIndex, columnIndex)) {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                item->setCheckState((setText.size() > 0) ? (Qt::Checked) : (Qt::Unchecked));
                mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, item);
            } else {
                mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(setText));
            }
        }
    }
    constructMergeInfo(sheetIndex, newSheetData);
    updateDisplayMergeCell(sheetIndex);  // Draw - Merge Cell
}

void GuiExcel::updateDisplaySheetNew(const int& sheetIndex, const int& rowMax, const int& columnMax) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    // qDebug() << "updateDisplaySheetNew :" << sheetIndex << rowMax << columnMax;
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

void GuiExcel::updateCellInfoContent(const int& sheetIndex, const int& row, const int& column) {
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

void GuiExcel::updateDefaultSheetFocus(const int& sheetIndex, const int& row, const int& column) {
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

void GuiExcel::updateInitialExcelSheet() {
    setCurrSheetIndex(ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    mMergeInfo.clear();
    mModelIndex.clear();
    mCopyMergeInfo.clear();
    mClearMergeInfo.clear();
    mClearCellInfo.clear();

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

void GuiExcel::updateDisplayKey(const int& keyValue) {
    int sheetIndex = getCurrSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if (keyValue == Qt::Key_Escape) {
        clearClipboardInfo(true);
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
    updateCellInfoContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
#else
    mGui->CellInfoContent->clear();
#endif
}

void GuiExcel::updateDisplayExcelSheet() {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    bool excelOpen = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
    qDebug() << "Excel Update :" << ((excelOpen) ? ("Open") : ("New")) << ", Size :" << mExcelSheet.size();

    // Clear - Table, TableWidget, CellInfo, Menu, Action
    updateInitialExcelSheet();
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
            updateDisplaySheetText(sheetIndex);
        } else {
            updateDisplaySheetNew(sheetIndex, rowMax, columnMax);
        }
        updateDefaultSheetFocus(sheetIndex, 0, 0);
        updateCellInfoContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
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
            updateCellInfoContent(sheetIndex, row, column);
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
        updateDisplaySheetHeaderAdjust(sheetIndex, true);
        sheetIndex++;
    }

    // qDebug() << "\t Excel : complete";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

void GuiExcel::updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if (isSheetChanged(sheetIndex) == false) {
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

    if (columnIndex > (-1)) {
        QString signal = (mExcelSheet[sheetIndex]->item(row, columnIndex) == nullptr)
                             ? (QString())
                             : (mExcelSheet[sheetIndex]->item(row, columnIndex)->text());
        QVariantList cellDataInfo = QVariantList({signal, text, sheetIndex, row, column});
        createSignal(ivis::common::EventTypeEnum::EventTypeUpdateCellDataInfo, cellDataInfo);
    }

    updateDisplaySheetHeaderAdjust(sheetIndex, false);  // 편집시 화면 변경 되는 이슈
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
        updateDisplaySelectGenType();
    } else if (supportVehicleType) {
        updateDisplayAutoCompleteVehicle();
    } else if (supportConfigName) {
        updateDisplayAutoCompleteConfigName();
    } else {
        bool description = (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
        updateDisplayAutoCompleteSignal(description, column);
    }
    mExcelSheet[sheetIndex]->clearFocus();
}

void GuiExcel::updateDisplayAutoCompleteSignal(const bool& description, const int& columnIndex) {
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

    // qDebug() << "updateDisplayAutoCompleteSignal :" << description << columnIndex;
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

void GuiExcel::updateDisplayAutoCompleteVehicle() {
    QVariantList info = QVariantList({
        QString("Select Vehicle"),
        QString(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeSelectVehicleType, info);
}

void GuiExcel::updateDisplayAutoCompleteConfigName() {
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

void GuiExcel::updateDisplayValueEnum(const QVariantList& data) {
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

void GuiExcel::updateDisplayTCNameResult(const QStringList& data) {
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

void GuiExcel::updateDisplaySelectGenType() {
    QVariantList info = QVariantList({
        QString("Select Type"),
        QString(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeGenTypeList).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeGenType, info);
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
        // updateDisplaySheetHeaderAdjust(sheetIndex, true);  // 편집시 화면 변경 되는 이슈
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

void GuiExcel::printMergeInfo(const QString& title, const bool& mergeSplit) {
#if 0
    qDebug() << "\t ****************************************************************************";
    qDebug() << "\t [" << title << "]";
    qDebug() << "\t >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    QMap<int, ExcelSheet> mergeInfo = mMergeInfo;
    QString type = QString("All");
    if (mergeSplit) {
        mergeInfo.clear();
        mergeInfo[mergeInfo.size()] = mClearMergeInfo;
        mergeInfo[mergeInfo.size()] = mCopyMergeInfo;
        type = QString("Copy/Clear");
    }
    for (ExcelSheet merge : mergeInfo) {
        QMapIterator<int, QList<QPair<int, int>>> iter(merge.isMergeInfo());
        while (iter.hasNext()) {
            iter.next();
            for (const auto& info : iter.value()) {
                qDebug() << "\t\t " << type << ":" << iter.key() << info.first << info.second;
            }
        }
        qDebug() << "\t\t -------------------------------------------------";
    }
    qDebug() << "\t <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
#endif
}

void GuiExcel::copyClipboardInfo(const bool& cutState) {
    int sheetIndex = getCurrSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    if (modelIndexs.size() == 0) {
        qDebug() << "Select cell count : 0";
        return;
    }

    int rowStart = modelIndexs.at(0).row();
    int rowEnd = modelIndexs.last().row() - rowStart + 1;
    int rowMax = (rowStart + rowEnd);
    int columnStart = modelIndexs.at(0).column();
    int columnEnd = modelIndexs.last().column() - columnStart + 1;
    int columnMax = (columnStart + columnEnd);

    if (rowEnd == 1) {  // 병합된 셀의 경우 다시 선택시 셀 선택한 카운트(rowEnd) 값이 1로 나오는 경우 발생함
        int mergeCount = isMergeCell(sheetIndex, columnStart, rowStart);
        if (mergeCount > 0) {
            rowEnd = mergeCount;
            rowMax = (rowStart + rowEnd);
            // qDebug() << "isMergeCell :" << mergeCount << rowEnd << rowMax;
        }
    }

    // qDebug() << "\t Copy/Cut Sheet[" << sheetIndex << "] - Index :" << rowStart << columnStart << rowEnd << columnEnd << ","
    //          << rowMax << columnMax;

    QMap<int, QVariantList> newSheetData = QMap<int, QVariantList>();
    QString clipboardData = QString();
    QVariantList readIndexInfo = QVariantList({rowStart, columnStart, rowMax, columnMax});
    QVariantList sheetData = readSheetProperty(sheetIndex, readIndexInfo, clipboardData);

    clipboardData.resize(clipboardData.size() - 1);
    QGuiApplication::clipboard()->setText(clipboardData);

    for (const auto& rowListData : sheetData) {
        int columnIndex = 0;
        for (const auto& data : rowListData.toList()) {
            newSheetData[columnIndex++].append(data.toString());
        }
    }

    // Copy, Paste : Cell, Merge Info
    mCopyMergeInfo.clear();
    mClearMergeInfo.clear();
    mClearCellInfo.clear();

    constructMergeSplitInfo(newSheetData, rowStart, columnStart);

    if (cutState) {
        mClearCellInfo.append(sheetIndex);
        mClearCellInfo.append(rowStart);
        mClearCellInfo.append(columnStart);
        mClearCellInfo.append(rowMax);
        mClearCellInfo.append(columnMax);

        for (int clearRowIndex = rowStart; clearRowIndex < rowMax; clearRowIndex++) {
            for (int clearColumnIndex = columnStart; clearColumnIndex < columnMax; clearColumnIndex++) {
                if (mExcelSheet[sheetIndex]->item(clearRowIndex, clearColumnIndex) == nullptr) {
                    mExcelSheet[sheetIndex]->setItem(clearRowIndex, clearColumnIndex, new QTableWidgetItem(""));
                }
                mExcelSheet[sheetIndex]->item(clearRowIndex, clearColumnIndex)->setBackground(QColor(200, 200, 200));
            }
        }
    }
}

int GuiExcel::clearClipboardInfo(const bool& escapeKeyClear) {
    int clearSheetIndex = ((mClearCellInfo.size() == 5) ? (mClearCellInfo.at(0)) : (-1));

    if (clearSheetIndex != (-1)) {
        int clearRowStart = mClearCellInfo.at(1);
        int clearColumnStart = mClearCellInfo.at(2);
        int clearRowEnd = mClearCellInfo.at(3);
        int clearColumnEnd = mClearCellInfo.at(4);

        for (int clearRowIndex = clearRowStart; clearRowIndex < clearRowEnd; clearRowIndex++) {
            for (int clearColumnIndex = clearColumnStart; clearColumnIndex < clearColumnEnd; clearColumnIndex++) {
                mExcelSheet[clearSheetIndex]->setItem(clearRowIndex, clearColumnIndex, new QTableWidgetItem(""));
                mExcelSheet[clearSheetIndex]->item(clearRowIndex, clearColumnIndex)->setBackground(QColor(255, 255, 255));
            }
        }
        mClearCellInfo.clear();
    }

    if (escapeKeyClear) {
        mClearMergeInfo.clear();
        mCopyMergeInfo.clear();
        QGuiApplication::clipboard()->clear();
    }

    return clearSheetIndex;
}

void GuiExcel::pasteClipboardInfo() {
    int sheetIndex = getCurrSheetIndex();
    if (chcekExcelSheet(sheetIndex)) {
        return;
    }

    if ((QApplication::clipboard()->mimeData() == nullptr) || (QApplication::clipboard()->mimeData()->hasText() == false)) {
        qDebug() << "Clipboard data invalid";
        return;
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    int selectCellCount = modelIndexs.size();
    if ((selectCellCount != 1)) {
        qDebug() << "Fail to select cell - count :" << selectCellCount;
        return;
    }

    setCellEditSkip(true);

    QStringList clipboardData = QApplication::clipboard()->mimeData()->text().split("\n");
    int rowStart = (selectCellCount == 1) ? (modelIndexs.at(0).row()) : (0);
    int columnStart = (selectCellCount == 1) ? (modelIndexs.at(0).column()) : (0);
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    int rowCount = clipboardData.size() + rowStart;  // Row : 0(Header), 1 ~ Count(Item)

    if (rowCount >= rowMax) {
        mExcelSheet[sheetIndex]->setRowCount(rowCount);
    }

    for (int rowIndex = 0; rowIndex < clipboardData.size(); rowIndex++) {
        QStringList rowDataList = clipboardData[rowIndex].split("\t");
        for (int columnIndex = 0; columnIndex < rowDataList.size(); columnIndex++) {
            int itemRowIndex = (rowStart + rowIndex);
            int itemColumnIndex = (columnStart + columnIndex);
            if (itemColumnIndex >= columnMax) {
                continue;
            }
            QString setText = rowDataList[columnIndex];
            if (setText.compare("[Empty]") == false) {
                setText.clear();
            }
            mExcelSheet[sheetIndex]->setItem(itemRowIndex, itemColumnIndex, new QTableWidgetItem(setText));
        }
    }

    QVariantList updateSheetList = QVariantList({sheetIndex});
    int clearSheetIndex = clearClipboardInfo(false);
    if (clearSheetIndex >= 0) {
        QMapIterator<int, QList<QPair<int, int>>> iterClear(mClearMergeInfo.isMergeInfo());
        while (iterClear.hasNext()) {
            iterClear.next();
            for (const auto& info : iterClear.value()) {
                updateMergeInfo(true, clearSheetIndex, iterClear.key(), info.first, info.second);
            }
        }
        mClearMergeInfo.clear();

        if (sheetIndex != clearSheetIndex) {
            updateSheetList.append(clearSheetIndex);
            updateDisplaySplitCell(clearSheetIndex);
            updateDisplayMergeCell(clearSheetIndex);
        }
    }

    QMapIterator<int, QList<QPair<int, int>>> iterCopy(mCopyMergeInfo.isMergeInfo());
    while (iterCopy.hasNext()) {
        iterCopy.next();
        int mergeColumnIndex = columnStart + iterCopy.key();
        for (const auto& info : iterCopy.value()) {
            int mergeRowStart = rowStart + info.first;
            int mergeRowEnd = info.second;
            updateMergeInfo(false, sheetIndex, mergeColumnIndex, mergeRowStart, mergeRowEnd);
        }
    }
    mCopyMergeInfo.clear();
    QGuiApplication::clipboard()->clear();

    updateDisplaySplitCell(sheetIndex);
    updateDisplayMergeCell(sheetIndex);

    updateDisplaySheetHeaderAdjust(sheetIndex, false);  // 편집시 화면 변경 되는 이슈
    // chaged data sync : gui -> control
    for (const auto& updateIndex : updateSheetList) {
        syncSheetData(updateIndex.toInt(), true);
        createSignal(ivis::common::EventTypeEnum::EventTypeUpdateAutoCompleteData, QVariantList({sheetIndex, updateIndex}));
    }

    setCellEditSkip(false);
}

QList<QStringList> GuiExcel::isSheetData(const int& sheetIndex, const bool& removeMerge, const bool& readProperty) {
    if (chcekExcelSheet(sheetIndex)) {
        return QList<QStringList>();
    }
    const QString mergeStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeStart).toString();
    const QString mergeEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeEnd).toString();
    const QString merge = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});
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
            for (auto& readColumnText : rowData) {
                if (ivis::common::isContainsString(readColumnText, mergeStart)) {
                    ivis::common::getRemoved(readColumnText, mergeInfos);
                } else if (ivis::common::isContainsString(readColumnText, merge)) {
                    readColumnText.clear();
                } else if (ivis::common::isContainsString(readColumnText, mergeEnd)) {
                    readColumnText.clear();
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
    return mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->text();
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
            columIndex = updateDisplayInsertDelete(sheetIndex, true);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeDelete: {
            columIndex = updateDisplayInsertDelete(sheetIndex, false);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit: {
            columIndex = updateDisplayMergeSplit(sheetIndex);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeCopy: {
            columIndex = updateDisplayCopy(sheetIndex, false);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypeCut: {
            columIndex = updateDisplayCopy(sheetIndex, true);
            break;
        }
        case ivis::common::ShortcutTypeEnum::ShortcutTypePaste: {
            columIndex = updateDisplayPaste(sheetIndex);
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
        updateDisplaySheetHeaderAdjust(sheetIndex, false);    // Resize Header Size : Row(O), Column(X)
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
    QVariantList autoInputInfo = QVariantList({sheetIndex, row, moduleName});
    createSignal(ivis::common::EventTypeEnum::EventTypeAutoInputDescriptionInfo, autoInputInfo);
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
            updateDisplayValueEnum(value.toList());
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTCNameResult: {
            updateDisplayTCNameResult(value.toStringList());
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
