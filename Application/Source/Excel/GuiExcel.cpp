#include "GuiExcel.h"
#include "AbstractHandler.h"

#include "CommonResource.h"

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

GuiExcel::GuiExcel(AbstractHandler* handler) : AbstractGui(handler) {
    mMainView = new QTabWidget(isHandler()->getScreen());
    mMenuRight = new QMenu(isHandler()->getScreen());
    updateDisplaySize();
}

void GuiExcel::drawDisplayDepth0() {
    updateDisplayVisible();
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
}

void GuiExcel::updateDisplayVisible() {
    if (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        mMainView->show();
    } else {
        mMainView->hide();
    }
}

bool GuiExcel::chcekExcleSheet(const int& sheetIndex) {
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

QVariantList GuiExcel::readExcelSheet(const int& sheetIndex, const QVariantList& readIndexInfo, QString& allString) {
    if (chcekExcleSheet(sheetIndex)) {
        qDebug() << "Fail to excel sheet nullptr : " << sheetIndex << ", size :" << mExcelSheet.size();
        return QVariantList();
    }

    QVariantList sheetData = QVariantList();
    int rowStart = 0;
    int columnStart = 0;
    int rowMax = 0;
    int columnMax = 0;

    if (readIndexInfo.size() == 0) {
        rowMax = mExcelSheet[sheetIndex]->rowCount();
        columnMax = mExcelSheet[sheetIndex]->columnCount();
        // Read Title
        QStringList title = QStringList();
        for (int columnIndex = 0; columnIndex < columnMax; columnIndex++) {
            title.append(mExcelSheet[sheetIndex]->horizontalHeaderItem(columnIndex)->text());
        }
        sheetData.append(title);
    } else {
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
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);
    QMap<int, QList<QPair<int, int>>> mergeInfo = mMergeInfo[sheetIndex].isMergeInfo();
    for (int rowIndex = rowStart; rowIndex < rowMax; rowIndex++) {
        QStringList rowData = QStringList();
        for (int columnIndex = columnStart; columnIndex < columnMax; columnIndex++) {
            QString readText = ((mExcelSheet[sheetIndex]->item(rowIndex, columnIndex))
                                    ? (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->text())
                                    : (QString()));
            QString text = readText;

            if (rowData.size() > 0) {
                allString.append("\t");
            }
            allString.append(text);

            for (const auto& info : mergeInfo[columnIndex]) {
                int rowStart = info.first;
                int rowEnd = (info.first + info.second);
                if ((rowIndex >= rowStart) && (rowIndex < rowEnd)) {
                    if (rowIndex == rowStart) {
                        text = excelMergeTextStart.toString();
                    } else if (rowIndex == (rowEnd - 1)) {
                        text = excelMergeTextEnd.toString();
                    } else {
                        text = excelMergeText.toString();
                    }
                    text = text + readText;
                    // qDebug() << "MergeText[" << sheetIndex << "][" << rowIndex << "," << columnIndex << "] :" << text;
                }
            }
            rowData.append(text);

            // if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
            //     qDebug() << "ReadText[" << sheetIndex << "][" << columnIndex << "," << rowIndex << "] :" << text;
            // }
        }
        sheetData.append(rowData);
        allString.append("\n");
    }

    // qDebug() << "readExcelSheet() ->"<< "Length :" << rowMax << columnMax << sheetData.size();
    // qDebug() << mExcelSheet[sheetIndex] << ":" << sheetData;
    // qDebug() << "==================================================================================================\n";

    return sheetData;
}

void GuiExcel::readAllExcelSheet() {
    if (chcekExcleSheet(0)) {
        return;
    }

    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    for (const auto& sheet : mExcelSheet) {
        QString allString = QString();
        QVariantList sheetData = readExcelSheet(sheetIndex, QVariantList(), allString);
        if (sheetData.size() > 0) {
            int eventType = (sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) +
                            ivis::common::EventTypeEnum::EventTypeListDescription;
            createSignal(eventType, sheetData);
            sheetIndex++;
        }
    }
    QVariant saveFielPath = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeReadExcelSheetBeforeSave);
    createSignal(ivis::common::EventTypeEnum::EventTypeSaveFromReadExcelSheet, saveFielPath);
}

