#include "MainWindow.h"
#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlManager.h"

#include <QApplication>


// Q_LOGGING_CATEGORY(MAINWINDOW, "MainWindow")

// ivis::common::CheckLib mCheckLib = ivis::common::CheckLib(QStringList({"openpyxl", "pandas"}));
MainWindow::MainWindow() {
    qDebug() << "================================================================================================";
    qDebug() << "- Application - Path :" << QApplication::applicationDirPath().toLatin1().data();
    qDebug() << "- QT_VERSION :" << QT_VERSION_STR << "\n\n";
    ivis::common::CheckTimer checkTimer;

    this->setGeometry(QRect(SCREEN_POSITION_X, SCREEN_POSITION_Y, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT));
    this->setMinimumSize(QSize(SCREEN_MINIMUM_WIDTH, SCREEN_MINIMUM_HEIGHT));
    // this->setMaximumSize(QSize(SCREEN_MAXIMUM_WIDTH, SCREEN_MAXIMUM_HEIGHT));
    this->setObjectName(QString("RootWidget"));
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->show();
    checkTimer.check("MainWindow");

    ScreenInfo::instance().data()->updateRootItem(this);
    checkTimer.check("ScreenInfo");

    ConfigSetting::instance().data();
    checkTimer.check("ConfigSetting");

    ControlManager::instance().data()->init();
    checkTimer.check("ControlManager");

    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this, &QApplication::quit);
    // connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this, &QWidget::close);
    // connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this, &QApplication::closeAllWindows());

    // ivis::common::CheckLib* checkLib = new ivis::common::CheckLib(QStringList({"openpyxl", "pandas"}));
    // checkLib->check();

    // ivis::common::CheckLib checkLib(QStringList({"openpyxl", "pandas"}));

//    QStringList libInfo = {"openpyxl", "pandas"};
//    mCheckLib = ivis::common::CheckLib(libInfo);

    mCheckLib.data()->setLibInfo(QStringList({"openpyxl", "pandas"}));
    mCheckLib.data()->check();
    qDebug() << "1 CheckLib :" << mCheckLib.data();
    mCheckLib.clear();
    qDebug() << "2 CheckLib :" << mCheckLib.data();
    checkTimer.check("CheckLib");
}

MainWindow::~MainWindow() {
    qDebug() << "Complete - Exit Application !!!!!!!! \n\n";
}

void MainWindow::mousePressEvent(QMouseEvent* mouseEvent) {
    // ControlManager::instance().data()->mouseEvent(0, mouseEvent);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    // ControlManager::instance().data()->mouseEvent(1, mouseEvent);
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(KeyTypeEnum::KeyInputTypePress, keyEvent);
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(KeyTypeEnum::KeyInputTypeRelease, keyEvent);
}

void MainWindow::closeEvent(QCloseEvent* closeEvent) {
    // qDebug() << "MainWindow::closeEvent()\n\n";
    if (true) {
        closeEvent->accept();
    } else {
        closeEvent->ignore();
    }
}

void MainWindow::moveEvent(QMoveEvent* moveEvent) {
    qDebug() << "MainWindow::moveEvent()->" << moveEvent;
}

void MainWindow::resizeEvent(QResizeEvent* resizeEvent) {
    ScreenInfo::instance().data()->resizeEvent(resizeEvent);
    ControlManager::instance().data()->resizeEvent(resizeEvent);
}
