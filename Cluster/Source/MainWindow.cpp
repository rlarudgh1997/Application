#include "MainWindow.h"
#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlManager.h"

#include <QGuiApplication>
#include <QQmlContext>

// Q_LOGGING_CATEGORY(MAINWINDOW, "MainWindow")

MainWindow::MainWindow(QQuickView* parent) : QQuickView(parent) {
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

#if defined(__MODULE_SUB_WINDOW__)
    mSubWindow = new SubWindow();
    checkTimer.check("SubWindow");
#endif
}

MainWindow::~MainWindow() {
#if defined(__MODULE_SUB_WINDOW__)
    delete mSubWindow;
#endif
    qInfo() << "Complete - Exit Application !!!!!!!! \n\n";
}

void MainWindow::controlConnect() {
#if defined(USE_APP_EXIT_NEW)
    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this, &MainWindow::close,
            Qt::UniqueConnection);
#else
    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this,
            &QGuiApplication::quit,  // &QWidget::close, &QApplication::closeAllWindows()
            Qt::UniqueConnection);
#endif
}

void MainWindow::setContextProperty() {
    this->rootContext()->setContextProperty("ScreenInfoInstance", ScreenInfo::instance().data());
}

void MainWindow::setQmlRegisterType() {
    // Common
    qmlRegisterType<ivis::common::DisplayEnum>("CommonEnum", 1, 0, "Display");
    qmlRegisterType<ivis::common::PropertyEnum>("CommonEnum", 1, 0, "PropertyType");

    // Home
    qmlRegisterType<ivis::common::HandlerHomeEnum>("HomeEnum", 1, 0, "Home");

    // Gauge
    qmlRegisterType<ivis::common::HandlerGaugeEnum>("GaugeEnum", 1, 0, "Gauge");

    qmlRegisterType<ivis::common::SpeedStateType>("GaugeEnum", 1, 0, "SpeedStateType");
    qmlRegisterType<ivis::common::SpeedUnitType>("GaugeEnum", 1, 0, "SpeedUnitType");
    qmlRegisterType<ivis::common::RpmStateType>("GaugeEnum", 1, 0, "RpmStateType");
    qmlRegisterType<ivis::common::SpeedSubDigitalStateType>("GaugeEnum", 1, 0, "SpeedSubDigitalStateType");

    // Telltale
    qmlRegisterType<ivis::common::TelltaleStatType>("TelltaleEnum", 1, 0, "TelltaleStat");
    qmlRegisterType<ivis::common::TelltaleECASStatOptionalType>("TelltaleEnum", 1, 0, "TelltaleECASStatOptional");
    qmlRegisterType<ivis::common::TelltaleECASAxleMotionStatOptionalType>("elltaleEnum", 1, 0,
                                                                          "TelltaleECASAxleMotionStatOptional");
    qmlRegisterType<ivis::common::TelltaleOATIceWarnStatOptionalType>("TelltaleEnum", 1, 0, "TelltaleOATIceWarnStatOptional");
    qmlRegisterType<ivis::common::TelltaleLowFuelWarnStatOptionalType>("TelltaleEnum", 1, 0, "TelltaleLowFuelWarnStatOptional");

    // Evnet
    qmlRegisterType<ivis::common::HandlerEventEnum>("EventEnum", 1, 0, "Event");
    qmlRegisterType<ivis::common::PopupColorType>("EventEnum", 1, 0, "PopupColor");
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

void MainWindow::closeEvent(QCloseEvent* closeEvent) {
#if defined(USE_APP_EXIT_NEW)
    // mSubWindow->close();
    // QQuickView::closeEvent(closeEvent);
    QGuiApplication::quit();  // qApp->quit();
#else
    emit ControlManager::instance().data()->signalExitProgram();
#endif
}