bool GuiExcel::updateMergeInfo(const bool& erase, const int& sheetIndex, const int& columnIndex, const int& rowStart,
                               const int& rowEnd) {
    if (chcekExcleSheet(sheetIndex)) {
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
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    // QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);

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
            QStringList startText = readText.split(excelMergeTextStart.toString());
            QStringList endText = readText.split(excelMergeTextEnd.toString());
            // QStringList mergeText = readText.split(excelMergeText.toString());

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
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    mExcelSheet[sheetIndex]->clearSpans();
}

void GuiExcel::updateDisplayMergeCell(const int& sheetIndex) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

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

void GuiExcel::updateDisplaySheetHeaderAdjust(const int& sheetIndex) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    if (mExcelSheet[sheetIndex]) {
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
        mExcelSheet[sheetIndex]->resizeRowsToContents();
    }
}

void GuiExcel::updateDisplaySheetText(const int& sheetIndex) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QMap<int, QVariantList> newSheetData = QMap<int, QVariantList>();
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);
    QVariantList sheetData = isHandler()->getProperty(sheetIndex).toList();
    // qDebug() << "====================================================================================================";
    // qDebug() << "Length :" << sheetData.size() << rowMax << columnMax;
    // qDebug() << sheet << ":" << sheetData;

    for (int rowIndex = 0; rowIndex < sheetData.size(); rowIndex++) {
        QStringList rowDataList = sheetData[rowIndex].toStringList();
        for (int columnIndex = 0; columnIndex < rowDataList.size(); columnIndex++) {
            QString readText = rowDataList[columnIndex];
            QStringList startText = readText.split(excelMergeTextStart.toString());
            QStringList endText = readText.split(excelMergeTextEnd.toString());
            QStringList mergeText = readText.split(excelMergeText.toString());
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
            mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(setText));
        }
    }
    constructMergeInfo(sheetIndex, newSheetData);
    updateDisplayMergeCell(sheetIndex);  // Draw - Merge Cell
}

void GuiExcel::updateInitialExcelSheet() {
    mCurrentSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;

    if (chcekExcleSheet(0)) {
        return;
    }

    for (const auto& sheet : mExcelSheet) {
        sheet->clear();
        disconnect(sheet);
        delete sheet;
    }
    mExcelSheet.clear();
    mMainView->clear();

    for (const auto& action : mMenuActionItem) {
        delete action;
    }
    mMenuActionItem.clear();
    mMenuRight->clear();

    for (ExcelSheet cellInfo : mMergeInfo) {
        cellInfo.clear();
    }
    mMergeInfo.clear();

    mCopyMergeInfo.clear();
    mClearMergeInfo.clear();
    mClearCellInfo.clear();

    // qDebug() << "\t ExcelSheet : Clear";
}

