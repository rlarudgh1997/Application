#include "GuiExcel.h"
#include "AbstractHandler.h"

#include "CommonResource.h"

#include <QMenu>
#include <QLabel>
#include <QTextEdit>
 #include <QHeaderView>
// #include <QDialog>



QSharedPointer<GuiExcel>& GuiExcel::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiExcel> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiExcel>(new GuiExcel(handler));
    }
    return gGui;
}

GuiExcel::GuiExcel(AbstractHandler* handler) : AbstractGui(handler) {
    mMainView = new QTabWidget(isHandler()->getScreen());
    // mMainView->setStyleSheet("background-color : blue");
    mMenuRight = new QMenu(isHandler()->getScreen());
    updateDisplaySize();

#if 0   // QSharedPointer<QMap<>> : 사용법
    enum ItemType {
        Widget = 0,
        MainWindow,
        TabeWidget,
    };
    QSharedPointer<QMap<ItemType, QWidget*>> mItem = QSharedPointer<QMap<ItemType, QWidget*>>();
    mItem = QSharedPointer<QMap<ItemType, QWidget*>>::create(QMap<ItemType, QWidget*>{
                {ItemType::Widget,     qobject_cast<QWidget*>(isHandler()->getScreen())},
                {ItemType::MainWindow, qobject_cast<QWidget*>(new QMainWindow())},
                {ItemType::TabeWidget, qobject_cast<QWidget*>(new QTabWidget())},
            });
#endif
}

void GuiExcel::controlConnect(const bool& state) {
//     if (state) {
//         connect(isHandler(),                       &HandlerExcel::signalHandlerEvent,
//                 this,                              &ControlExcel::slotHandlerEvent,
//                 Qt::UniqueConnection);
//         connect(ConfigSetting::instance().data(),  &ConfigSetting::signalConfigChanged,
//                 this,                              &ControlExcel::slotConfigChanged,
//                 Qt::UniqueConnection);
//         connect(ControlManager::instance().data(), &ControlManager::signalEventInfoChanged,
//                 this,                              &ControlExcel::slotEventInfoChanged,
//                 Qt::UniqueConnection);
// #if defined(USE_RESIZE_SIGNAL)
//         connect(ControlManager::instance().data(), &ControlManager::signalScreenSizeChanged, [=](const QSize& screenSize) {
//                 updateDataHandler(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize, screenSize);
//         });
// #endif
//     } else {
//         disconnect(isHandler());
//         disconnect(ControlManager::instance().data());
//         disconnect(ConfigSetting::instance().data());
//     }
}

