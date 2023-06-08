#include "HandlerMain.h"
#include "CommonEnum.h"


QSharedPointer<HandlerMain> HandlerMain::instance() {
    static QSharedPointer<HandlerMain> gHandler;

    if (gHandler.isNull()) {
        gHandler = QSharedPointer<HandlerMain>(new HandlerMain());
    }

    return gHandler;
}

HandlerMain::HandlerMain() : AbstractHandler(ScreenEnum::DisplayTypeMain, QString("HandlerMain"), true) {
}

void HandlerMain::timerFunc(const int& timerId) {
    Q_UNUSED(timerId)
    if (timerId == getTimerId(HandlerMainTimerStart)) {
        // do nothing
    }
}

void HandlerMain::initPropertyInfo() {
    registerProperty(PropertyTypeEnum::PropertyTypeDisplay,                   QVariant(0));
    registerProperty(PropertyTypeEnum::PropertyTypeMode,                      QVariant(0));
}


#include <QPushButton>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableWidget>

void HandlerMain::drawSubItem() {
    QMainWindow* main = new QMainWindow();

    main->setGeometry(getComponent()->geometry());
    main->setParent(getComponent());


    QMenu* menu = main->menuBar()->addMenu(QString("File"));
    QToolBar* toolBar = main->addToolBar(QString("File"));

    QAction *actionNew = new QAction(QIcon::fromTheme("actionNew", QIcon(":/images/new.png")), QString("New"), this);
    if (actionNew) {
        actionNew->setShortcuts(QKeySequence::New);
        actionNew->setStatusTip(QString("Create a new file"));

        // connect(actionNew, &QAction::triggered, this, &MainWindow::newFile);

        menu->addAction(actionNew);
        toolBar->addAction(actionNew);
    }


    main->show();


    QTabWidget* tabWidget = new QTabWidget();
    main->setCentralWidget(tabWidget);

    QStringList columnTitles = QStringList();
    columnTitles.push_back("TCName");
    columnTitles.push_back("VehicleType");
    columnTitles.push_back("Result");
    columnTitles.push_back("Case");
    columnTitles.push_back("Input_Signal");
    columnTitles.push_back("Input_Data");
    columnTitles.push_back("Output_Signal");
    columnTitles.push_back("isInitialize");
    columnTitles.push_back("Output_Value");
    columnTitles.push_back("Config_Signal");
    columnTitles.push_back("Data");
    columnTitles.push_back("Negative");
    columnTitles.push_back("Test");

    QStringList seetTitles = QStringList();
    seetTitles.push_back("Privates");
    seetTitles.push_back("Telltales");
    seetTitles.push_back("Constants");
    seetTitles.push_back("Events");
    seetTitles.push_back("Sounds");
    seetTitles.push_back("Inters");
    seetTitles.push_back("Outputs");

    int rowCount = 5;
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
    foreach(auto seetTitle, seetTitles) {
        int index = mTableWidgets.size();
        mTableWidgets[index] = new QTableWidget(rowCount, columnTitles.size()-1, getComponent());
        mTableWidgets[index]->setHorizontalHeaderLabels(columnTitles);
        tabWidget->addTab(mTableWidgets[index], seetTitle);

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

        qDebug() << "Table[" << seetTitle << "] : " << mTableWidgets[index]->rowCount()
                << ", " << mTableWidgets[index]->columnCount();

        for (int row = 0; row < rowCount; row++) {
            for (int column = 0; column < columnTitles.size(); column++) {
                QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1[%2, %3]").arg(seetTitle).arg(row+1).arg(column+1));
                newItem->setTextAlignment(Qt::AlignCenter);
                mTableWidgets[index]->setItem(row, column, newItem);
            }
        }
    }





    QPushButton* button = new QPushButton(getComponent());
    button->setGeometry(300, 300, 300, 100);
    button->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
    button->setText("Button 2");
    button->show();
    connect(button, &QPushButton::clicked, [=]() {
        qDebug() << "clicked";
    });

    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}

