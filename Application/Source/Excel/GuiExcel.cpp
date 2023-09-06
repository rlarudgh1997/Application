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
            mInputNodeAddress->setStyleSheet("background-color: rgb(200, 200, 200); color: black; font: bold; font-size:12px");
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
            if (mInputNodeAddress) {
#if defined(USE_AUTO_COMPLETE_NORMAL)
                QModelIndexList modelIndexs = sheet->selectionModel()->selectedIndexes();
                foreach(const QModelIndex& modelIndex, modelIndexs) {
                    QRect rect = sheet->visualRect(modelIndex);
                    int xMargin = (sheet->rowCount() >= 100) ? (33) : (24);
                    QRect setRect = QRect(rect.x() + xMargin, rect.y() + 47, rect.width(), rect.height());
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
                    if (mInputNodeAddress) {
                        mInputNodeAddress->clear();
                        mInputNodeAddress->hide();
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
        default : {
            break;
        }
    }

    if ((mInputNodeAddress == nullptr) || (mAutoComplete == nullptr)) {
        qDebug() << "Fail to create instnace - mInputNodeAddress :" << mInputNodeAddress << ", mAutoComplete :" << mAutoComplete;
    }
}

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
                AutoComplete type = (value.toBool()) ? (AutoComplete::Show) : (AutoComplete::Hide);
                updateDisplayNodeAddress(type, mCurrentSheet, mCurrentCellItem);
            } else {
                qDebug() << "ivis::common::PropertyTypeEnum::PropertyTypeAutoComplete";
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeReadExcelInfo : {
            readExcelInfo();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew :
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen : {
            updateDisplaySheetInfo(type);
            break;
        }
        default : {
            break;
        }
    }
}


















#if 0
void GuiExcel::UpdateNodeAddressDialog(QTableWidgetItem* item) {
    // if (mInputNodeAddress == nullptr) {
        static QDialog* aaa = new QDialog(isHandler()->getScreen());

    if (1) {
#if 1
        QRect rect = isHandler()->getScreen()->geometry();
        // QRect setRect = QRect(rect.x());

        aaa->setGeometry(1200, 500, 600, 50);
        aaa->setModal(true);
        mInputNodeAddress = new QLineEdit(aaa);
        mInputNodeAddress->setGeometry(0, 0, 600, 50);
#else
        mInputNodeAddress = new QLineEdit();
        mInputNodeAddress->setGeometry(900, 50, 400, 30);
#endif
        mInputNodeAddress->setStyleSheet("background-color: white; color: black; font: bold; font-size:12px");
        mInputNodeAddress->activateWindow();
        mInputNodeAddress->setFocus();

        if (item) {
            mInputNodeAddress->show();
        } else {
            mInputNodeAddress->hide();
        }
        mCurrentCellItem = item;

        if (mAutoComplete == nullptr) {
            mAutoComplete = new QCompleter(mInputNodeAddress);
            mAutoComplete->setCaseSensitivity(Qt::CaseInsensitive);
            mAutoComplete->setFilterMode(Qt::MatchContains);
            mAutoComplete->setWrapAround(false);
            mAutoComplete->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);

            connect(mAutoComplete, QOverload<const QString &>::of(&QCompleter::activated), [=](const QString &text) {
                qDebug() << "activated :" << text << mCurrentCellItem;
                mCurrentCellItem->setText(text);
                aaa->close();
            });
        }
    }

    connect(mInputNodeAddress, &QWidget::destroyed, [=]() {
        qDebug() << "destroyed :" << mInputNodeAddress;
    });

    QStringList nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll).toStringList();
    if (mInputNodeAddress) {
        if ((mAutoComplete) && (nodeAddress.size() > 0)) {
            static QStringListModel* listModel = new QStringListModel(nodeAddress);
            mAutoComplete->setModel(listModel);
            mInputNodeAddress->setCompleter(mAutoComplete);
        }
    }
    mInputNodeAddress->raise();

        aaa->show();
        aaa->exec();
}
#endif
