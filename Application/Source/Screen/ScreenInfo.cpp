#include "ScreenInfo.h"

#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QResizeEvent>
#include <QDebug>


#define SCREEN_HEIGHT_MARGIN            60

QSharedPointer<ScreenInfo>& ScreenInfo::instance() {
    static QSharedPointer<ScreenInfo> gScreenInfo;
    if (gScreenInfo.isNull()) {
        gScreenInfo = QSharedPointer<ScreenInfo>(new ScreenInfo());
    }
    return gScreenInfo;
}

ScreenInfo::ScreenInfo() {
}

ScreenInfo::~ScreenInfo() {
    delete mTranslator;
    // foreach(const auto& screen, mSubScreens) {
    //     delete screen;
    // }
    // delete mRootScreen;
    qDebug() << "~ScreenInfo";
}

void ScreenInfo::updateRootItem(QWidget *rootItem) {
    if (mRootScreen == nullptr) {
        mRootScreen = rootItem;
        mRootScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

QWidget* ScreenInfo::drawScreen(const int& displayType, const QString& objectName) {
    QWindowList windowList = qApp->allWindows();

    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "ScreenInfo::drawScreen(" << displayType << "," << objectName << ")";

     if (windowList.count() > 0) {
        if (mSubScreens[displayType] == nullptr) {
            mSubScreens[displayType] = new QWidget(mRootScreen);
            if (mAlwaysTopScreen == nullptr) {
                mAlwaysTopScreen = mSubScreens[displayType];
                mSubScreens[displayType]->setGeometry(0, 0, mRootScreen->geometry().width(), SCREEN_HEIGHT_MARGIN);
            } else  {
                mSubScreens[displayType]->setGeometry(0, 0, mRootScreen->geometry().width(), mRootScreen->geometry().height());
            }
            mSubScreens[displayType]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            mSubScreens[displayType]->setStyleSheet("color: rgb(50, 50, 255)");
            mSubScreens[displayType]->setObjectName(objectName);
            mSubScreens[displayType]->show();
        }
     }

    qDebug() << "Widget :" << mSubScreens[displayType] << "\n\n";
    return mSubScreens[displayType];
}

void ScreenInfo::controlScreen(const int& displayType, const bool& show) {
    if (mSubScreens[displayType]) {
        foreach(const auto& screen, mSubScreens) {
            if ((screen == mAlwaysTopScreen) || (screen == mSubScreens[displayType])) {
                continue;
            }
            screen->hide();
        }
        mSubScreens[displayType]->show();
        mSubScreens[displayType]->stackUnder(mAlwaysTopScreen);
    }
}

void ScreenInfo::captureScreen(const QRect& rect) {
    #define PWD QApplication::applicationDirPath().toLatin1().data()
    #define DATE_TIME QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyMMdd_hhmmss")) \
            .arg(QTime::currentTime().msec())

    #define SCREENSHOT_SAVE_NAME "/CaptureScreen_"
    #define FILE_EXTENSION_JPG ".jpg"
    #define FILE_EXTENSION_PNG ".png"
    #define FILE_EXTENSION_BMP ".bmp"

    // QWindowList windowList = qApp->allWindows();
    QRect screenRect = (rect.isNull()) ? (mRootScreen->geometry()) : (rect);
    QScreen* screen = QGuiApplication::screens()[0];
    QPixmap pixmap = screen->grabWindow(0,
                                        screenRect.x(), screenRect.y(),
                                        screenRect.width(), screenRect.height());
    QString savePath = QString(PWD);
    savePath.append(SCREENSHOT_SAVE_NAME);
    savePath.append(DATE_TIME);
    savePath.append(FILE_EXTENSION_PNG);

    if (pixmap.save(savePath, nullptr, 100)) {
        qDebug() << "CaptrueScreen - OK ->" << savePath;
    } else {
        qDebug() << "CaptrueScreen - Fail";
    }
}

bool ScreenInfo::updateLanguage(const int& changeLanguage, QString languageFileName) {
    bool result = false;

    if (languageFileName.size() == 0) {
        switch (changeLanguage) {
            case LanguageEnglish : {
                languageFileName = QString(":/Language/MULTI_LANGUAGE_ENG.qm");
                break;
            }
            case LanguageKorea : {
                languageFileName = QString(":/Language/MULTI_LANGUAGE_KOR.qm");
                break;
            }
            default : {
                languageFileName = QString(":/Language/MULTI_LANGUAGE_KOR.qm");
                break;
            }
        }
    }

    qApp->removeTranslator(mTranslator);
    if (mTranslator->load(languageFileName)) {
        // qApp->installTranslator(mTranslator);
        // qobject_cast<QQuickView*>(qApp->allWindows().at(0))->engine()->retranslate();
        // const QQuickView* view = qobject_cast<QQuickView*>(qApp->allWindows().at(0));
        // QQmlEngine* engine = view->engine();
        // engine->retranslate();
        // result = true;
    }

    qDebug() << "ScreenInfo::updateMultiLanguage(" << changeLanguage << "," << languageFileName
                                        << ")->LanguageChange :" << ((result)?("Sucess"):("Fail")) << "!!!!!!!\n\n\n\n";
    return result;
}

void ScreenInfo::resizeScreenInfo(QResizeEvent& resizeEvent) {
    if (mRootScreen) {
        mRootScreen->resize(resizeEvent.size().width(), resizeEvent.size().height());
    }
    foreach(const auto& widget, mSubScreens) {
        if (widget == mAlwaysTopScreen) {
            widget->setGeometry(0, 0, resizeEvent.size().width(), SCREEN_HEIGHT_MARGIN);
        } else {
            widget->setGeometry(0, 0, resizeEvent.size().width(), resizeEvent.size().height());
        }
    }
}

void ScreenInfo::loadComplete(const int& displayType) {
    qDebug() << "ScreenInfo::loadComplete(" << displayType << ")";
    emit signalLoadComplete(displayType);
}
