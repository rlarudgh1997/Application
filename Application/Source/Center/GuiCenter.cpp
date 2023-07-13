#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"


// #include <QToolBar>
// #include <QPushButton>
// #include <QLineEdit>
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>



QSharedPointer<GuiCenter> GuiCenter::instance(AbstractHandler* handler) {
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

    int marginHeight = 60;
    QRect rect = isItem(ItemType::Widget)->geometry();
    rect.setY(marginHeight);
    rect.setHeight(rect.height()-marginHeight);
    isItem(ItemType::MainWindow)->setGeometry(rect);
    isItem(ItemType::MainWindow)->setParent(isItem(ItemType::Widget));
//    qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->setCentralWidget(isItem(ItemType::TabeWidget));
    Q_CAST(QMainWindow(), isItem(ItemType::MainWindow))->setCentralWidget(isItem(ItemType::TabeWidget));
    isItem(ItemType::MainWindow)->show();
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
    updateDisplay(true);

    QStringList sheetTitles = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeSheetName).toStringList();
    QStringList columnTitles = isHandler()->getProperty(PropertyTypeEnum::PropertyTypeContentTitle).toStringList();
    int rowCount = 5;
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
    foreach(auto sheetTitle, sheetTitles) {
        int currentSheet = mTableWidgets.size();
        mTableWidgets[currentSheet] = new QTableWidget(rowCount, columnTitles.size()-1, isItem(ItemType::Widget));
        mTableWidgets[currentSheet]->setObjectName(QString("tableWidget_%1").arg(currentSheet));
        qDebug() << "objectName :" << mTableWidgets[currentSheet]->objectName();
        mTableWidgets[currentSheet]->setHorizontalHeaderLabels(columnTitles);
        qobject_cast<QTabWidget*>(isItem(ItemType::TabeWidget))->addTab(mTableWidgets[currentSheet], sheetTitle);

        mTableWidgets[currentSheet]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

        connect(mTableWidgets[currentSheet], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            QModelIndexList modelIndexs = mTableWidgets[currentSheet]->selectionModel()->selectedIndexes();
            qDebug() << currentSheet << ". customContextMenuRequested : " << pos;
            qDebug() << "[Info]\n" << modelIndexs << "\nSize :" << modelIndexs.size();

            static bool span = false;
            if (modelIndexs.size() >= 2) {
                if (span) {
                    span = false;
                    mTableWidgets[currentSheet]->clearSpans();
                } else {
                    span = true;
                    std::sort(modelIndexs.begin(), modelIndexs.end());
                    auto row = modelIndexs.at(currentSheet).row();
                    auto col = modelIndexs.at(currentSheet).column();
                    auto rowSpan = modelIndexs.last().row() - row + 1;
                    auto colSpan = modelIndexs.last().column() - col + 1;
                    qDebug() << "Info :" << row << "," << col << "," << rowSpan << "," << colSpan;
                    mTableWidgets[currentSheet]->setSpan(row, col, rowSpan, colSpan);
                }
            }
        });
//        connect(mTableWidgets[currentSheet], &QTableWidget::itemPressed, [=](QTableWidgetItem *item) {
//            qDebug() << currentSheet << ". itemPressed : " << item;
//        });
//        connect(mTableWidgets[currentSheet], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
//            qDebug() << currentSheet << ". itemClicked : " << item;
//        });
//        connect(mTableWidgets[currentSheet], &QTableWidget::currentItemChanged,
//                                    [=](QTableWidgetItem *current, QTableWidgetItem *previous) {
//            qDebug() << currentSheet << ". currentItemChanged : " << previous << " -> " << current;
//        });
//        connect(mTableWidgets[currentSheet], &QTableWidget::itemSelectionChanged, [=]() {
//            qDebug() << currentSheet << ". itemSelectionChanged";
//        });
//        connect(mTableWidgets[currentSheet], &QTableWidget::cellPressed, [=](auto row, auto column) {  // c++14 > version
//            qDebug() << currentSheet << ". cellPressed : " << row << ", " << column;
//        });
//        connect(mTableWidgets[currentSheet], &QTableWidget::cellClicked, [=](int row, int column) {
//            qDebug() << currentSheet << ". cellClicked : " << row << ", " << column;
//        });
//        connect(mTableWidgets[currentSheet], &QTableWidget::cellEntered, [=](int row, int column) {
//            qDebug() << currentSheet << ". cellEntered : " << row << ", " << column;
//        });
        connect(mTableWidgets[currentSheet], &QTableWidget::cellDoubleClicked, [=](int row, int column) {
            qDebug() << currentSheet << ". cellDoubleClicked : " << row << ", " << column;
        });


        for (int row = 0; row < rowCount; row++) {
            for (int column = 0; column < columnTitles.size(); column++) {
                QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1[%2, %3]").arg(sheetTitle).arg(row+1).arg(column+1));
                newItem->setTextAlignment(Qt::AlignCenter);
                mTableWidgets[currentSheet]->setItem(row, column, newItem);
            }
        }
    }
}

void GuiCenter::drawDisplayDepth1() {
    qDebug() << "GuiCenter::drawDisplayDepth1()";
}

void GuiCenter::drawDisplayDepth2() {
    qDebug() << "GuiCenter::drawDisplayDepth2()";
}

void GuiCenter::updateDisplay(const bool& first, const int& type) {
    if (first) {
    } else {
    }

    if (isHandler()->getProperty(PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->show();
    } else {
        qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->hide();
    }
}

void GuiCenter::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case PropertyTypeEnum::PropertyTypeVisible : {
            updateDisplay(false, type);
            break;
        }
        case PropertyTypeEnum::PropertyTypeDepth : {
            if (value == QVariant(ScreenEnum::DisplayDepthDepth0)) {
                drawDisplayDepth0();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth1)) {
                drawDisplayDepth1();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth1)) {
                drawDisplayDepth1();
            } else {
            }
            break;
        }
        default : {
            break;
        }
    }
}
