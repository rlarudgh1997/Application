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

GuiCenter::GuiCenter(AbstractHandler* handler) : mHandler(handler), mScreen(handler->getScreen()) {
    mMainWindow = new QMainWindow();
    QRect rect = mScreen->geometry();
    rect.setY(60);
    mMainWindow->setGeometry(rect);
    mMainWindow->setParent(mScreen);
    mTabWidget = new QTabWidget();
    mMainWindow->setCentralWidget(mTabWidget);
    mMainWindow->show();
}

bool GuiCenter::createSignal(const int& type, const QVariant& value) {
    if (mHandler) {
        emit mHandler->signalHandlerEvent(type, value);
        return true;
    }

    qDebug() << "Fail to create signal - handler :" << mHandler;
    return false;
}

void GuiCenter::drawDisplayDepth0() {
    mMainWindow->centralWidget()->show();

    QStringList columnTitles = mHandler->getProperty(PropertyTypeEnum::PropertyTypeSheetName).toStringList();
    QStringList seetTitles = mHandler->getProperty(PropertyTypeEnum::PropertyTypeContextName).toStringList();
    int rowCount = 5;
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
    foreach(auto seetTitle, seetTitles) {
        int index = mTableWidgets.size();
        mTableWidgets[index] = new QTableWidget(rowCount, columnTitles.size()-1, mScreen);
        mTableWidgets[index]->setHorizontalHeaderLabels(columnTitles);
        mTabWidget->addTab(mTableWidgets[index], seetTitle);

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


