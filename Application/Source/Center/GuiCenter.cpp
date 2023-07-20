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

void GuiCenter::updateDisplaySize() {
    QRect rect = isItem(ItemType::Widget)->geometry();
    QSize size = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    rect.setY(mMarginPosY);
    rect.setWidth(size.width());
    rect.setHeight(size.height()-mMarginPosY);
    isItem(ItemType::MainWindow)->setGeometry(rect);
}

void GuiCenter::updateDisplayVisible() {
    if (isHandler()->getProperty(PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        isItem(ItemType::MainWindow)->show();
        // qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->show();
    } else {
        isItem(ItemType::MainWindow)->hide();
        // qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->hide();
    }
}

void GuiCenter::updateDisplaySheetInfo() {
    int updateSheetInfo = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeUpdateSheetInfo).toInt();
    qDebug() << "==============================================================================================";

    if (updateSheetInfo == 0) {
        qDebug() << "Fail to update sheet info size : 0";
        return;
    } else {
        qDebug() << "UpdateSheetSize :" << updateSheetInfo;
    }

    // Clear - Previous Table Widget
    qobject_cast<QTabWidget*>(isItem(ItemType::TabeWidget))->clear();
    mTableWidgets.clear();

    QString sheetName = QString();
    QStringList contentTitle = QStringList();

    for (int index = 0; index < updateSheetInfo; index++) {
        int sheetIndex = PropertyTypeEnum::PropertyTypeDetailInfoDescription + index;
        // qDebug() << "Info[" << sheetIndex << "] =" << isHandler()->getProperty(sheetIndex) << "\n\n";
        QVariantList detailInfo = isHandler()->getProperty(sheetIndex).toList();

        // Constructor - Detail List Info
        QString sheetName = detailInfo[ListInfoEnum::ListInfoExcel::Sheet].toString();
        QStringList contentTitle = detailInfo[ListInfoEnum::ListInfoExcel::Title].toStringList();
        QVariantList count = detailInfo[ListInfoEnum::ListInfoExcel::Count].toList();

        if (count.size() < 2) {
            qDebug() << "Fail to detail info data size :" << count.size();
            return;
        }

        int rowCount = count.at(0).toInt();
        int columnCount = count.at(1).toInt();

        // Draw - Sheet/Title
        mTableWidgets[sheetIndex] = new QTableWidget(rowCount, columnCount, isItem(ItemType::Widget));
        mTableWidgets[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        mTableWidgets[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        qobject_cast<QTabWidget*>(isItem(ItemType::TabeWidget))->addTab(mTableWidgets[sheetIndex], sheetName);

        // Draw - Detail List Data
        for (int dataRowIndex = 0; dataRowIndex < rowCount; dataRowIndex++) {
            QStringList detailInfoData = detailInfo[ListInfoEnum::ListInfoExcel::Data + dataRowIndex].toStringList();
            qDebug() << "Data[" << dataRowIndex << "] :" << detailInfoData;

            int dataColumnIndex = 0;
            foreach(const auto& data, detailInfoData) {
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
}

void GuiCenter::updateDisplay(const bool& first, const int& type) {
    if (type == PropertyTypeEnum::PropertyTypeDisplaySize) {
        updateDisplaySize();
    } else if (type == PropertyTypeEnum::PropertyTypeVisible) {
        updateDisplayVisible();
    } else if (type == PropertyTypeEnum::PropertyTypeUpdateSheetInfo) {
        updateDisplaySheetInfo();
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