void GuiExcel::drawDisplayDepth0() {
    updateDisplayVisible();
    updateDisplayNodeAddress(AutoComplete::Draw, nullptr, nullptr);
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

void GuiExcel::updateDisplayNodeAddress(const AutoComplete& type, QTableWidget* sheet, QTableWidgetItem* item) {
    // qDebug() << "updateDisplayNodeAddress :" << static_cast<int>(type) << sheet << item;

    switch (type) {
        case AutoComplete::Draw : {
            mInputNodeAddress = new QLineEdit(mMainView);
            mInputNodeAddress->setStyleSheet("background-color: rgb(230, 230, 230); color: black; font-size:12px");
            mInputNodeAddress->setTextMargins(1, 0, 0, 0);
            mInputNodeAddress->setFrame(false);
            mInputNodeAddress->setEnabled(true);
            if (mAutoComplete == nullptr) {
                mAutoComplete = new QCompleter(mInputNodeAddress);
                mAutoComplete->setCaseSensitivity(Qt::CaseInsensitive);
                mAutoComplete->setFilterMode(Qt::MatchContains);
                mAutoComplete->setWrapAround(false);
                mAutoComplete->setMaxVisibleItems(20);
                // mAutoComplete->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);
                connect(mAutoComplete, QOverload<const QString &>::of(&QCompleter::activated), [=](const QString &text) {
                    if (mCurrentCellItem) {
                        mCurrentCellItem->setText(text);
                    }
                    if (mInputNodeAddress) {
                        mInputNodeAddress->clear();
                        mInputNodeAddress->hide();
                        mCurrentCellItem = nullptr;
                    }
                });
            }
            break;
        }
        case AutoComplete::Update : {
            QVariant nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll);
            if ((mInputNodeAddress) && (mAutoComplete) && (nodeAddress.toStringList().size() > 0)) {
                mNodeAddressListModel.setStringList(nodeAddress.toStringList());
                mAutoComplete->setModel(&mNodeAddressListModel);
                mInputNodeAddress->setCompleter(mAutoComplete);
            }
            break;
        }
        case AutoComplete::Show : {
            if ((mInputNodeAddress) && (sheet)) {
#if defined(USE_AUTO_COMPLETE_NORMAL)
                QModelIndexList modelIndexs = sheet->selectionModel()->selectedIndexes();
                foreach(const QModelIndex& modelIndex, modelIndexs) {
                    QRect rect = sheet->visualRect(modelIndex);
                    int marginX = sheet->verticalHeader()->geometry().x() + sheet->verticalHeader()->geometry().width() + 2;
                    int marginY = 47;
                    QRect setRect = QRect(rect.x() + marginX, rect.y() + marginY, rect.width(), rect.height());
                    mInputNodeAddress->setText(item->text());
                    mInputNodeAddress->raise();
                    mInputNodeAddress->setGeometry(setRect);
                    mInputNodeAddress->activateWindow();
                    mInputNodeAddress->setFocus();
                    mInputNodeAddress->show();
                }
#else
                if (sheet) {
                    sheet->setCellWidget(item->row(), item->column(), mInputNodeAddress);
                    qDebug() << "\t Show -" << item->row() << item->column();
                }
#endif
                mCurrentSheet = sheet;
                mCurrentCellItem = item;
            }
            break;
        }
        case AutoComplete::Hide : {
#if defined(USE_AUTO_COMPLETE_NORMAL)
            if (mInputNodeAddress) {
                mInputNodeAddress->hide();
                QString inputText = mInputNodeAddress->text();
                if (inputText.size() > 0) {
                    if (mCurrentCellItem) {
                        mCurrentCellItem->setText(inputText);
                    }
                    mInputNodeAddress->clear();
                    if (mCurrentCellItem) {
                        mCurrentCellItem = nullptr;
                    }
                }
                // mCurrentSheet->setFocus();
                mCurrentSheet = sheet;
                mCurrentCellItem = item;
            }
#else
            if ((sheet) && (mCurrentCellItem)) {
                    qDebug() << "\t Hide -" << mCurrentCellItem->row() << mCurrentCellItem->column();
                sheet->removeCellWidget(mCurrentCellItem->row(), mCurrentCellItem->column());
            }
            mCurrentSheet = sheet;
            mCurrentCellItem = item;
#endif
            break;
        }
        case AutoComplete::Cancel : {
            if (mInputNodeAddress) {
                mInputNodeAddress->hide();
                mInputNodeAddress->clear();
            }
            break;
        }
        default : {
            break;
        }
    }

    if ((mInputNodeAddress == nullptr) || (mAutoComplete == nullptr)) {
        qDebug() << "Fail to create instnace - mInputNodeAddress :" << mInputNodeAddress << ", mAutoComplete :" << mAutoComplete;
    }
}

#if defined(USE_EXCEL_FUNCTION_NEW)
QVariantList GuiExcel::readExcelSheet(const int& sheetIndex) {
    QVariantList sheetData = QVariantList();

    if ((mExcelSheet[sheetIndex] == nullptr) || (mExcelSheet.size() == 0)) {
        qDebug() << "Fail to excel sheet nullptr : " << sheetIndex << ", size :" << mExcelSheet.size();
        return sheetData;
    }

    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    QMap<int, QList<QPair<int, int>>> mergeInfo = mCellInfo[sheetIndex].isMergeInfo();

    // Read Title
    QStringList title = QStringList();
    for (int columnIndex = 0; columnIndex < columnMax; columnIndex++) {
        title.append(mExcelSheet[sheetIndex]->horizontalHeaderItem(columnIndex)->text());
    }
    sheetData.append(title);

    // Read Row Data
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);
    for (int rowIndex = 0; rowIndex < rowMax; rowIndex++) {
        QStringList rowData = QStringList();
        for (int columnIndex = 0; columnIndex < columnMax; columnIndex++) {
            QString readText = ((mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)) ?
                                (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->text()) : (QString()));
            QString text = readText;
            foreach(const auto& info, mergeInfo[columnIndex]) {
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
                    qDebug() << "MergeText[" << sheetIndex << "][" << rowIndex << "," << columnIndex << "] :" << text;
                }
            }
            rowData.append(text);
            // qDebug() << "ReadText[" << sheetIndex << "][" << columnIndex << "," << rowIndex << "] :" << text;
        }
        sheetData.append(rowData);
    }
    // qDebug() << "readExcelSheet() ->"<< "Length :" << rowMax << columnMax << sheetData.size();
    // qDebug() << mExcelSheet[sheetIndex] << ":" << sheetData;
    // qDebug() << "==================================================================================================\n";

    return sheetData;
}

