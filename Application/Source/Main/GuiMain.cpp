#include "GuiMain.h"
#include "CommonEnum.h"


#if defined(USE_GUI_INSTANCE_SINGTONE)
QSharedPointer<GuiMain> GuiMain::instance(QWidget* parent) {
    static QSharedPointer<GuiMain> gGui;

    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiMain>(new GuiMain(parent));
    }

    return gGui;
}
#else
GuiMain::GuiMain(QWidget* parent) : mGuiScreen(parent) {
     qDebug() << "GuiMain::GuiMain :" << mGuiScreen;
}

GuiMain::~GuiMain() {
    delete mGuiScreen;
}
#endif

void GuiMain::updateGuiScreen(QWidget* parent) {
    mGuiScreen = parent;
}

void GuiMain::updateGui(const int& dataType, const QVariant& value) {
    qDebug() << "GuiMain::updateGui :" << dataType << ", " << value;

    switch (dataType) {
        case PropertyTypeEnum::PropertyTypeDepth : {
            if (value.toInt() == ScreenEnum::DisplayDepthMain) {
                drawDisplayMain();
            }
            break;
        }
        default : {
            break;
        }
    }
}

 #include <QMainWindow>
 #include <QMenu>
 #include <QMenuBar>
 #include <QToolBar>
 #include <QTableWidget>
 #include <QPushButton>

void GuiMain::drawDisplayMain() {
#if 1
    QMainWindow* main = new QMainWindow();

    main->setGeometry(mGuiScreen->geometry());
    main->setParent(mGuiScreen);


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
        mTableWidgets[index] = new QTableWidget(rowCount, columnTitles.size()-1, mGuiScreen);
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





    QPushButton* button = new QPushButton(mGuiScreen);
    button->setGeometry(300, 300, 300, 100);
    button->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
    button->setText("Button 2");
    button->show();
    connect(button, &QPushButton::clicked, [=]() {
        qDebug() << "clicked";
    });
    button->setStyleSheet("color: rgb(50, 50, 100)");

    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
#endif
}
