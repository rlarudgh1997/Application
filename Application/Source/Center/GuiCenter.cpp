#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
//#include "CommonUtil.h"


#include <QToolBar>
#include <QTableWidget>

#include <QPushButton>
#include <QLineEdit>


QSharedPointer<GuiCenter> GuiCenter::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiCenter> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiCenter>(new GuiCenter(handler));
    }
    return gGui;
}

GuiCenter::GuiCenter(AbstractHandler* handler) : AbstractGui(handler) {
    mItem = QSharedPointer<QMap<ItemType, QWidget*>>::create(QMap<ItemType, QWidget*>{
                {ItemType::Screen, qobject_cast<QWidget*>(handler->getScreen())},
                {ItemType::MainWindow, qobject_cast<QWidget*>(new QMainWindow())},
                {ItemType::TabeWidget, qobject_cast<QWidget*>(new QTabWidget())},
            });

    qDebug() << "temp :" << Q_CAST(QWidget(), handler->getScreen()) << ", " << Q_CAST(QWidget(), isItem(ItemType::MainWindow));

    int marginHeight = 60;
    QRect rect = isItem(ItemType::Screen)->geometry();
    rect.setY(marginHeight);
    rect.setHeight(rect.height()-marginHeight);
    isItem(ItemType::MainWindow)->setGeometry(rect);
    isItem(ItemType::MainWindow)->setParent(isItem(ItemType::Screen));
//    qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->setCentralWidget(isItem(ItemType::TabeWidget));
    Q_CAST(QMainWindow(), isItem(ItemType::MainWindow))->setCentralWidget(isItem(ItemType::TabeWidget));
    isItem(ItemType::MainWindow)->show();
}

AbstractHandler* GuiCenter::isHandler() {
    return mHandler;
}

QWidget* GuiCenter::isItem(const int& type) {
    if (static_cast<int>(type) < mItem.data()->size()) {
        return mItem.data()->value(static_cast<ItemType>(type));
    }
    qDebug() << "Fail to item is nullptr - type :" << static_cast<int>(type) << ", size :" << mItem.data()->size();
    return nullptr;
}

bool GuiCenter::createSignal(const int& type, const QVariant& value) {
    if (mHandler) {
        emit mHandler->signalHandlerEvent(type, value);
        return true;
    }
    qDebug() << "Fail to create signal - handler is nullptr";
    return false;
}

void GuiCenter::drawDisplay(const int& depth) {
}

void GuiCenter::drawDisplayDepth0() {
    if (mHandler->getProperty(PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->show();
    } else {
        qobject_cast<QMainWindow*>(isItem(ItemType::MainWindow))->centralWidget()->hide();
    }

    QStringList columnTitles = mHandler->getProperty(PropertyTypeEnum::PropertyTypeSheetName).toStringList();
    QStringList seetTitles = mHandler->getProperty(PropertyTypeEnum::PropertyTypeContextName).toStringList();
    int rowCount = 5;
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
    foreach(auto seetTitle, seetTitles) {
        int index = mTableWidgets.size();
        mTableWidgets[index] = new QTableWidget(rowCount, columnTitles.size()-1, isItem(ItemType::Screen));
        mTableWidgets[index]->setHorizontalHeaderLabels(columnTitles);
        qobject_cast<QTabWidget*>(isItem(ItemType::TabeWidget))->addTab(mTableWidgets[index], seetTitle);

        mTableWidgets[index]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

        connect(mTableWidgets[index], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            qDebug() << "customContextMenuRequested : " << pos;
        });
        connect(mTableWidgets[index], &QTableWidget::itemPressed, [=](QTableWidgetItem *item) {
            qDebug() << "itemPressed : " << item;
        });
        connect(mTableWidgets[index], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
            qDebug() << "itemClicked : " << item;
        });
        connect(mTableWidgets[index], &QTableWidget::currentItemChanged,
                                    [=](QTableWidgetItem *current, QTableWidgetItem *previous) {
            qDebug() << "currentItemChanged : " << previous << " -> " << current;
        });
        connect(mTableWidgets[index], &QTableWidget::itemSelectionChanged, [=]() {
            qDebug() << "itemSelectionChanged";
        });
        connect(mTableWidgets[index], &QTableWidget::cellPressed, [=](auto row, auto column) {  // c++14 > version
            qDebug() << "cellPressed : " << row << ", " << column;
        });
        connect(mTableWidgets[index], &QTableWidget::cellClicked, [=](int row, int column) {
            qDebug() << "cellClicked : " << row << ", " << column;
        });

        for (int row = 0; row < rowCount; row++) {
            for (int column = 0; column < columnTitles.size(); column++) {
                QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1[%2, %3]").arg(seetTitle).arg(row+1).arg(column+1));
                newItem->setTextAlignment(Qt::AlignCenter);
                mTableWidgets[index]->setItem(row, column, newItem);
            }
        }
    }
}

void GuiCenter::drawDisplayDepth1() {
}

void GuiCenter::drawDisplayDepth2() {
}

void GuiCenter::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case PropertyTypeEnum::PropertyTypeVisible : {
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


