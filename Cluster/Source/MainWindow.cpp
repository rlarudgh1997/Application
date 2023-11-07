#include "MainWindow.h"
#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlManager.h"

#include <QGuiApplication>
#include <QQmlContext>


// Q_LOGGING_CATEGORY(MAINWINDOW, "MainWindow")


MainWindow::MainWindow(QQuickView *parent) : QQuickView(parent) {
    qInfo() << "================================================================================================";
    qInfo() << "APP_PATH    :" << QGuiApplication::applicationDirPath().toLatin1().data();
    qInfo() << "QT_VERSION  :" << QT_VERSION_STR;
    ivis::common::CheckTimer checkTimer;

#if defined(PLATFORM_TARGET)
    QGuiApplication::setOverrideCursor(Qt::BlankCursor);
    this->setFlags(Qt::FramelessWindowHint);
#endif
    // this->setColor(QColor(Qt::black));
    this->setResizeMode(QQuickView::SizeViewToRootObject);
    this->setGeometry(QRect(SCREEN_POSITION_X, SCREEN_POSITION_Y, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT));
    this->setMinimumSize(QSize(SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT));
    this->setMaximumSize(QSize(SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT));
    this->show();

    setContextProperty();
    setQmlRegisterType();
    checkTimer.check("QuickRegister");

    ControlManager::instance().data()->init();
    checkTimer.check("ControlManager");

    controlConnect();
}

MainWindow::~MainWindow() {
    qInfo() << "Complete - Exit Application !!!!!!!! \n\n";
}

void MainWindow::controlConnect() {
    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram,
            this,                              &QGuiApplication::quit,    // &QWidget::close, &QApplication::closeAllWindows()
            Qt::UniqueConnection);
}

void MainWindow::setContextProperty() {
    this->rootContext()->setContextProperty("ScreenInfoInstance", ScreenInfo::instance().data());
}

void MainWindow::setQmlRegisterType() {
    // Common
    qmlRegisterType<ivis::common::DisplayEnum>                           ("CommonEnum",          1, 0,
                                                                          "Display");
    qmlRegisterType<ivis::common::PropertyEnum>                          ("CommonEnum",          1, 0,
                                                                          "PropertyType");

    // Home
    qmlRegisterType<ivis::common::HandlerHomeEnum>                       ("GaugeEnum",     1, 0,
                                                                          "Gauge");

    // Gauge
    qmlRegisterType<ivis::common::HandlerHomeEnum>                       ("HomeEnum",     1, 0,
                                                                          "Home");

    // Telltale
    qmlRegisterType<ivis::common::TelltaleStatType>                      ("TelltaleEnum", 1, 0,
                                                                          "TelltaleStat");
    qmlRegisterType<ivis::common::TelltaleECASStatOptionalType>          ("TelltaleEnum", 1, 0,
                                                                          "TelltaleECASStatOptional");
    qmlRegisterType<ivis::common::TelltaleECASAxleMotionStatOptionalType>("elltaleEnum", 1, 0,
                                                                          "TelltaleECASAxleMotionStatOptional");
    qmlRegisterType<ivis::common::TelltaleOATIceWarnStatOptionalType>    ("TelltaleEnum", 1, 0,
                                                                          "TelltaleOATIceWarnStatOptional");
    qmlRegisterType<ivis::common::TelltaleLowFuelWarnStatOptionalType>   ("TelltaleEnum", 1, 0,
                                                                          "TelltaleLowFuelWarnStatOptional");
}

void MainWindow::mousePressEvent(QMouseEvent* mouseEvent) {
    // ControlManager::instance().data()->mouseEvent(0, mouseEvent);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    // ControlManager::instance().data()->mouseEvent(1, mouseEvent);
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(ivis::common::KeyEnum::KeyInputTypePress, keyEvent);
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    ControlManager::instance().data()->keyEvent(ivis::common::KeyEnum::KeyInputTypeRelease, keyEvent);
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
