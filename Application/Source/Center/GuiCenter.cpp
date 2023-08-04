#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"

#include <QMenu>



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
    updateDisplay(true, PropertyTypeEnum::PropertyTypeVisible);
}

void GuiCenter::drawDisplayDepth1() {
    qDebug() << "GuiCenter::drawDisplayDepth1()";
}

void GuiCenter::drawDisplayDepth2() {
    qDebug() << "GuiCenter::drawDisplayDepth2()";
}

void GuiCenter::updateDisplaySize() {
    QRect rect = isHandler()->getScreen()->geometry();
    QSize size = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    QSize margin = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeDisplaySizeMargin).toSize();

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
    if (isHandler()->getProperty(PropertyTypeEnum::PropertyTypeVisible).toBool()) {
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
    }
    qDebug() << "UpdateSheetSize[" << type << "] :" << updateSheetSize;

    // Clear - Previous Table Widget
    mTabWidget->clear();
    mTableWidgets.clear();

    QString sheetName = QString();
    QStringList contentTitle = QStringList();

    updateSheetSize = (PropertyTypeEnum::PropertyTypeDetailInfoDescription + updateSheetSize);

    for (int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription; sheetIndex < updateSheetSize; sheetIndex++) {
        QVariantList detailInfo = isHandler()->getProperty(sheetIndex).toList();
        // qDebug() << "Info[" << sheetIndex << "] =" << detailInfo << "\n\n";

        // Constructor - Detail List Info
        QString sheetName = detailInfo[ListInfoEnum::ListInfoExcel::Sheet].toString();
        QStringList contentTitle = detailInfo[ListInfoEnum::ListInfoExcel::Title].toStringList();
        QVariantList count = detailInfo[ListInfoEnum::ListInfoExcel::Count].toList();

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
            QStringList detailInfoData = detailInfo[ListInfoEnum::ListInfoExcel::Data + rowIndex].toStringList();
            for (int columnIndex = 0; columnIndex < detailInfoData.size(); columnIndex++) {
                QString data = detailInfoData[columnIndex];
                if ((type == PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen)
                    && (sheetIndex != PropertyTypeEnum::PropertyTypeDetailInfoDescription)
                    && ((columnIndex < 4) && (columnIndex != 1))) {    // (0:TCName, 1:VehicleType, 2:Result, 3:Case)
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
                    qDebug() << "\t CellMergeInfo[" << titleIndex << "] :" << mergeInfo.at(mergeIndex);
                }
            }
        }


        // Draw - Cell Text Width/Height
        mTableWidgets[sheetIndex]->resizeColumnsToContents();
        mTableWidgets[sheetIndex]->resizeRowsToContents();


        // Connect - Signal
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellDoubleClicked, [=](int row, int column) {
            qDebug() << sheetIndex << ". cellDoubleClicked : " << row << ", " << column;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            QString text = mTableWidgets[sheetIndex]->item(row, column)->text();
            // qDebug() << sheetIndex << ". cellChanged : " << row << ", " << column << ", Text" << text;
            createSignal(EventTypeEnum::EventTypeUpdateSheetInfo, QVariant(QVariantList({sheetIndex, row, column, text})));
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            QModelIndexList modelIndexs = mTableWidgets[sheetIndex]->selectionModel()->selectedIndexes();
            qDebug() << sheetIndex << ". customContextMenuRequested : " << pos;
            qDebug() << "[Info]\n" << modelIndexs << "\nSize :" << modelIndexs.size();

            QPoint gpos = mTableWidgets[sheetIndex]->mapToGlobal(pos);
            QMenu* menu = new QMenu(isHandler()->getScreen());
            menu->addAction("Insert");
            menu->addAction("Delete");
            QAction* selectAction = menu->exec(gpos);
            connect(selectAction, &QAction::triggered, [=]() {
                qDebug() << "triggered :";
            });
            connect(menu, &QMenu::triggered, [=](QAction *action) {
                qDebug() << "QMenu::triggered :" << action;
            });



            // static bool span = false;
            // if (modelIndexs.size() >= 2) {
            //     if (span) {
            //         span = false;
            //         mTableWidgets[sheetIndex]->clearSpans();
            //     } else {
            //         span = true;
            //         std::sort(modelIndexs.begin(), modelIndexs.end());
            //         auto row = modelIndexs.at(sheetIndex).row();
            //         auto col = modelIndexs.at(sheetIndex).column();
            //         auto rowSpan = modelIndexs.last().row() - row + 1;
            //         auto colSpan = modelIndexs.last().column() - col + 1;
            //         qDebug() << "Info :" << row << "," << col << "," << rowSpan << "," << colSpan;
            //         mTableWidgets[sheetIndex]->setSpan(row, col, rowSpan, colSpan);
            //     }
            // }
        });
#if 0
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellActivated, [=](int row, int column) {
            qDebug() << sheetIndex << ". cellActivated : " << row << ", " << column;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellEntered, [=](int row, int column) {
            qDebug() << sheetIndex << ". cellEntered : " << row << ", " << column;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::currentCellChanged,
                                            [=](int currentRow, int currentColumn, int previousRow, int previousColumn) {
            qDebug() << sheetIndex << ". currentCellChanged : " << currentRow << ", " << currentColumn
                        << ", " << previousRow << ", " << previousColumn;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::currentItemChanged,
                                            [=](QTableWidgetItem *current, QTableWidgetItem *previous) {
            qDebug() << sheetIndex << ". currentItemChanged : " << previous << " -> " << current;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::itemPressed, [=](QTableWidgetItem *item) {
            qDebug() << sheetIndex << ". itemPressed : " << item;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
            qDebug() << sheetIndex << ". itemClicked : " << item;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::itemSelectionChanged, [=]() {
            qDebug() << sheetIndex << ". itemSelectionChanged";
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellPressed, [=](auto row, auto column) {  // c++14 > version
            qDebug() << sheetIndex << ". cellPressed : " << row << ", " << column;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellClicked, [=](int row, int column) {
            qDebug() << sheetIndex << ". cellClicked : " << row << ", " << column;
        });
        connect(mTableWidgets[sheetIndex], &QTableWidget::cellEntered, [=](int row, int column) {
            qDebug() << sheetIndex << ". cellEntered : " << row << ", " << column;
        });
#endif
    }

    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

void GuiCenter::updateDisplay(const bool& first, const int& type) {
    if (type == PropertyTypeEnum::PropertyTypeDisplaySize) {
        updateDisplaySize();
    } else if (type == PropertyTypeEnum::PropertyTypeVisible) {
        updateDisplayVisible();
    } else if ((type == PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew)
        || (type == PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen)) {
        updateDisplaySheetInfo(type);
    } else {
    }
}

void GuiCenter::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case PropertyTypeEnum::PropertyTypeDepth : {
            drawDisplay(value);
            break;
        }
        case PropertyTypeEnum::PropertyTypeDisplaySize :
        case PropertyTypeEnum::PropertyTypeVisible :
        case PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew :
        case PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen : {
            updateDisplay(false, type);
            break;
        }
        default : {
            break;
        }
    }
}
