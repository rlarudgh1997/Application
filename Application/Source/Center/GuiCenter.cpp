#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"



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
    qDebug() << "==============================================================================================";
    int updateSheetInfo = isHandler()->getProperty(type).toInt();

    if (updateSheetInfo == 0) {
        qDebug() << "Fail to update sheet info size : 0, type :" << type;
        return;
    }
    qDebug() << "UpdateSheetSize :" << updateSheetInfo;

    // Clear - Previous Table Widget
    mTabWidget->clear();
    mTableWidgets.clear();

    QString sheetName = QString();
    QStringList contentTitle = QStringList();
    QMap<int, QList<CellInfo>> mCellInfo = QMap<int, QList<CellInfo>>();

    for (int index = 0; index < updateSheetInfo; index++) {
        int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription + index;
        QVariantList detailInfo = isHandler()->getProperty(sheetIndex).toList();

        // qDebug() << "Info[" << sheetIndex << "] =" << detailInfo << "\n\n";

        // Constructor - Detail List Info
        QString sheetName = detailInfo[ListInfoEnum::ListInfoExcel::Sheet].toString();
        QStringList contentTitle = detailInfo[ListInfoEnum::ListInfoExcel::Title].toStringList();
        QVariantList count = detailInfo[ListInfoEnum::ListInfoExcel::Count].toList();

        if (count.size() != 2) {
            if ((count.at(0).type() != QVariant::Type::Int) || (count.at(1).type() != QVariant::Type::Int)) {
                qDebug() << "Fail to detail info data type(int) error";
            } else {
                qDebug() << "Fail to detail info data size :" << count.size();
            }
            return;
        }


        // Draw - Sheet/Title
        int rowCount = count.at(0).toInt();
        int columnCount = count.at(1).toInt();
        mTableWidgets[sheetIndex] = new QTableWidget(rowCount, columnCount);
        mTableWidgets[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        mTableWidgets[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mTabWidget->addTab(mTableWidgets[sheetIndex], sheetName);
        qDebug() << "Count :" << rowCount << "," << columnCount;

        // Draw - Detail List Data
        for (int dataRowIndex = 0; dataRowIndex < rowCount; dataRowIndex++) {
            QStringList detailInfoData = detailInfo[ListInfoEnum::ListInfoExcel::Data + dataRowIndex].toStringList();
            int dataColumnIndex = 0;
            foreach(const auto& data, detailInfoData) {
                if ((type == PropertyTypeEnum::PropertyTypeUpdateSheetInfoOpen) && (dataColumnIndex < 4)) {
                    if (data.compare("") == false) {
                        mCellInfo[sheetIndex].append(CellInfo(dataRowIndex, dataColumnIndex));
                    }
                }

                QTableWidgetItem *detailDataItem = new QTableWidgetItem(data);
                detailDataItem->setTextAlignment(Qt::AlignCenter);
                mTableWidgets[sheetIndex]->setItem(dataRowIndex, dataColumnIndex, detailDataItem);
                dataColumnIndex++;
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
        // connect(mTableWidgets[sheetIndex], &QTableWidget::currentItemChanged,
        //                                     [=](QTableWidgetItem *current, QTableWidgetItem *previous) {
        //     qDebug() << sheetIndex << ". currentItemChanged : " << previous << " -> " << current;
        // });
        // connect(mTableWidgets[sheetIndex], &QTableWidget::itemPressed, [=](QTableWidgetItem *item) {
        //     qDebug() << sheetIndex << ". itemPressed : " << item;
        // });
        // connect(mTableWidgets[sheetIndex], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
        //     qDebug() << sheetIndex << ". itemClicked : " << item;
        // });
        // connect(mTableWidgets[sheetIndex], &QTableWidget::itemSelectionChanged, [=]() {
        //     qDebug() << sheetIndex << ". itemSelectionChanged";
        // });
        // connect(mTableWidgets[sheetIndex], &QTableWidget::cellPressed, [=](auto row, auto column) {  // c++14 > version
        //     qDebug() << sheetIndex << ". cellPressed : " << row << ", " << column;
        // });
        // connect(mTableWidgets[sheetIndex], &QTableWidget::cellClicked, [=](int row, int column) {
        //     qDebug() << sheetIndex << ". cellClicked : " << row << ", " << column;
        // });
        // connect(mTableWidgets[sheetIndex], &QTableWidget::cellEntered, [=](int row, int column) {
        //     qDebug() << sheetIndex << ". cellEntered : " << row << ", " << column;
        // });
#endif
    }

    if (type == PropertyTypeEnum::PropertyTypeUpdateSheetInfoNew) {
        return;
    }
        return;
    qDebug() << "\n\n\n";
    qDebug() << "=======================================================================";
    qDebug() << "Draw - Cell Merge/Split\n\n";

    QMapIterator<int, QList<CellInfo>> iter(mCellInfo);
    int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    while (iter.hasNext()) {
        iter.next();
        // qDebug() << "DetailInfo :" << iter.key() << "," << iter.value() << "\n\n";
        QMap<int, QList<int>> mergeInfo = QMap<int, QList<int>>();
        QMultiMap<int, int> mergeInfo1 = QMultiMap<int, int>();

        foreach(const auto& info, iter.value()) {
//            qDebug() << "CellInfo[" << iter.key() << "] :" << info.isCellInfoColumn() << "," << info.isCellInfoRow();
            mergeInfo[info.isCellInfoColumn()].append(info.isCellInfoRow());
            mergeInfo1.insert(info.isCellInfoColumn(), info.isCellInfoRow());
//            qDebug() << "\t MergeInfo[" << info.isCellInfoColumn() << "] :" << mergeInfo[info.isCellInfoColumn()];
        }

//        foreach(const auto& merge, mergeInfo) {
//            qDebug()  << "\t\t merge[" << iter.key() << "] :" << merge;
//        }


        QMap<int, int>::iterator iter1;
        QMap<int, QPair<int, int>> mergerIndexInfo;

        int min = 100;
        int max = 0;
        for (iter1 = mergeInfo1.begin(); iter1 != mergeInfo1.end(); ++iter1) {
            sheetIndex = iter.key();
            // qDebug() << "\t merge[" << iter.key() << "] :" << iter1.key() << "," << iter1.value();

            min = qMin(min, iter1.value());
            max = qMax(max, iter1.value());
            qDebug() << "\t Range[" << iter.key() << "] :" << min << ", Max :" << max;
            mergerIndexInfo[iter1.key()] = QPair<int, int>(min, max);

            // qDebug() << "\t\t\t mergerIndexInfo :" << mergerIndexInfo;

            if (sheetIndex == 3003) {
                QModelIndexList modelIndexs = mTableWidgets[sheetIndex]->selectionModel()->selectedIndexes();
                // std::sort(modelIndexs.begin(), modelIndexs.end());
                mTableWidgets[sheetIndex]->setSpan(0, 0, 7, 1);
                qDebug() << "\t\t 1. Telltales - TCName";
                mTableWidgets[sheetIndex]->setSpan(0, 1, 7, 1);
                qDebug() << "\t\t 2. Telltales - VehicleType";
                mTableWidgets[sheetIndex]->setSpan(0, 2, 3, 1);
                mTableWidgets[sheetIndex]->setSpan(3, 2, 3, 1);
                qDebug() << "\t\t 3. Telltales - Result";
                mTableWidgets[sheetIndex]->setSpan(1, 3, 2, 1);
                mTableWidgets[sheetIndex]->setSpan(3, 3, 3, 1);
                qDebug() << "\t\t 4. Telltales - Case";
            } else if (sheetIndex == 3004) {
                mTableWidgets[sheetIndex]->setSpan(0, 0, 3, 1);
                mTableWidgets[sheetIndex]->setSpan(3, 0, 6, 1);
                qDebug() << "\t\t 1. Constants - TCName";
                mTableWidgets[sheetIndex]->setSpan(0, 1, 3, 1);
                mTableWidgets[sheetIndex]->setSpan(3, 1, 6, 1);
                qDebug() << "\t\t 2. Constants - VehicleType";
                mTableWidgets[sheetIndex]->setSpan(0, 2, 2, 1);
                mTableWidgets[sheetIndex]->setSpan(3, 2, 2, 1);
                mTableWidgets[sheetIndex]->setSpan(5, 2, 3, 1);
                qDebug() << "\t\t 3. Constants - Result";
                mTableWidgets[sheetIndex]->setSpan(0, 3, 2, 1);
                mTableWidgets[sheetIndex]->setSpan(3, 3, 2, 1);
                mTableWidgets[sheetIndex]->setSpan(5, 3, 3, 1);
                qDebug() << "\t\t 4. Constants - Case";
            } else if (sheetIndex == 3005) {
                mTableWidgets[sheetIndex]->setSpan(0, 0, 15, 1);
                qDebug() << "\t\t 1. Events - TCName";
                mTableWidgets[sheetIndex]->setSpan(0, 1, 15, 1);
                qDebug() << "\t\t 2. Events - VehicleType";
                mTableWidgets[sheetIndex]->setSpan(0, 2, 5, 1);
                mTableWidgets[sheetIndex]->setSpan(5, 2, 5, 1);
                mTableWidgets[sheetIndex]->setSpan(10, 2, 5, 1);
                qDebug() << "\t\t 3. Events - Result";
                mTableWidgets[sheetIndex]->setSpan(0, 3, 5, 1);
                mTableWidgets[sheetIndex]->setSpan(5, 3, 5, 1);
                mTableWidgets[sheetIndex]->setSpan(10, 3, 5, 1);
                qDebug() << "\t\t 4. Events - Case";
            } else {
            }
        }
        qDebug() << "-----------------------------------------------------------------------------\n\n";
    }

#if 0
    QMultiMap<int, QString> myMaps;

    myMaps.insert(1, "Autodesk Maya 2014");
    myMaps.insert(1, "Autodesk Inventor 2014");
    myMaps.insert(1, "Autodesk Alias 2014");
    myMaps.insert(1, "Autodesk 3ds Max 2014");
    myMaps.insert(1, "Autodesk Softimage 2014");
    myMaps.insert(2, "Adobe Photoshop CS6");
    myMaps.insert(2, "Adobe Illustrator CS6");
    myMaps.insert(2, "Adobe InDesign CS6");
    myMaps.insert(1, "Autodesk AutoCAD 2014");

    QMap<int, QString>::iterator it;
    QString output;

    for (it = myMaps.begin(); it != myMaps.end(); ++it) {
        // Format output here.
        qDebug() << "Value :" << QString("%1 : %2").arg(it.key()).arg(it.value());
    }
#endif
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
