#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"

#include <QMenu>
#include <QLabel>
#include <QTextEdit>



QSharedPointer<GuiCenter>& GuiCenter::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiCenter> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiCenter>(new GuiCenter(handler));
    }
    return gGui;
}

GuiCenter::GuiCenter(AbstractHandler* handler) : AbstractGui(handler) {
    mTabWidget = new QTabWidget(isHandler()->getScreen());
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

void GuiCenter::drawDisplayDepth0() {
    updateDisplay(true, ivis::common::PropertyTypeEnum::PropertyTypeVisible);
}

void GuiCenter::drawDisplayDepth1() {
    qDebug() << "GuiCenter::drawDisplayDepth1()";
}

void GuiCenter::drawDisplayDepth2() {
    qDebug() << "GuiCenter::drawDisplayDepth2()";
}

void GuiCenter::updateDisplaySize() {
    QRect rect = isHandler()->getScreen()->geometry();
    QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    QSize margin = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin).toSize();

    if (margin.width() > 0) {
        rect.setX(margin.width());
        rect.setWidth(size.width() - margin.width());
    }
    if (margin.height() > 0) {
        rect.setY(margin.height());
        rect.setHeight(size.height() - margin.height());
    }

    mTabWidget->setGeometry(rect);
}

void GuiCenter::updateDisplayVisible() {
    if (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        mTabWidget->show();
    } else {
        mTabWidget->hide();
    }
}

