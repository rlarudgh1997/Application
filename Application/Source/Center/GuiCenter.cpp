#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"


// #include <QToolBar>
// #include <QPushButton>
// #include <QLineEdit>
#include <QMainWindow>



QSharedPointer<GuiCenter>& GuiCenter::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiCenter> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiCenter>(new GuiCenter(handler));
    }
    return gGui;
}

GuiCenter::GuiCenter(AbstractHandler* handler) : AbstractGui(handler) {
    initItem();
}

void GuiCenter::initItem() {
    mItem = QSharedPointer<QMap<ItemType, QWidget*>>::create(QMap<ItemType, QWidget*>{
                {ItemType::Widget, qobject_cast<QWidget*>(isHandler()->getScreen())},
                {ItemType::MainWindow, qobject_cast<QWidget*>(new QMainWindow())},
                {ItemType::TabeWidget, qobject_cast<QWidget*>(new QTabWidget())},
            });

    qDebug() << "Item :" << Q_CAST(QWidget(), isHandler()->getScreen())
                << ", " << Q_CAST(QWidget(), isItem(ItemType::MainWindow));

    QRect rect = isItem(ItemType::Widget)->geometry();
    rect.setY(mMarginPosY);
    rect.setHeight(rect.height()-mMarginPosY);
    isItem(ItemType::MainWindow)->setGeometry(rect);
    isItem(ItemType::MainWindow)->setParent(isItem(ItemType::Widget));
    Q_CAST(QMainWindow(), isItem(ItemType::MainWindow))->setCentralWidget(isItem(ItemType::TabeWidget));
    // qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->setCentralWidget(isItem(ItemType::TabeWidget));
    // isItem(ItemType::MainWindow)->show();
}

QWidget* GuiCenter::isItem(const int& type) {
    if (static_cast<int>(type) < mItem.data()->size()) {
        return mItem.data()->value(static_cast<ItemType>(type));
    }
    qDebug() << "Fail to item is nullptr - type :" << static_cast<int>(type) << ", size :" << mItem.data()->size();
    return nullptr;
}

bool GuiCenter::createSignal(const int& type, const QVariant& value) {
    if (isHandler()) {
        emit isHandler()->signalHandlerEvent(type, value);
        return true;
    }
    qDebug() << "Fail to create signal - handler is nullptr";
    return false;
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

void GuiCenter::updateDisplay(const bool& first, const int& type) {
    if (type == PropertyTypeEnum::PropertyTypeDisplaySize) {
        QRect rect = isItem(ItemType::Widget)->geometry();
        QSize size = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
        rect.setY(mMarginPosY);
        rect.setWidth(size.width());
        rect.setHeight(size.height()-mMarginPosY);
        isItem(ItemType::MainWindow)->setGeometry(rect);
    } else if (type == PropertyTypeEnum::PropertyTypeVisible) {
        if (isHandler()->getProperty(PropertyTypeEnum::PropertyTypeVisible).toBool()) {
            isItem(ItemType::MainWindow)->show();
            // qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->show();
        } else {
            isItem(ItemType::MainWindow)->hide();
            // qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->hide();
        }
    } else if (type == PropertyTypeEnum::PropertyTypeUpdateSheetInfo) {
        QStringList sheetName = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeSheetName).toStringList();
        QStringList contentTitle = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeContentTitle).toStringList();
        QStringList sheetInfo = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeUpdateSheetInfo).toStringList();

        #define SINGLE_MAP 1
#if 0
        qDebug() << "==============================================================================================";
        qDebug() << "sheetName :" << sheetName << ", " << sheetName.size();
        foreach(auto title, contentTitle) {
            qDebug() << "title :" << title;
        }
        foreach(auto sheet, sheetInfo) {
            qDebug() << "sheet :" << sheet;
        }
        qDebug() << "==============================================================================================";