void GuiExcel::updateDefaultSheetFocus(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcleSheet(sheetIndex)) {
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

void GuiExcel::updateDisplayKey(const int& keyValue) {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    int row = (mExcelSheet[sheetIndex]->currentRow() < 0) ? (0) : (mExcelSheet[sheetIndex]->currentRow());
    int column = (mExcelSheet[sheetIndex]->currentColumn() < 0) ? (0) : (mExcelSheet[sheetIndex]->currentColumn());

#if 0
    qDebug() << "Sheet :" << sheetIndex << row << column << ", KeyValue :" << keyValue;
    if (keyValue == ivis::common::KeyTypeEnum::KeyInputValueOK) {
        if (mExcelSheet[sheetIndex]->isPersistentEditorOpen(mExcelSheet[sheetIndex]->item(row, column)) == false) {
            int rowMax = mExcelSheet[sheetIndex]->rowCount();
            ivis::common::LIMIT_P(row, 1, 0, rowMax);
            qDebug() << "Move Cell :" << row << column;
            mExcelSheet[sheetIndex]->setCurrentCell(row, column);
            // return;
        }
    }
#endif

    if (mExcelSheet[sheetIndex]->item(row, column)) {
        if (mExcelSheet[sheetIndex]->isPersistentEditorOpen(mExcelSheet[sheetIndex]->item(row, column)) == false) {
            mExcelSheet[sheetIndex]->openPersistentEditor(mExcelSheet[sheetIndex]->item(row, column));
            // mExcelSheet[sheetIndex]->editItem(mExcelSheet[sheetIndex]->item(row, column));
        }
    }
}

void GuiExcel::updateDisplayExcelSheet() {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    bool excelOpen = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
    qDebug() << "Excel Update :" << ((excelOpen) ? ("Open") : ("New")) << ", Size :" << mExcelSheet.size();

    // Clear - Table, TableWidget, CellInfo, Menu, Action
    updateInitialExcelSheet();

    // Create : Action Item
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert] = mMenuRight->addAction("Insert");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeDelete] = mMenuRight->addAction("Delete");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit] = mMenuRight->addAction("Merge/Split");

    // Draw - Sheet
    QStringList sheetName = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName).toStringList();
    QStringList descTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle).toStringList();
    QStringList otherTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle).toStringList();
    QVariantList listRowCount = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount).toList();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    for (const auto& sheet : sheetName) {
        QStringList contentTitle =
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) ? (descTitle) : (otherTitle);
        int rowMax = listRowCount.at(sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription).toInt();
        int columnMax = contentTitle.size();

        // Draw Sheet
        mExcelSheet[sheetIndex] = new QTableWidget(rowMax, columnMax);
        mExcelSheet[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        // mExcelSheet[sheetIndex]->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
        mExcelSheet[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mMainView->addTab(mExcelSheet[sheetIndex], sheet);

        // Draw Sheet Data
        if (excelOpen) {
            updateDisplaySheetText(sheetIndex);
        }
        updateDefaultSheetFocus(sheetIndex, 0, 0);

        connect(mExcelSheet[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            // QString text = mExcelSheet[sheetIndex]->item(row, column)->text();
            // qDebug() << sheetIndex << ". cellChanged :" << column << "," << row << ", Text" << text;
            QVariantList editSheetInfo = QVariantList();
            if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoPrivates) ||
                (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoInters)) {
                QString allString = QString();
                QVariantList nodeAddress = QVariantList();
                for (const auto& data : readExcelSheet(sheetIndex, QVariantList(), allString)) {
                    QVariantList rowLineData = data.toList();
                    if (rowLineData.size() > 0) {
                        nodeAddress.append(rowLineData.at(0));
                    }
                }
                if (nodeAddress.size() > 0) {
                    nodeAddress.removeAt(0);  // nodeAddress[0] = "TCName" -> 필요없음
                }
                editSheetInfo = QVariantList({sheetIndex, QVariant(nodeAddress)});
            }
            createSignal(ivis::common::EventTypeEnum::EventTypeEditExcelSheet, QVariant(editSheetInfo));
            updateDisplaySheetHeaderAdjust(sheetIndex);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint& pos) {
            QAction* selectAction = mMenuRight->exec(mExcelSheet[sheetIndex]->mapToGlobal(QPoint((pos.x() + 20), (pos.y() + 5))));
            int shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInvalid;

            if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeInsert;
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeDelete;
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit]) {
                shortcutType = ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit;
            } else {
                qDebug() << "Fail to menu right selection action item";
                return;
            }
            updateDisplayCellEdit(shortcutType);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellDoubleClicked, [=](int row, int column) {
            if (((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) && (column != 3)) ||
                ((sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) &&
                 ((column != 4) && (column != 6) && (column != 9)))) {
                qDebug() << "Fail to not support auto complete :" << sheetIndex << column;
                return;
            }

            mSelectItem = mExcelSheet[sheetIndex]->item(row, column);
            if (mSelectItem == nullptr) {
                mExcelSheet[sheetIndex]->setItem(row, column, new QTableWidgetItem(""));
                mSelectItem = mExcelSheet[sheetIndex]->item(row, column);
            }
            updateDisplayAutoComplete(true, sheetIndex, row, column);
            mExcelSheet[sheetIndex]->clearFocus();
        });
#if 0
        connect(mExcelSheet[sheetIndex]->horizontalHeader(), &QHeaderView::sectionResized,
                                                                [=](int logicalIndex, int oldSize, int newSize) {
            qDebug() << sheetIndex << ". H_sectionResized :" << logicalIndex << oldSize << newSize;
        });
        connect(mExcelSheet[sheetIndex]->verticalHeader(), &QHeaderView::sectionResized,
                                                                [=](int logicalIndex, int oldSize, int newSize) {
            qDebug() << sheetIndex << ". V_sectionResized :" << logicalIndex << oldSize << newSize;
        });
#endif

        // Resize - Cell Width/Height
        updateDisplaySheetHeaderAdjust(sheetIndex);
        sheetIndex++;
    }

    connect(mMainView, &QTabWidget::currentChanged,
            [=](int index) { mCurrentSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription + index; });
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

