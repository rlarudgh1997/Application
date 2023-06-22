#include "MainWindow.h"
#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlManager.h"

#include <QApplication>
#include "CommonUtil.h"


MainWindow::MainWindow() {
    qDebug() << "\n\n\n================================================================================================";
    qDebug() << "[ Application : Path=" << QApplication::applicationDirPath().toLatin1().data() << "]";
    qDebug() << "- QT_VERSION : " << QT_VERSION_STR << "\n\n";

    this->setGeometry(QRect(SCREEN_POSITION_X, SCREEN_POSITION_Y, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT));
    this->setMinimumSize(QSize(SCREEN_MINIMUM_WIDTH, SCREEN_MINIMUM_HEIGHT));
    this->setMaximumSize(QSize(SCREEN_MAXIMUM_WIDTH, SCREEN_MAXIMUM_HEIGHT));
    this->setObjectName(QString("RootWidget"));
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->show();

    ScreenInfo::instance().data()->updateRootItem(this);
    ConfigSetting::instance();

#if 1
    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this, &QApplication::quit);
#else
    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, [=]() {
        // QWidget::close();
        // QApplication::closeAllWindows();
        QApplication::quit();
    });
#endif

    // TEST CODE
    FileInfo::isFileListInfo("");
}

MainWindow::~MainWindow() {
    qDebug() << "\n\n[Complete] Exit Application !!!!!!!! \n\n";
}

void MainWindow::mousePressEvent(QMouseEvent* mouseEvent) {
    // ControlManager::instance().data()->mouseEvent(0, mouseEvent);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    // ControlManager::instance().data()->mouseEvent(1, mouseEvent);
}

int MainWindow::isConvertKey(const QKeyEvent& keyEvent) {
    int inputValue = keyEvent.key();
#if defined(PLATFORM_X86)
    switch (inputValue) {
        case KeyTypeEnum::KeyInputValueNumUp    : { inputValue = KeyTypeEnum::KeyInputValueUp;    break; }
        case KeyTypeEnum::KeyInputValueNumDown  : { inputValue = KeyTypeEnum::KeyInputValueDown;  break; }
        case KeyTypeEnum::KeyInputValueNumLeft  : { inputValue = KeyTypeEnum::KeyInputValueLeft;  break; }
        case KeyTypeEnum::KeyInputValueNumRight : { inputValue = KeyTypeEnum::KeyInputValueRight; break; }
        case KeyTypeEnum::KeyInputValueNumOK    :
        case KeyTypeEnum::KeyInputValueNumOK2   : { inputValue = KeyTypeEnum::KeyInputValueOK;    break; }
        default :                                 {                                               break; }
    }
#endif
    return inputValue;
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(KeyTypeEnum::KeyInputTypePress, isConvertKey(*keyEvent));
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(KeyTypeEnum::KeyInputTypeRelease, isConvertKey(*keyEvent));
}

void MainWindow::closeEvent(QCloseEvent *closeEvent) {
    if (true) {
        closeEvent->accept();
    } else {
        closeEvent->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent * resizeEvent) {
    ScreenInfo::instance().data()->resizeEvent(resizeEvent);
}



