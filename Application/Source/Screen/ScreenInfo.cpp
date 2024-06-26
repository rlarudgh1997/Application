#include "ScreenInfo.h"

#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QResizeEvent>
#include <QDebug>

#define SCREEN_HEIGHT_MARGIN 60

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
    qDebug() << "~ScreenInfo";
}

void ScreenInfo::updateRootItem(QWidget* rootItem) {
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
            try {
                mSubScreens[displayType] = new QWidget(mRootScreen);
                if (mAlwaysTopScreen == nullptr) {
                    mAlwaysTopScreen = mSubScreens[displayType];
                    mSubScreens[displayType]->setGeometry(0, 0, mRootScreen->geometry().width(), SCREEN_HEIGHT_MARGIN);
                } else {
                    mSubScreens[displayType]->setGeometry(0, 0, mRootScreen->geometry().width(),
                                                          mRootScreen->geometry().height());
                }
                mSubScreens[displayType]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                // mSubScreens[displayType]->setStyleSheet("color: rgb(50, 50, 255)");
                mSubScreens[displayType]->setObjectName(objectName);
                mSubScreens[displayType]->show();
            } catch (...) {
                qDebug() << "Fail to create component";
            }
        }
    }

    qDebug() << "Widget :" << mSubScreens[displayType] << "\n\n";
    return mSubScreens[displayType];
}

void ScreenInfo::controlScreen(const int& displayType, const bool& show) {
    if (mSubScreens[displayType]) {
        for (const auto& screen : mSubScreens) {
            try {
                if ((screen == mAlwaysTopScreen) || (screen == mSubScreens[displayType])) {
                    continue;
                }
                screen->hide();
            } catch (...) {
                qDebug() << "Fail to hide screen :" << screen;
            }
        }
        mSubScreens[displayType]->stackUnder(mAlwaysTopScreen);
        // mSubScreens[displayType]->setFocus();
        mSubScreens[displayType]->show();
    }
}

void ScreenInfo::captureScreen(const QRect& rect) {
    // QWindowList windowList = qApp->allWindows();
    QRect screenRect = (rect.isNull()) ? (mRootScreen->geometry()) : (rect);
    QScreen* screen = QGuiApplication::screens()[0];
    QPixmap pixmap = screen->grabWindow(0, screenRect.x(), screenRect.y(), screenRect.width(), screenRect.height());
    QString savePath = QString(QApplication::applicationDirPath().toLatin1().data());
    savePath.append("/CaptureScreen_");
    savePath.append(
        QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyMMdd_hhmmss")).arg(QTime::currentTime().msec()));
    savePath.append(".png");

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
            case LanguageEnglish: {
                languageFileName = QString(":/Language/MULTI_LANGUAGE_ENG.qm");
                break;
            }
            case LanguageKorea: {
                languageFileName = QString(":/Language/MULTI_LANGUAGE_KOR.qm");
                break;
            }
            default: {
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

    // qDebug() << "ScreenInfo::updateMultiLanguage(" << changeLanguage << "," << languageFileName
    //          << ")->LanguageChange :" << ((result) ? ("Success") : ("Fail")) << "!!!!!!!\n\n\n\n";
    return result;
}

void ScreenInfo::resizeScreenInfo(QResizeEvent& resizeEvent) {
    if (mRootScreen) {
        mRootScreen->resize(resizeEvent.size().width(), resizeEvent.size().height());
    }
    for (const auto& widget : mSubScreens) {
        try {
            if (widget == mAlwaysTopScreen) {
                widget->setGeometry(0, 0, resizeEvent.size().width(), SCREEN_HEIGHT_MARGIN);
            } else {
                widget->setGeometry(0, 0, resizeEvent.size().width(), resizeEvent.size().height());
            }
        } catch (...) {
            qDebug() << "Fail to resize widget :" << widget;
        }
    }
}

void ScreenInfo::loadComplete(const int& displayType) {
    // qDebug() << "ScreenInfo::loadComplete(" << displayType << ")";
    emit signalLoadComplete(displayType);
}
