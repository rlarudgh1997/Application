#include "MainWindow.h"
#include "CommonEnum.h"
#include "ScreenInfo.h"
#include "ConfigSetting.h"
#include "ControlManager.h"
#include "ControlExcel.h"
#include "TestCase.h"

#include <QApplication>

// Q_LOGGING_CATEGORY(MAINWINDOW, "MainWindow")

MainWindow::MainWindow(const QStringList& arguments) {
    ivis::common::CheckTimer checkTimer;
    bool graphicsMode = (arguments.size() == 0);
    qInfo() << "\n\n==========================================================================================================";
    qInfo() << "APP_PATH    :" << QApplication::applicationDirPath().toLatin1().data();
    qInfo() << "QT_VERSION  :" << QT_VERSION_STR;
    qInfo() << "GUI_MODE    :" << ((graphicsMode) ? ("Graphic") : ("CLI"));
    qInfo() << "ARGUMENTS   :" << arguments;

    ConfigSetting::instance().data();
    ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeGraphicsMode, graphicsMode);
    checkTimer.check("ConfigSetting");

    mCheckLib.data()->setLibInfo(QStringList({"openpyxl", "pandas"}));
    mCheckLib.data()->check();
    checkTimer.check("CheckLib");

    controlConnect(graphicsMode);
    setArguments(arguments);

    if (graphicsMode) {
        mScreenInfo = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect();
        qInfo() << "SCREEN_INFO :" << mScreenInfo << "\n\n";

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

        // Title Text
        int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
        emit ConfigSetting::instance().data()->signalUpdateWindowTitle(QString(), appMode);
    }
}

MainWindow::~MainWindow() {
    qInfo() << "Complete - Exit Application !!!!!!!! \n\n";
}

void MainWindow::controlConnect(const bool& graphicsMode) {
    connect(this, &MainWindow::signalBootingCompleted, [=]() {
        if (graphicsMode == false) {
            const QString keyGen("GEN");
            QStringList arguments;
            for (const auto& arg : getArguments()) {
                arguments.append(arg.toUpper());
            }
            if (arguments.indexOf(keyGen) >= 0) {
                arguments.removeAll(keyGen);
                TestCase::instance().data()->start(arguments);
            }
        }
    });
    connect(ControlManager::instance().data(), &ControlManager::signalExitProgram, this,
            &QApplication::quit,  // &QWidget::close, &QApplication::closeAllWindows()
            Qt::UniqueConnection);
    connect(mCheckLib.data(), &ivis::common::CheckLib::signalCheckLibResult, [=](const QString& lib, const bool& state) {
        if (lib.compare("openpyxl", Qt::CaseInsensitive) == false) {
            qInfo() << "openpyxl :" << ((state) ? ("valid") : ("invalid"));
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibOpenpyxl, state);
        } else if (lib.compare("pandas", Qt::CaseInsensitive) == false) {
            qInfo() << "pandas   :" << ((state) ? ("valid") : ("invalid"));
            ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibPandas, state);
        } else {
            mCheckLib.clear();
            emit signalBootingCompleted();
        }
    });

    if (graphicsMode) {
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalConfigChanged,
                [=](const int& type, const QVariant& value) {
                    if (type == ConfigInfo::ConfigTypeInit) {
                        mScreenInfo = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect();
                        this->setGeometry(mScreenInfo);
                    }
        });
        connect(ConfigSetting::instance().data(), &ConfigSetting::signalUpdateWindowTitle,
                [=](const QString& title, const int& appMode) {
                    QString text = QString("TC Creator");
                    if (appMode == ivis::common::AppModeEnum::AppModeTypeCV) {
                        text.append("[CV]");
                    } else if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
                        text.append("[PV]");
                    } else {
                    }

                    if (title.size() > 0) {
                        text.append(" : ");
                        text.append(title);
                    }
                    this->setWindowTitle(text);
                });
    }
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
