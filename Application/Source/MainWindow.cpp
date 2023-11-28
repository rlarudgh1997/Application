#include "MainWindow.h"
#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlManager.h"

#include <QApplication>

// Q_LOGGING_CATEGORY(MAINWINDOW, "MainWindow")

MainWindow::MainWindow() {
    qInfo() << "================================================================================================";
    qInfo() << "APP_PATH    :" << QApplication::applicationDirPath().toLatin1().data();
    qInfo() << "QT_VERSION  :" << QT_VERSION_STR;
    ivis::common::CheckTimer checkTimer;

    ConfigSetting::instance().data();
    mScreenInfo = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect();
    qInfo() << "SCREEN_INFO :" << mScreenInfo << "\n\n";
    checkTimer.check("ConfigSetting");

    this->setWindowTitle("TC Creator");
    this->setGeometry(mScreenInfo);
    this->setMinimumSize(QSize(SCREEN_MINIMUM_WIDTH, SCREEN_MINIMUM_HEIGHT));
    // this->setMaximumSize(QSize(SCREEN_MAXIMUM_WIDTH, SCREEN_MAXIMUM_HEIGHT));
    this->setObjectName(QString("RootWidget"));
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->show();

    ScreenInfo::instance().data()->updateRootItem(this);
    checkTimer.check("ScreenInfo");

    ControlManager::instance().data()->init();
    checkTimer.check("ControlManager");

    mCheckLib.data()->setLibInfo(QStringList({"openpyxl", "pandas"}));
    mCheckLib.data()->check();
    checkTimer.check("CheckLib");

    controlConnect();
}

MainWindow::~MainWindow() {
    qInfo() << "Complete - Exit Application !!!!!!!! \n\n";
}

void MainWindow::controlConnect() {
    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this,
            &QApplication::quit,  // &QWidget::close, &QApplication::closeAllWindows()
            Qt::UniqueConnection);
    connect(ConfigSetting::instance().data(), &ConfigSetting::signalUpdateWindowTitle, [=](const QString& title) {
        QString text = QString("TC Creator");
        if (title.size() > 0) {
            text.append(" : ");
            text.append(title);
        }
        this->setWindowTitle(text);
    });
    connect(mCheckLib.data(), &ivis::common::CheckLib::signalCheckLibResult, [=](const QString& lib, const bool& state) {
        if (lib.compare("openpyxl", Qt::CaseInsensitive) == false) {
            qInfo() << "Python lib openpyxl :" << ((state) ? ("valid") : ("invalid"));
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibOpenpyxl, state);
        } else if (lib.compare("pandas", Qt::CaseInsensitive) == false) {
            qInfo() << "Python lib pandas   :" << ((state) ? ("valid") : ("invalid"));
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibPandas, state);
        } else {
            mCheckLib.clear();
        }
    });
}
void MainWindow::mousePressEvent(QMouseEvent* mouseEvent) {
    Q_UNUSED(mouseEvent)
    // ControlManager::instance().data()->mouseEvent(0, mouseEvent);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    Q_UNUSED(mouseEvent)
    // ControlManager::instance().data()->mouseEvent(1, mouseEvent);
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(ivis::common::KeyTypeEnum::KeyInputTypePress, keyEvent);
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(ivis::common::KeyTypeEnum::KeyInputTypeRelease, keyEvent);
}

void MainWindow::closeEvent(QCloseEvent* closeEvent) {
    closeEvent->ignore();
    if (closeEvent->type() == QEvent::Type::Close) {
        ControlManager::instance().data()->exitProgram(true);
    }
}

void MainWindow::moveEvent(QMoveEvent* moveEvent) {
    mScreenInfo = QRect(moveEvent->pos().x(), moveEvent->pos().y(), mScreenInfo.width(), mScreenInfo.height());
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeScreenInfo, mScreenInfo);
}

void MainWindow::resizeEvent(QResizeEvent* resizeEvent) {
    ScreenInfo::instance().data()->resizeScreenInfo(*resizeEvent);
    ControlManager::instance().data()->resizeEvent(*resizeEvent);

    mScreenInfo = QRect(mScreenInfo.x(), mScreenInfo.y(), resizeEvent->size().width(), resizeEvent->size().height());
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeScreenInfo, mScreenInfo);
}