void GuiExcel::readAllExcelSheet() {
    if ((mMainView == nullptr) || (mExcelSheet.size() == 0)) {
        qDebug() << "Fail to read excel sheet - MainView :" << mMainView << ", ExcelSheet :" << mExcelSheet.size();
        return;
    }

    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    foreach(const auto& sheet, mExcelSheet) {
        QVariantList sheetData = readExcelSheet(sheetIndex);
        if (sheetData.size() > 0) {
            int eventType = ivis::common::EventTypeEnum::EventTypeListDescription;
            eventType += (sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription);
            createSignal(eventType, sheetData);
            sheetIndex++;
        }
    }
    createSignal(ivis::common::EventTypeEnum::EventTypeSaveFromReadExcelSheet, true);
}

void GuiExcel::updateDisplayMergeCell(const int& sheetIndex) {
    QMap<int, QList<QPair<int, int>>> mergeInfo = mCellInfo[sheetIndex].isMergeInfo();
    QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
    while (iter.hasNext()) {
        iter.next();
        int mergeColumnIndex = iter.key();
        foreach(const auto& v, iter.value()) {
            if (mExcelSheet[sheetIndex]) {
                int mergeRowStart = v.first;
                int mergeRowEnd = v.second;
                // qDebug() << "\t UpdateMergeCell :" << mergeColumnIndex << mergeRowStart << mergeRowEnd;
                mExcelSheet[sheetIndex]->setSpan(mergeRowStart, mergeColumnIndex, mergeRowEnd, 1);
            }
        }
    }
}

void GuiExcel::updateDisplayCellInfo(const int& sheetIndex, const QVariantList& mergeInfo,
                                                                const QMap<int, QVariantList>& sheetData) {
    if (mergeInfo.size() != 3) {
        qDebug() << "Fail to merge text size :" << mergeInfo.size();
        return;
    }

    QString excelMergeTextStart = mergeInfo.at(0).toString();
    QString excelMergeTextEnd = mergeInfo.at(1).toString();
    QString excelMergeText = mergeInfo.at(2).toString();

    QMapIterator<int, QVariantList> iter(sheetData);
    while (iter.hasNext()) {
        iter.next();
        int columnIndex = iter.key();
        int rowIndex = 0;
        int rowStart = (-1);
        int rowEnd = (-1);
        foreach(const auto& v, iter.value()) {
            QString readText = v.toString();
            QStringList startText = readText.split(excelMergeTextStart);
            QStringList endText = readText.split(excelMergeTextEnd);
            QStringList mergeText = readText.split(excelMergeText);
            QString type = QString();

            if (startText.size() == 2) {
                type = "Start ";
                rowStart = rowIndex;
                rowEnd = (-1);
            } else if (endText.size() == 2) {
                type = "End   ";
                rowEnd = rowIndex - rowStart + 1;
            } else {
                if (mergeText.size() == 2) {
                    type = "Merge ";
                } else {
                    type = "Normal";
                }
            }
            rowIndex++;

            if ((rowStart >= 0) && (rowEnd >= 0)) {
                mCellInfo[sheetIndex].insertMergeInfo(columnIndex, rowStart, rowEnd);
                // qDebug() << "\t MergeCell[" << sheetIndex << "] :" << columnIndex << rowStart << rowEnd;
                rowStart = (-1);
                rowEnd = (-1);
            }
        }
        // qDebug() << "---------------------------------------------------------------------";
    }

    // Draw - Merge Cell
    updateDisplayMergeCell(sheetIndex);
}

