#include "GuiExcel.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"

#include <QMenu>
#include <QLabel>
#include <QTextEdit>



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

void GuiExcel::updateDisplaySheetInfo(const int& type) {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    int updateSheetSize = isHandler()->getProperty(type).toInt();

    if (updateSheetSize == 0) {
        qDebug() << "Fail to update sheet info[" << type << "] size : 0";
        return;
    } else {
        qDebug() << "UpdateSheetSize[" << type << "] :" << updateSheetSize
                    << ", CurrentSheetIndex :" << mMainView->currentIndex();
    }

    // True : Update Cell Insert/Delete,  False : New, Open Update Sheet Info
    bool editSheet = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditSheet).toBool();
    int currentSheetIndex = (editSheet) ? (mMainView->currentIndex()) : (0);

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


    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    updateSheetSize = (updateSheetSize + ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription);

    for ( ; sheetIndex < updateSheetSize; sheetIndex++) {
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
        qDebug() << "Sheet[" << sheetIndex << mExcelSheet[sheetIndex] << "] - Count :" << rowCount << "," << columnCount;

        // Draw - Detail List Data
        QMap<int, QList<QPair<int, int>>> mergeInfos;
        const int MERGEINFO_IVALID = (-1);
        for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
            QStringList detailInfoData = detailInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex].toStringList();
            for (int columnIndex = 0; columnIndex < detailInfoData.size(); columnIndex++) {
                QString data = detailInfoData[columnIndex];
                if (((columnIndex < 4))    // && (columnIndex != 1))    // (0:TCName, 1:VehicleType, 2:Result, 3:Case)
                    // && (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription)
                    // && (type == ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen)
                    ) {
#if defined(CELL_INFO_TEMP)
                    if (data.compare("") == false) {
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
                    if (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
                        continue;
                    }
                    QStringList temp = data.split("ExcelMergeText");
                    if (temp.size() == 2) {
                        QList<QPair<int, int>> info = mergeInfos[columnIndex];
                        if (info.size() > 0) {
                            qDebug() << "2. Append :" << rowIndex << columnIndex;
                            int startIndex = info.at(info.size() - 1).first;
                            int endIndex = rowIndex - startIndex + 1;
                            mergeInfos[columnIndex][info.size() - 1] = QPair<int, int>(startIndex, endIndex);
                        }


                        // if (mergeIndexInfo.first == (-1)) {
                        //     int startIndex = (temp[1].toInt() == rowIndex) ? (0) : (rowIndex);
                        //     mergeIndexInfo = QPair<int, int>(startIndex, 0);
                        // } else {
                        //     mergeIndexInfo = QPair<int, int>(mergeIndexInfo.first, temp[1].toInt());
                        // }

                        qDebug() << "\t MergeInfo[" << rowIndex << "," << columnIndex << "] :" << data
                                        << "," << temp[1].toInt() << "," << rowIndex;
                    } else {
                        qDebug() << "1. First  :" << rowIndex << columnIndex;
                        mergeInfos[columnIndex].append(QPair<int, int>(rowIndex, MERGEINFO_IVALID));
                    }
#endif
                }

                if (data.compare(excelBlankText) == false) {
                    data.clear();
                }
                // QTableWidgetItem *detailDataItem = new QTableWidgetItem(data);
                // detailDataItem->setTextAlignment(Qt::AlignCenter);
                mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(data));
            }
        }

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

        // Draw - Cell Text Width/Height
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
        mExcelSheet[sheetIndex]->resizeRowsToContents();

        // Connect - Signal
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            QString text = mExcelSheet[sheetIndex]->item(row, column)->text();
            qDebug() << sheetIndex << ". cellChanged :" << row << "," << column << ", Text" << text;
            createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetTextInfo,
                            QVariant(QVariantList({sheetIndex, row, column, text})));
            mExcelSheet[sheetIndex]->resizeColumnsToContents();
            mExcelSheet[sheetIndex]->resizeRowsToContents();
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
            qDebug() << sheetIndex << ". MenuRightClick : " << pos;    // << ", SelectItem :" << modelIndexs;
            bool sheetInfoNull = (modelIndexs.size() == 0);

            if (sheetInfoNull) {
                qDebug() << "Fail to menu right click - modalindex size : 0 -> only display menu : insert";
            }

            int rowStart = (sheetInfoNull) ? (0) : (modelIndexs.at(0).row());
            int columnStart = (sheetInfoNull) ? (1) : (modelIndexs.at(0).column());
            int rowEnd = (sheetInfoNull) ? (1) : (modelIndexs.last().row() - rowStart + 1);
            int columnEnd = (sheetInfoNull) ? (1) : (modelIndexs.last().column() - columnStart + 1);
            qDebug() << sheetIndex << ". cellSelected :" << rowStart << "," << columnStart << "," << rowEnd << "," << columnEnd;

            QMenu* menuRight = new QMenu(isHandler()->getScreen());
            QMap<MenuItemRight, QAction*> menuItem = QMap<MenuItemRight, QAction*>();

            menuItem[MenuItemRight::RowInsert] = menuRight->addAction("Insert");
            if (sheetInfoNull == false) {
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
            } else if (selectAction == menuItem[MenuItemRight::RowDelete]) {
                selectMenuItem = MenuItemRight::RowDelete;
            } else if (selectAction == menuItem[MenuItemRight::CellMergeSplit]) {
                selectMenuItem = MenuItemRight::CellMergeSplit;
            } else {
                qDebug() << "Fail to select right menu item.";
                return;
            }

            if (selectMenuItem == MenuItemRight::RowInsert) {
                createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetCellInfo, QVariant(QVariantList({
                                                                        ivis::common::EditCellEnum::EditCellInfo::Insert,
                                                                        sheetIndex,
                                                                        0, rowStart, rowEnd})));
            } else if (selectMenuItem == MenuItemRight::RowDelete) {
                createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetCellInfo, QVariant(QVariantList({
                                                                        ivis::common::EditCellEnum::EditCellInfo::Delete,
                                                                        sheetIndex,
                                                                        0, rowStart, rowEnd})));
            } else {
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
                                                                        columnStart, rowStart, rowEnd})));
                } else {
                    createSignal(ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning, QVariant());
                }
            }
        });
    }


    // Set : Current Sheet Index
    mMainView->setCurrentIndex(currentSheetIndex);
    // if (editSheet) {
    //     mExcelSheet[sheetIndex]->setCurrentCell(20, 1);
    // }
    connect(mMainView, &QTabWidget::currentChanged, [=](int index) {
        if (index >= 0) {
            mMainView->setCurrentIndex(index);
        }
    });
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
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