void GuiExcel::updateDisplayAutoComplete(const bool& show, const int& sheetIndex, const int& rowIndex, const int& columnIndex) {
    qDebug() << "updateDisplayAutoComplete :" << show << sheetIndex << rowIndex << columnIndex;

    QStringList list = QStringList();
#if 0  // 자동완성 동작 조건 처리
    if (columnIndex == 5) {
        list = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC).toStringList();
    } else if (columnIndex == 9) {
        list = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM).toStringList();
    } else {
        list = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll).toStringList();
    }
#else
    // list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressSFC).toStringList());
    // list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressVSM).toStringList());
    list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll).toStringList());
    list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressPrivate).toStringList());
    list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressInter).toStringList());
#endif

    if (mAutoComplete == nullptr) {
        mAutoComplete = new AutoCompleteDialog(isHandler()->getScreen(), QString("AutoComplete"), list);
        connect(mAutoComplete, &AutoCompleteDialog::signalAutoCompleteSelectedText, [=](const QString& text) {
            if (mSelectItem) {
                mSelectItem->setText(text);
            }
            mAutoComplete->hide();
            // mAutoComplete->finished(true);
        });
        connect(mAutoComplete, &QDialog::finished, [=]() {
            disconnect(mAutoComplete);
            delete mAutoComplete;
            mAutoComplete = nullptr;
        });
    }

    if (show) {
        mAutoComplete->setAutoCompleteStringList(list);
        mAutoComplete->show();
        if (mSelectItem) {
            mAutoComplete->setInputText(mSelectItem->text());
        }
    } else {
        mAutoComplete->hide();
    }
}

void GuiExcel::copyClipboardInfo(const bool& cutState) {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    if (modelIndexs.size() == 0) {
        qDebug() << "Select cell count : 0";
        return;
    }

    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);

    int rowStart = modelIndexs.at(0).row();
    int rowEnd = modelIndexs.last().row() - rowStart + 1;
    int rowMax = (rowStart + rowEnd);
    int columnStart = modelIndexs.at(0).column();
    int columnEnd = modelIndexs.last().column() - columnStart + 1;
    int columnMax = (columnStart + columnEnd);

    // qDebug() << "\t Copy/Cut Index :" << sheetIndex << ", Info :" << rowStart << columnStart << rowEnd << columnEnd << ","
    //          << rowMax << columnMax;

    QMap<int, QVariantList> newSheetData = QMap<int, QVariantList>();
    QString clipboardData = QString();
    QVariantList readIndexInfo = QVariantList({rowStart, columnStart, rowMax, columnMax});
    QVariantList sheetData = readExcelSheet(sheetIndex, readIndexInfo, clipboardData);

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
                    continue;
                }
                mExcelSheet[sheetIndex]->item(clearRowIndex, clearColumnIndex)->setBackground(QColor(200, 200, 200));
            }
        }
    }
}