void GuiExcel::updateDisplayExcelSheet() {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    bool excelOpen = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
    qDebug() << "Excel Update :" << ((excelOpen) ? ("Open") : ("New"));

    // Clear - Table, TableWidget, CellInfo, Menu, Action
    if (mExcelSheet.size() > 0) {
        foreach(const auto& s, mExcelSheet) {
            disconnect(s);
            delete s;
        }
        mExcelSheet.clear();
        mMainView->clear();

        foreach(const auto& a, mMenuActionItem) {
            delete a;
        }
        mMenuActionItem.clear();
        mMenuRight->clear();
    }
    mCellInfo.clear();

    mMenuActionItem[MenuItemRight::Insert] = mMenuRight->addAction("Insert");
    mMenuActionItem[MenuItemRight::Delete] = mMenuRight->addAction("Delete");
    mMenuActionItem[MenuItemRight::MergeSplit] = mMenuRight->addAction("Merge/Split");


    // Draw - Sheet
    QStringList sheetName = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName).toStringList();
    QStringList descTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle).toStringList();
    QStringList otherTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle).toStringList();
    QVariantList rowCount = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount).toList();
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    int rowIndexList = 0;
    foreach(const auto& sheet, sheetName) {
        QStringList contentTitle = (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) ?
                                                                            (descTitle) : (otherTitle);
        int rowMax = rowCount.at(rowIndexList++).toInt();
        int columnMax = contentTitle.size();

        // Draw Sheet
        mExcelSheet[sheetIndex] = new QTableWidget(rowMax, columnMax);
        mExcelSheet[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        mExcelSheet[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mMainView->addTab(mExcelSheet[sheetIndex], sheet);

        // Draw Sheet Data
        if (excelOpen) {
            QVariantList sheetData = isHandler()->getProperty(sheetIndex).toList();
            QMap<int, QVariantList> newSheetData = QMap<int, QVariantList>();

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
                    QString type = QString();

                    newSheetData[columnIndex].append(readText);

                    if (startText.size() == 2) {
                        type = "Start ";
                        setText = startText.at(1);
                    } else if (endText.size() == 2) {
                        type = "End   ";
                        setText = endText.at(1);
                    } else {
                        if (mergeText.size() == 2) {
                            type = "Merge ";
                            setText = mergeText.at(1);
                        } else {
                            type = "Normal";
                            setText = mergeText.at(0);
                        }
                    }
                    // qDebug() << type << "[" << rowIndex << columnIndex <<"] :" << setText;
                    mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(setText));
                }
            }

            QVariantList mergeInfo = QVariantList{excelMergeTextStart, excelMergeTextEnd, excelMergeText};
            updateDisplayCellInfo(sheetIndex, mergeInfo, newSheetData);
#if 0   // defined(USE_EXCEL_CELL_INFO_TEMP)
        } else {
            for (int rowIndex = 0; rowIndex < rowMax; rowIndex++) {
                for (int columnIndex = 0; columnIndex < columnMax; columnIndex++) {
                    QString text = QString("%1[%2_%3]").arg(sheet).arg(rowIndex).arg(columnIndex);
                    mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(text));
                }
            }
#endif
        }

        connect(mExcelSheet[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            QString text = mExcelSheet[sheetIndex]->item(row, column)->text();
            qDebug() << sheetIndex << ". cellChanged :" << column << "," << row << ", Text" << text;
            createSignal(ivis::common::EventTypeEnum::EventTypeEditExcelSheet,
                            QVariant(QVariantList({sheetIndex, column, row, text})));
            mExcelSheet[sheetIndex]->resizeColumnsToContents();
            mExcelSheet[sheetIndex]->resizeRowsToContents();
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
            bool selectInavalid = (modelIndexs.size() == 0);
            int columnStart = (selectInavalid) ? (1) : (modelIndexs.at(0).column());
            int columnEnd = (selectInavalid) ? (1) : (modelIndexs.last().column() - columnStart + 1);
            int rowStart = (selectInavalid) ? (0) : (modelIndexs.at(0).row());
            int rowEnd = (selectInavalid) ? (1) : (modelIndexs.last().row() - rowStart + 1);
            QAction* selectAction = mMenuRight->exec(mExcelSheet[sheetIndex]->mapToGlobal(QPoint((pos.x() + 20), (pos.y() + 5))));

            qDebug() << sheetIndex << ". cellSelected :" << pos << ", Index :" << columnStart << rowStart << rowEnd << columnEnd;

            if ((selectAction == mMenuActionItem[MenuItemRight::Insert])
                || (selectAction == mMenuActionItem[MenuItemRight::Delete])) {
                bool insert = (selectAction == mMenuActionItem[MenuItemRight::Insert]);
                for (int index = 0; index < rowEnd; index++) {
                    if (insert) {
                        mExcelSheet[sheetIndex]->insertRow(rowStart);
                    } else {
                        mExcelSheet[sheetIndex]->removeRow(rowStart);
                    }
                }
            } else if (selectAction == mMenuActionItem[MenuItemRight::MergeSplit]) {
                bool columnSelectError = (columnEnd > 1);
                if (columnSelectError) {
                    createSignal(ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning, QVariant());
                } else {
                    if (mCellInfo[sheetIndex].isCellStateMerge(columnStart, rowStart, rowEnd)) {
                        // qDebug() << "\t Split Cell :" << columnStart << rowStart << rowEnd;
                        mExcelSheet[sheetIndex]->clearSpans();
                        mCellInfo[sheetIndex].updateMergeInfo(columnStart, rowStart, rowEnd);
                    } else {
                        // qDebug() << "\t Insert Cell :" << columnStart << rowStart << rowEnd;
                        mCellInfo[sheetIndex].insertMergeInfo(columnStart, rowStart, rowEnd);
                    }
                    // Draw - Merge Cell
                    updateDisplayMergeCell(sheetIndex);
                }
            } else {
                qDebug() << "Fail to menu right selection action item";
                return;
            }

            createSignal(ivis::common::EventTypeEnum::EventTypeEditExcelSheet,
                            QVariant(QVariantList({sheetIndex, columnStart, rowStart, rowEnd})));
            mExcelSheet[sheetIndex]->resizeColumnsToContents();
            mExcelSheet[sheetIndex]->resizeRowsToContents();
        });

        // Resize - Cell Width/Height
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
        mExcelSheet[sheetIndex]->resizeRowsToContents();
        sheetIndex++;
    }
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