void GuiCenter::updateDisplaySheetInfo(const int& type) {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    int updateSheetSize = isHandler()->getProperty(type).toInt();

    if (updateSheetSize == 0) {
        qDebug() << "Fail to update sheet info[" << type << "] size : 0";
        return;
    } else {
        qDebug() << "UpdateSheetSize[" << type << "] :" << updateSheetSize
                    << ", CurrentSheetIndex :" << mTabWidget->currentIndex();
    }

    // True : Update Cell Insert/Delete,  False : New, Open Update Sheet Info
    bool editSheet = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateEditSheet).toBool();
    int currentSheetIndex = (editSheet) ? (mTabWidget->currentIndex()) : (0);
    // Clear - Previous Table Widget
    mTabWidget->clear();
    mTableWidgets.clear();
    mCellInfo.clear();

    QString sheetName = QString();
    QStringList contentTitle = QStringList();


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
        mTableWidgets[sheetIndex] = new QTableWidget(rowCount, columnCount);
        mTableWidgets[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        mTableWidgets[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mTabWidget->addTab(mTableWidgets[sheetIndex], sheetName);
        qDebug() << "Sheet[" << sheetIndex << "] - Count :" << rowCount << "," << columnCount;

        // Draw - Detail List Data
        QMap<int, QList<QPair<int, int>>> mergeInfos;
        const int MERGEINFO_IVALID = (-1);

        for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
            QStringList detailInfoData = detailInfo[ivis::common::CellInfoEnum::ListInfoExcel::Data + rowIndex].toStringList();
            for (int columnIndex = 0; columnIndex < detailInfoData.size(); columnIndex++) {
                QString data = detailInfoData[columnIndex];
                if (((columnIndex < 4) && (columnIndex != 1))    // (0:TCName, 1:VehicleType, 2:Result, 3:Case)
                    // && (sheetIndex != ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription)
                    // && (type == ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen)
                    ) {
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
                }
                QTableWidgetItem *detailDataItem = new QTableWidgetItem(data);
                // detailDataItem->setTextAlignment(Qt::AlignCenter);
                mTableWidgets[sheetIndex]->setItem(rowIndex, columnIndex, detailDataItem);
            }
        }
        mergeInfos[1] = mergeInfos[0];  // VehicleType 공백인 경우 존재하여 TCName 값으로 대입함.
        // Draw - Merge Cell : (0:TCName, 1:VehicleType, 2:Result, 3:Case)
        for (int titleIndex = 0; titleIndex < mergeInfos.size(); titleIndex++) {
            QList<QPair<int, int>> mergeInfo = mergeInfos[titleIndex];
            for (int mergeIndex = 0; mergeIndex < mergeInfo.size(); mergeIndex++) {
                int start = mergeInfo.at(mergeIndex).first;
                int end = mergeInfo.at(mergeIndex).second;
                if (end != MERGEINFO_IVALID) {
                    mTableWidgets[sheetIndex]->setSpan(start, titleIndex, end, 1);
                    mCellInfo[sheetIndex].append(CellInfo(start, titleIndex, end));
                    // qDebug() << "\t CellMergeInfo[" << titleIndex << "] :" << mergeInfo.at(mergeIndex);
                }
            }
        }

        // Draw - Cell Text Width/Height
        mTableWidgets[sheetIndex]->resizeColumnsToContents();
        mTableWidgets[sheetIndex]->resizeRowsToContents();

        // Connect - Signal
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            QString text = mTableWidgets[sheetIndex]->item(row, column)->text();
            qDebug() << sheetIndex << ". cellChanged :" << row << "," << column << ", Text" << text;
            createSignal(ivis::common::EventTypeEnum::EventTypeUpdateSheetInfo,
                            QVariant(QVariantList({sheetIndex, row, column, text})));
            mTableWidgets[sheetIndex]->resizeColumnsToContents();
            mTableWidgets[sheetIndex]->resizeRowsToContents();
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            QModelIndexList modelIndexs = mTableWidgets[sheetIndex]->selectionModel()->selectedIndexes();
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
            QAction* selectAction = menuRight->exec(mTableWidgets[sheetIndex]->mapToGlobal(newPos));
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
                createSignal(ivis::common::EventTypeEnum::EventTypeSheetRowInsert,
                                QVariant(QVariantList({sheetIndex, rowStart, rowEnd})));
            } else if (selectMenuItem == MenuItemRight::RowDelete) {
                createSignal(ivis::common::EventTypeEnum::EventTypeSheetRowDelete,
                                QVariant(QVariantList({sheetIndex, rowStart, rowEnd})));
            } else {
                // std::sort(modelIndexs.begin(), modelIndexs.end());
                bool checkValidColumn = ((columnEnd == 1) && (columnStart < 4));
                if (checkValidColumn) {
                    bool mergeCell = true;
                    if (mCellInfo[sheetIndex].size() > 0) {
                        QList<CellInfo> newCellInfo = QList<CellInfo>();
                        for (auto& cellInfo : mCellInfo[sheetIndex]) {
                            if (cellInfo.isMergeCell(rowStart, columnStart, rowEnd)) {
                                mergeCell = false;
                            } else {
                                newCellInfo.append(cellInfo);
                            }
                        }
                        mCellInfo[sheetIndex] = newCellInfo;
                    }

                    if (mergeCell) {
                        mCellInfo[sheetIndex].append(CellInfo(rowStart, columnStart, rowEnd));
                        mTableWidgets[sheetIndex]->setSpan(rowStart, columnStart, rowEnd, columnEnd);
                    } else {
                        mTableWidgets[sheetIndex]->clearSpans();
                        for (auto& cellInfo : mCellInfo[sheetIndex]) {
                            cellInfo.isCellInfo(rowStart, columnStart, rowEnd);
                            mTableWidgets[sheetIndex]->setSpan(rowStart, columnStart, rowEnd, 1);
                        }
                    }
                    createSignal(ivis::common::EventTypeEnum::EventTypeCellMergeSplit,
                                    QVariant(QVariantList({sheetIndex, columnStart, rowStart, rowEnd, mergeCell})));
                } else {
                    createSignal(ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning, QVariant());
                    qDebug() << "Fail to select column cell";
                }
            }
        });
    }


    // Set : Current Sheet Index
    mTabWidget->setCurrentIndex(currentSheetIndex);
    if (editSheet) {
        mTableWidgets[sheetIndex]->setCurrentCell(20, 1);
    }
    connect(mTabWidget, &QTabWidget::currentChanged, [=](int index) {
        if (index >= 0) {
            mTabWidget->setCurrentIndex(index);
        }
    });
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

void GuiCenter::updateDisplay(const bool& first, const int& type) {
    if (type == ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize) {
        updateDisplaySize();
    } else if (type == ivis::common::PropertyTypeEnum::PropertyTypeVisible) {
        updateDisplayVisible();
    } else if ((type == ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew)
        || (type == ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen)) {
        updateDisplaySheetInfo(type);
    } else {
    }
}

void GuiCenter::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth : {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize :
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible :
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew :
        case ivis::common::PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen : {
            updateDisplay(false, type);
            break;
        }
        default : {
            break;
        }
    }
}