void GuiExcel::printMergeInfo(const QString& title, const bool& mergeSplit) {
#if 1
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

void GuiExcel::pasteClipboardInfo() {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
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

    QStringList clipboardData = QApplication::clipboard()->mimeData()->text().split("\n");
    int rowStart = (selectCellCount == 1) ? (modelIndexs.at(0).row()) : (0);
    int columnStart = (selectCellCount == 1) ? (modelIndexs.at(0).column()) : (0);
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    int rowCount = clipboardData.size() + rowStart;  // Row : 0(Header), 1 ~ Count(Item)

    if (rowCount >= rowMax) {
        // qDebug() << "\t Incress Row :" << rowStart << rowCount << rowMax;
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
            mExcelSheet[sheetIndex]->setItem(itemRowIndex, itemColumnIndex, new QTableWidgetItem(setText));
        }
    }

    // Cut Cell Clear
    if (mClearCellInfo.size() == 5) {
        // QGuiApplication::clipboard()->clear();
        int clearSheetIndex = mClearCellInfo.at(0);
        int clearRowStart = mClearCellInfo.at(1);
        int clearColumnStart = mClearCellInfo.at(2);
        int clearRowEnd = mClearCellInfo.at(3);
        int clearColumnEnd = mClearCellInfo.at(4);
        for (int clearRowIndex = clearRowStart; clearRowIndex < clearRowEnd; clearRowIndex++) {
            for (int clearColumnIndex = clearColumnStart; clearColumnIndex < clearColumnEnd; clearColumnIndex++) {
                if (mExcelSheet[clearSheetIndex]->item(clearRowIndex, clearColumnIndex) == nullptr) {
                    continue;
                }
                mExcelSheet[clearSheetIndex]->setItem(clearRowIndex, clearColumnIndex, new QTableWidgetItem(""));
                mExcelSheet[clearSheetIndex]->item(clearRowIndex, clearColumnIndex)->setBackground(QColor(255, 255, 255));
            }
        }
        mClearCellInfo.clear();

        QMapIterator<int, QList<QPair<int, int>>> iterClear(mClearMergeInfo.isMergeInfo());
        while (iterClear.hasNext()) {
            iterClear.next();
            for (const auto& info : iterClear.value()) {
                updateMergeInfo(true, clearSheetIndex, iterClear.key(), info.first, info.second);
            }
        }
        mClearMergeInfo.clear();

        if (sheetIndex != clearSheetIndex) {
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
}

void GuiExcel::updateDisplayClipboardInfo(const int& clipboardType) {
    if ((clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeCopy) ||
        (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeCut)) {
        copyClipboardInfo(clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeCut);
    } else if (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypePaste) {
        pasteClipboardInfo();
    } else if (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeUndo) {
        qDebug() << "\t\t ShortcutTypeUndo";
    } else if (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeRedo) {
        qDebug() << "\t\t ShortcutTypeRedo";
    } else {
    }
}

void GuiExcel::updateDisplayCellEdit(const int& editType) {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    bool selectInvalid = (modelIndexs.size() == 0);
#if 1
    int columnStart = (selectInvalid) ? (1) : (modelIndexs.at(0).column());
    int columnEnd = (selectInvalid) ? (1) : (modelIndexs.last().column() - columnStart + 1);
    int rowStart = (selectInvalid) ? (0) : (modelIndexs.at(0).row());
    int rowEnd = (selectInvalid) ? (1) : (modelIndexs.last().row() - rowStart + 1);
    int rowCount = mExcelSheet[sheetIndex]->rowCount();
#else
    int columnStart = (selectInvalid) ? (mExcelSheet[sheetIndex]->currentColumn()) : (modelIndexs.at(0).column());
    int columnEnd = (selectInvalid) ? (1) : (modelIndexs.last().column() - columnStart + 1);
    int rowStart = (selectInvalid) ? (mExcelSheet[sheetIndex]->currentRow()) : (modelIndexs.at(0).row());
    int rowEnd = (selectInvalid) ? (1) : (modelIndexs.last().row() - rowStart + 1);
    int rowCount = mExcelSheet[sheetIndex]->rowCount();
#endif
    // qDebug() << "updateDisplayCellEdit : Index :" << sheetIndex << ", RowColumn :" << columnStart << rowStart << rowEnd
    //          << columnEnd << ", RowCount :" << rowCount;

    if ((editType == ivis::common::ShortcutTypeEnum::ShortcutTypeInsert) ||
        (editType == ivis::common::ShortcutTypeEnum::ShortcutTypeDelete)) {
        for (int index = 0; index < rowEnd; index++) {
            if (editType == ivis::common::ShortcutTypeEnum::ShortcutTypeInsert) {
                mExcelSheet[sheetIndex]->insertRow(rowStart);
            } else {
                mExcelSheet[sheetIndex]->removeRow(rowStart);
            }
        }
        updateDisplaySheetText(sheetIndex);
    } else if (editType == ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit) {
        bool columnSelectError = (columnEnd > 1);
        if (columnSelectError) {
            createSignal(ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning, QVariant());
        } else {
            if (updateMergeInfo(false, sheetIndex, columnStart, rowStart, rowEnd)) {
                updateDisplaySplitCell(sheetIndex);
            }
            updateDisplayMergeCell(sheetIndex);
        }
    } else {
        qDebug() << "Fail to menu right selection action item";
        return;
    }

    createSignal(ivis::common::EventTypeEnum::EventTypeEditExcelSheet, QVariant());
    updateDisplaySheetHeaderAdjust(sheetIndex);
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
        case ivis::common::PropertyTypeEnum::PropertyTypeReadExcelSheetBeforeSave: {
            readAllExcelSheet();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeClipboardType: {
            int clipboardType = value.toInt();
            updateDisplayClipboardInfo(clipboardType);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeShortcutType: {
            int shortcutType = value.toInt();
            updateDisplayCellEdit(shortcutType);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeKey: {
            int keyValue = value.toInt();
            updateDisplayKey(keyValue);
            break;
        }
        default: {
            break;
        }
    }
}