#else
void GuiExcel::updateDisplaySheetInfo(const int& type) {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    int updateSheetSize = isHandler()->getProperty(type).toInt();

    if (updateSheetSize == 0) {
        qDebug() << "Fail to update sheet info[" << type << "] size : 0";
        return;
    }

    qDebug() << "UpdateSheetSize[" << type << "] :" << updateSheetSize << ", CurrentSheetIndex :" << mMainView->currentIndex();

    // True : Update Cell Insert/Delete,  False : New, Open Update Sheet Info
    bool editSheet = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditSheet).toBool();
    mCurrentSheetIndex = (editSheet) ? (mMainView->currentIndex()) : (0);

    // Clear - Previous Table Widget
    mMainView->clear();
    foreach(const auto& s, mExcelSheet) {
        delete s;
    }
    mExcelSheet.clear();
    mExcelCellInfo.clear();

    QString sheetName = QString();
    QStringList contentTitle = QStringList();
    QString excelBlankText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelBlankText).toString();

    for (int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
            sheetIndex < (updateSheetSize + ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription); sheetIndex++) {
        QVariantList detailInfo = isHandler()->getProperty(sheetIndex).toList();
        // qDebug() << "Info[" << sheetIndex << "] =" << detailInfo << "\n\n";

        // Constructor - Detail List Info
        QString sheetName = detailInfo[ivis::common::CellInfoEnum::ListInfoExcel::Sheet].toString();
        QStringList contentTitle = detailInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title].toStringList();
        QVariantList count = detailInfo[ivis::common::CellInfoEnum::ListInfoExcel::Count].toList();

        if (count.size() != 2) {
            qDebug() << "Fail to detail info data size :" << count.size();
            return;
        }

        // Draw - Sheet/Title
        int rowCount = count.at(0).toInt();
        int columnCount = count.at(1).toInt();
        mExcelSheet[sheetIndex] = new QTableWidget(rowCount, columnCount);
        mExcelSheet[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        mExcelSheet[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mMainView->addTab(mExcelSheet[sheetIndex], sheetName);
        // qDebug() << "Sheet[" << sheetIndex << mExcelSheet[sheetIndex] << "] - Count :" << rowCount << "," << columnCount;

        // Draw - Detail List Data
        QMap<int, QList<QPair<int, int>>> mergeInfos;
        const int MERGEINFO_IVALID = (-1);
        for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
            QStringList detailInfoData = detailInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex].toStringList();
            for (int columnIndex = 0; columnIndex < detailInfoData.size(); columnIndex++) {
                QString cellText = detailInfoData[columnIndex];
                if (((columnIndex < 4))    // && (columnIndex != 1))    // (0:TCName, 1:VehicleType, 2:Result, 3:Case)
                    // && (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription)
                    // && (type == ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen)
                    ) {
#if defined(USE_EXCEL_CELL_INFO_TEMP)
                    if (cellText.compare("") == false) {
                        QList<QPair<int, int>> info = mergeInfos[columnIndex];
                        if (info.size() > 0) {
                            int startIndex = info.at(info.size() - 1).first;
                            int endIndex = rowIndex - startIndex + 1;
                            mergeInfos[columnIndex][info.size() - 1] = QPair<int, int>(startIndex, endIndex);
                        }
                    } else {
                        mergeInfos[columnIndex].append(QPair<int, int>(rowIndex, MERGEINFO_IVALID));
                    }
#else
                    if (editSheet) {
                        if (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
                            continue;
                        }
                        QStringList temp = cellText.split("[=]");
                        if (temp.size() == 4) {
                            mergeInfos[temp[1].toInt()].append(QPair<int, int>(temp[2].toInt(), MERGEINFO_IVALID));
                            // cellText = temp[3];
                            qDebug() << "1 Column :" << temp[1] << ", RowStart :" << temp[2] << ", Text :" << temp[3];
                        } else if (temp.size() == 3) {
                            QList<QPair<int, int>> info = mergeInfos[temp[1].toInt()];
                            if (info.size() > 0) {
                                int startIndex = info.at(info.size() - 1).first;
                                mergeInfos[temp[1].toInt()][info.size() - 1] = QPair<int, int>(startIndex, temp[2].toInt());
                            }
                            // cellText.clear();
                            qDebug() << "3 Column :" << temp[1] << ", RowEnd   :" << temp[2];
                            // if (temp[1].toInt()) {
                            //     qDebug() << "2 Text :" << temp[1] << "," << temp[1].toInt();
                            // } else {
                            //     qDebug() << "1 Text :" << temp[1] << ", not int value !!!!!";
                            // }
                        } else if (temp.size() == 2) {
                            // cellText.clear();
                            qDebug() << "2 Erase String(ExcelMergeText)";
                            // if (temp[0].contains("ExcelMergeText", Qt::CaseInsensitive)) {
                            //     qDebug() << "2 Erase String(ExcelMergeText)";
                            // }
                        } else {
                        }
                    }
#endif
                }

                if (cellText.compare(excelBlankText) == false) {
                    cellText.clear();
                }

                mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(cellText));
                // QFont font;
                // font.setPixelSize(12);
                // mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->setFont(font);
            }
        }