#endif
        qobject_cast<QTabWidget*>(isItem(ItemType::TabeWidget))->clear();
        mTableWidgets.clear();

        foreach(const auto& sheetTitle, sheetName) {
            int sheetIndex = mTableWidgets.size();
            // Draw - Sheet Table (Description, Privates, Telltales, Constants, Events, Sounds, Inters, Outputs)
            mTableWidgets[sheetIndex] = new QTableWidget(sheetInfo.size(), contentTitle.size(), isItem(ItemType::Widget));
            // Draw - ContentTitle
            //          - [TCName, VehicleType, Result, Case, Input_Signal, Input_Data, Output_Signal,
            //          -  isInitialize, Output_Value, Config_Signal, Data, Negative, Test]
            mTableWidgets[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
            mTableWidgets[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
            qobject_cast<QTabWidget*>(isItem(ItemType::TabeWidget))->addTab(mTableWidgets[sheetIndex], sheetTitle);

            // Draw - Sheet Data
            int rowIndex = 0;
#if SINGLE_MAP
            QMap<int, CellInfo> mCellInfo = QMap<int, CellInfo>();
#else
            QMap<int, QMap<int, CellInfo>> mCellInfo = QMap<int, QMap<int, CellInfo>>();
#endif
            foreach(const auto& infoData, sheetInfo) {
                QStringList rowInfo = infoData.split("\t");
                int columnIndex = 0;
                // qDebug() << " rowInfo[" << rowIndex << "] :" << rowInfo;

                foreach(const auto& columnInfo, rowInfo) {
                    // qDebug() << "\t columnInfo[" << rowIndex << ", " << columnIndex << "] :" << columnInfo;
                    QTableWidgetItem *itemData = new QTableWidgetItem(columnInfo);
                    itemData->setTextAlignment(Qt::AlignCenter);
                    mTableWidgets[sheetIndex]->setItem(rowIndex, columnIndex, itemData);
                    if (columnInfo.compare("NULL") == false) {
#if SINGLE_MAP
                        mCellInfo[mCellInfo.size()] = CellInfo(rowIndex, columnIndex);
#else
                        if (mCellInfo.size() == 0) {
                            mCellInfo[mCellInfo.size()].insert
                        } else {
                            if ((cellInfo.isCellInfoRow() == rowIndex) || (cellInfo.isCellInfoRow() == columnIndex)) {

                            }
                        }
#endif
                        // qDebug() << "\t columnInfo[" << rowIndex << ", " << columnIndex << "] :" << columnInfo;
                    }
                    columnIndex++;
                }
                rowIndex++;
            }

            // Draw - Cell Merage/Split
#if SINGLE_MAP
            if (mCellInfo.size() > 1) {
                foreach(const auto& cellInfo, mCellInfo) {
                    qDebug() << "isNull = [" << cellInfo.isCellInfoRow() << "," << cellInfo.isCellInfoColumn() << "]";
                }
                auto row = mCellInfo[0].isCellInfoRow();
                auto col = mCellInfo[0].isCellInfoColumn()-1;
                auto rowSpan = mCellInfo[mCellInfo.size()-1].isCellInfoRow() - row + 1;
                auto colSpan = mCellInfo[mCellInfo.size()-1].isCellInfoColumn() - col + 1;
                qDebug() << "Info :" << row << "," << col << "," << rowSpan << "," << colSpan;
                // mTableWidgets[sheetIndex]->setSpan(row, col, rowSpan, colSpan);
            }
#else
            qDebug() << "Size :" << mCellInfo.size();
#endif

            // Draw - Cell Text Width/Height
            mTableWidgets[sheetIndex]->resizeColumnsToContents();
            mTableWidgets[sheetIndex]->resizeRowsToContents();

            // Connect - Signal
            connect(mTableWidgets[sheetIndex], &QTableWidget::cellDoubleClicked, [=](int row, int column) {
                qDebug() << sheetIndex << ". cellDoubleClicked : " << row << ", " << column;
            });
            connect(mTableWidgets[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
                QModelIndexList modelIndexs = mTableWidgets[sheetIndex]->selectionModel()->selectedIndexes();
                qDebug() << sheetIndex << ". customContextMenuRequested : " << pos;
                qDebug() << "[Info]\n" << modelIndexs << "\nSize :" << modelIndexs.size();

                static bool span = false;
                if (modelIndexs.size() >= 2) {
                    if (span) {
                        span = false;
                        mTableWidgets[sheetIndex]->clearSpans();
                    } else {
                        span = true;
                        std::sort(modelIndexs.begin(), modelIndexs.end());
                        auto row = modelIndexs.at(sheetIndex).row();
                        auto col = modelIndexs.at(sheetIndex).column();
                        auto rowSpan = modelIndexs.last().row() - row + 1;
                        auto colSpan = modelIndexs.last().column() - col + 1;
                        qDebug() << "Info :" << row << "," << col << "," << rowSpan << "," << colSpan;
                        mTableWidgets[sheetIndex]->setSpan(row, col, rowSpan, colSpan);
                    }
                }
            });
            // connect(mTableWidgets[sheetIndex], &QTableWidget::itemPressed, [=](QTableWidgetItem *item) {
            //     qDebug() << sheetIndex << ". itemPressed : " << item;
            // });
            // connect(mTableWidgets[sheetIndex], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
            //     qDebug() << sheetIndex << ". itemClicked : " << item;
            // });
            // connect(mTableWidgets[sheetIndex], &QTableWidget::currentItemChanged,
            //                                     [=](QTableWidgetItem *current, QTableWidgetItem *previous) {
            //     qDebug() << sheetIndex << ". currentItemChanged : " << previous << " -> " << current;
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
        }
    } else {
    }
}

void GuiCenter::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case PropertyTypeEnum::PropertyTypeDisplaySize :
        case PropertyTypeEnum::PropertyTypeVisible :
        case PropertyTypeEnum::PropertyTypeUpdateSheetInfo : {
            updateDisplay(false, type);
            break;
        }
        case PropertyTypeEnum::PropertyTypeDepth : {
            if (value == QVariant(ScreenEnum::DisplayDepthDepth0)) {
                drawDisplayDepth0();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth1)) {
                drawDisplayDepth1();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth2)) {
                drawDisplayDepth2();
            } else {
            }
            break;
        }
        default : {
            break;
        }
    }
}