#if 1  // defined(USE_EXCEL_CELL_INFO_TEMP)
        // mergeInfos[1] = mergeInfos[0];  // VehicleType 공백인 경우 존재하여 TCName 값으로 대입함.
        // Draw - Merge Cell : (0:TCName, 1:VehicleType, 2:Result, 3:Case)
        for (int mergeColumnIndex = 0; mergeColumnIndex < mergeInfos.size(); mergeColumnIndex++) {
            QList<QPair<int, int>> mergeInfo = mergeInfos[mergeColumnIndex];
            for (int mergeIndex = 0; mergeIndex < mergeInfo.size(); mergeIndex++) {
                int start = mergeInfo.at(mergeIndex).first;
                int end = mergeInfo.at(mergeIndex).second;
                if (end != MERGEINFO_IVALID) {
                    mExcelSheet[sheetIndex]->setSpan(start, mergeColumnIndex, end, 1);
                    mExcelCellInfo[sheetIndex].append(CellInfo(start, mergeColumnIndex, end));
                    qDebug() << "\t MergeInfo[" << mergeColumnIndex << "] :" << mergeInfo.at(mergeIndex);
                }
            }
        }
#endif

        // Draw - Cell Text Width/Height
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
        mExcelSheet[sheetIndex]->resizeRowsToContents();

        // Connect - Signal
        connect(mExcelSheet[sheetIndex]->horizontalHeader(), &QHeaderView::sectionResized, [=](int logicalIndex,
                                                                                        int oldSize, int newSize) {
            Q_UNUSED(logicalIndex)
            Q_UNUSED(oldSize)
            Q_UNUSED(newSize)
            // qDebug() << sheetIndex << ". H sectionResized :" << logicalIndex << oldSize << newSize;
            updateDisplayNodeAddress(AutoComplete::Hide, nullptr, nullptr);
        });
        connect(mExcelSheet[sheetIndex]->verticalHeader(), &QHeaderView::sectionResized, [=](int logicalIndex,
                                                                                        int oldSize, int newSize) {
            Q_UNUSED(logicalIndex)
            Q_UNUSED(oldSize)
            Q_UNUSED(newSize)
            // qDebug() << sheetIndex << ". V sectionResized :" << logicalIndex << oldSize << newSize;
            updateDisplayNodeAddress(AutoComplete::Hide, nullptr, nullptr);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemSelectionChanged, [=]() {
            qDebug() << sheetIndex << ". itemSelectionChanged :" << mExcelSheet[sheetIndex]->selectedItems();
            if (mExcelSheet[sheetIndex]->selectedItems().size() == 0) {
                updateDisplayNodeAddress(AutoComplete::Hide, nullptr, nullptr);
            }
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::currentItemChanged, [=](QTableWidgetItem *current,
                                                                                QTableWidgetItem *previous) {
            qDebug() << sheetIndex << ". currentItemChanged :" << current << previous;
#if defined(USE_AUTO_COMPLETE_NORMAL)
            updateDisplayNodeAddress(AutoComplete::Hide, mExcelSheet[sheetIndex], current);
#else
            mCurrentSheet = mExcelSheet[sheetIndex];
            // mCurrentCellItem = current;
            updateDisplayNodeAddress(AutoComplete::Hide, mExcelSheet[sheetIndex], mCurrentCellItem);
#endif
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemDoubleClicked, [=](QTableWidgetItem *item) {
            qDebug() << sheetIndex << ". itemDoubleClicked :" << item << item->column() << item->row();
            // if (((item->column() != 4) && (item->column() != 6) && (item->column() != 9))) {
            //     qDebug() << "Fail to not draw auto complete - column :" << item->column();
            //     return;
            // }
#if !defined(USE_AUTO_COMPLETE_NORMAL)
            mCurrentSheet = mExcelSheet[sheetIndex];
            mCurrentCellItem = item;
#endif
            updateDisplayNodeAddress(AutoComplete::Show, mExcelSheet[sheetIndex], item);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            // updateDisplayNodeAddress(AutoComplete::Show, mExcelSheet[sheetIndex], mCurrentCellItem);

            QString text = mExcelSheet[sheetIndex]->item(row, column)->text();
            qDebug() << sheetIndex << ". cellChanged :" << column << "," << row << ", Text" << text;
            mExcelSheet[sheetIndex]->resizeColumnsToContents();
            mExcelSheet[sheetIndex]->resizeRowsToContents();
#if defined(USE_EXCEL_FUNCTION_NORMAL)
            createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetTextInfo,
                            QVariant(QVariantList({sheetIndex, column, row, text})));
#endif
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
            int curretSheetRowCount = mExcelSheet[sheetIndex]->rowCount();
            qDebug() << sheetIndex << ". MenuRightClick : " << pos << modelIndexs.size() << curretSheetRowCount;
            bool selectCellInfo = ((modelIndexs.size() == 0)/*&& (curretSheetRowCount == 0)*/);

            if (selectCellInfo) {
                qDebug() << "Menu Right Display : Only Insert -  modalindex size : 0";
            }

            int columnStart = (selectCellInfo) ? (1) : (modelIndexs.at(0).column());
            int columnEnd = (selectCellInfo) ? (1) : (modelIndexs.last().column() - columnStart + 1);
            int rowStart = (selectCellInfo) ? (0) : (modelIndexs.at(0).row());
            int rowEnd = (selectCellInfo) ? (1) : (modelIndexs.last().row() - rowStart + 1);
            qDebug() << sheetIndex << ". cellSelected :" << "," << columnStart << "," << columnEnd
                                    << "," << rowStart << "," << rowEnd;

            QMenu* menuRight = new QMenu(isHandler()->getScreen());
            QMap<MenuItemRight, QAction*> menuItem = QMap<MenuItemRight, QAction*>();

            menuItem[MenuItemRight::RowInsert] = menuRight->addAction("Insert");
            if (selectCellInfo == false) {
                menuItem[MenuItemRight::RowDelete] = menuRight->addAction("Delete");
                menuItem[MenuItemRight::CellMergeSplit] = menuRight->addAction("Merge/Split");
            }

            QPoint newPos = pos;
            newPos.setX(pos.x() + 20);
            newPos.setY(pos.y() + 5);
            QAction* selectAction = menuRight->exec(mExcelSheet[sheetIndex]->mapToGlobal(newPos));
            MenuItemRight selectMenuItem = MenuItemRight::Invalid;

            if (selectAction == menuItem[MenuItemRight::RowInsert]) {
                selectMenuItem = MenuItemRight::RowInsert;
            } else {
                if (selectCellInfo == false) {
                    if (selectAction == menuItem[MenuItemRight::RowDelete]) {
                        selectMenuItem = MenuItemRight::RowDelete;
                    } else if (selectAction == menuItem[MenuItemRight::CellMergeSplit]) {
                        selectMenuItem = MenuItemRight::CellMergeSplit;
                    } else {
                    }
                }
            }
            qDebug() << "selectMenuItem :" << static_cast<int>(selectMenuItem);

            if (selectMenuItem == MenuItemRight::Invalid) {
                qDebug() << "Fail to select right menu item.";
                return;
            }


            if (selectMenuItem == MenuItemRight::RowInsert) {
                createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetCellInfo, QVariant(QVariantList({
                                                                        ivis::common::EditCellEnum::EditCellInfo::Insert,
                                                                        sheetIndex,
                                                                        columnStart, columnEnd,
                                                                        rowStart, rowEnd})));
            } else if (selectMenuItem == MenuItemRight::RowDelete) {
                createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetCellInfo, QVariant(QVariantList({
                                                                        ivis::common::EditCellEnum::EditCellInfo::Delete,
                                                                        sheetIndex,
                                                                        columnStart, columnEnd,
                                                                        rowStart, rowEnd})));
            } else {
#if defined(USE_EXCEL_FUNCTION_NORMAL)
                if (((columnEnd == 1) && (columnStart < 4))) {
                    ivis::common::EditCellEnum::EditCellInfo editType = ivis::common::EditCellEnum::EditCellInfo::Merge;
                    if (mExcelCellInfo[sheetIndex].size() > 0) {
                        QList<CellInfo> newCellInfo = QList<CellInfo>();
                        for (auto& cellInfo : mExcelCellInfo[sheetIndex]) {
                            if (cellInfo.isMergeCell(rowStart, columnStart, rowEnd)) {
                                editType = ivis::common::EditCellEnum::EditCellInfo::Split;
                            } else {
                                newCellInfo.append(cellInfo);
                            }
                        }
                        mExcelCellInfo[sheetIndex] = newCellInfo;
                    } else {
                        mExcelCellInfo[sheetIndex].append(CellInfo(rowStart, columnStart, rowEnd));
                    }

                    createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetCellInfo, QVariant(QVariantList({
                                                                        editType,
                                                                        sheetIndex,
                                                                        columnStart, columnEnd,
                                                                        rowStart, rowEnd})));
                } else {
                    createSignal(ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning, QVariant());
                }
#else

#endif
            }
        });
    }


    // Set : Current Sheet Index
    mMainView->setCurrentIndex(mCurrentSheetIndex);
    // if (mExcelSheet[mCurrentSheetIndex]) {
    //     mExcelSheet[mCurrentSheetIndex]->setCurrentCell(1, 1);
    // }
    connect(mMainView, &QTabWidget::currentChanged, [=](int index) {
        if (index >= 0) {
            mMainView->setCurrentIndex(index);
            updateDisplayNodeAddress(AutoComplete::Hide, nullptr, nullptr);
        }
    });
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

void GuiExcel::readExcelInfo() {
    if ((mMainView == nullptr) || (mExcelSheet.size() == 0)) {
        qDebug() << "Fail to read excel info - MainView :" << mMainView << ", ExcelSheet :" << mExcelSheet.size();
        return;
    }
    qDebug() << "readExcelInfo() -> MainView :" << mMainView->count() << ", ExcelSheet :" << mExcelSheet.size();

    QMap<int, QVariantList> excelDataInfo = QMap<int, QVariantList>();
    int sheetIndex = 0;
    foreach(const auto& sheet, mExcelSheet) {
        QMap<int, QVariant> dataInfo = QMap<int, QVariant>();
        int rowCount = sheet->rowCount();
        int columnCount = sheet->columnCount();

        dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Sheet] = QVariant(mMainView->tabText(sheetIndex));
        QStringList title = QStringList();
        for (int columnIndex = 0; columnIndex < sheet->columnCount(); columnIndex++) {
            title.append(sheet->horizontalHeaderItem(columnIndex)->text());
        }
        dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title] = QVariant(title);
        dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Count] = QVariant(QVariantList({rowCount, columnCount}));
        for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
            QVariantList rowData = QVariantList();
            for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                rowData.append(sheet->item(rowIndex, columnIndex)->text());
            }
            dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex] = rowData;
        }

        excelDataInfo[sheetIndex++] = dataInfo.values();

        qDebug() << "\n\n=====================================================================================";
        qDebug() << "Sheet :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Sheet];
        qDebug() << "Count :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Count];
        qDebug() << "Title :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Title];
        for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
            qDebug() << "Data :" << dataInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex];
        }
    }
    // createSignal(ivis::common::EventTypeEnum::EventTypeReadExcelInfo, QVariant(excelDataInfo));
}
#endif

void GuiExcel::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth : {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize : {
            updateDisplaySize();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible : {
            updateDisplayVisible();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll : {
            updateDisplayNodeAddress(AutoComplete::Update, nullptr, nullptr);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete : {
            if ((mCurrentSheet) && (mCurrentCellItem)) {
                AutoComplete type = AutoComplete::Hide;
                int inputType = value.toInt();
                if (inputType == 1) {
                    type = AutoComplete::Show;
                } else if (inputType == 2) {
                    type = AutoComplete::Cancel;
                } else {
                    type = AutoComplete::Hide;
                }
                updateDisplayNodeAddress(type, mCurrentSheet, mCurrentCellItem);
            } else {
                qDebug() << "ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete";
            }
            break;
        }
#if defined(USE_EXCEL_FUNCTION_NEW)
        case ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen : {
            updateDisplayExcelSheet();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeReadExcelSheet : {
            readAllExcelSheet();
            break;
        }
#else
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew :
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen : {
            updateDisplaySheetInfo(type);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeReadExcelSheet : {
            readExcelInfo();
            break;
        }
#endif
        default : {
            break;
        }
    }
}
