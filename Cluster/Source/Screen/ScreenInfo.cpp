#include "ScreenInfo.h"

#include <QGuiApplication>
#include <QQuickView>

#include <QScreen>
#include <QDateTime>
#include <QResizeEvent>
#include <QDebug>

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

void ScreenInfo::updateRootItem(QQuickItem* rootItem) {
    if (mRootScreen == nullptr) {
        mRootScreen = rootItem;
    }
}

QQuickItem* ScreenInfo::drawScreen(const int& displayType, const QString& qml, const QString& objectName) {
    QWindowList windowList = qApp->allWindows();

    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "ScreenInfo::drawScreen(" << displayType << "," << objectName << ")";

    if (windowList.count() > 0) {
        if (mSubScreens[displayType] == nullptr) {
            try {
                const QQuickView* view = qobject_cast<QQuickView*>(windowList.at(0));
                QQmlEngine* engine = view->engine();
                QQmlComponent component(engine, QUrl(qml));

                mSubScreens[displayType] = qobject_cast<QQuickItem*>(component.create());

                if (mSubScreens[displayType]) {
                    mSubScreens[displayType]->setParent(view->contentItem());
                    mSubScreens[displayType]->setParentItem(view->contentItem());
                    mSubScreens[displayType]->setObjectName(objectName);
                    mSubScreens[displayType]->setZ(displayType);
                    mSubScreens[displayType]->setEnabled(true);  // Default : Enabled
                    mSubScreens[displayType]->setVisible(true);  // Default : Visible
                }
            } catch (...) {
                qDebug() << "Fail to create component";
            }
        }
    }

    qDebug() << "QQuickItem :" << mSubScreens[displayType] << "\n\n";
    return mSubScreens[displayType];
}

void ScreenInfo::controlScreen(const int& displayType, const bool& show) {
    if (mSubScreens[displayType]) {
        foreach (const auto& screen, mSubScreens) {
            try {
                if ((screen == mAlwaysTopScreen) || (screen == mSubScreens[displayType])) {
                    continue;
                }
                // screen->setZ(-displayType);
                screen->setVisible(false);
            } catch (...) {
                qDebug() << "Fail to hide screen :" << screen;
            }
        }
        // mSubScreens[displayType]->setZ(displayType);
        mSubScreens[displayType]->setVisible(true);
    }
}

void ScreenInfo::captureScreen(const QRect& rect) {
#if 0
#define PWD QGuiApplication::applicationDirPath().toLatin1().data()
#define DATE_TIME QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyMMdd_hhmmss")).arg(QTime::currentTime().msec())

#define SCREENSHOT_SAVE_NAME "/CaptureScreen_"
#define FILE_EXTENSION_JPG ".jpg"
#define FILE_EXTENSION_PNG ".png"
#define FILE_EXTENSION_BMP ".bmp"

    QPixmap pixmap = QPixmap();

    if (rect.isNull()) {
        pixmap = QPixmap::grabWindow(0,
                                    static_cast<int>(this->x()),
                                    static_cast<int>(this->y()),
                                    static_cast<int>(this->width()),
                                    static_cast<int>(this->height()));
    } else {
        pixmap = QPixmap::grabWindow(0,
                                    rect.x(),
                                    rect.y(),
                                    rect.width(),
                                    rect.height());
    }

    QString savePath = QString(PWD);

    savePath.append(SCREENSHOT_SAVE_NAME);
    savePath.append(DATE_TIME);
    savePath.append(FILE_EXTENSION_PNG);

    if (pixmap.save(savePath, nullptr, 100)) {
        qDebug("CaptrueScreen - OK, Path : %s", savePath.toLatin1().data());
    } else {
        qDebug("CaptrueScreen - Fail");
    }
#endif
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

    qDebug() << "ScreenInfo::updateMultiLanguage(" << changeLanguage << "," << languageFileName
             << ")->LanguageChange :" << ((result) ? ("Sucess") : ("Fail")) << "!!!!!!!\n\n\n\n";
    return result;
}

void ScreenInfo::resizeScreenInfo(QResizeEvent& resizeEvent) {
}

void ScreenInfo::loadComplete(const int& displayType) {
    qDebug() << "ScreenInfo::loadComplete(" << displayType << ")";
    emit signalLoadComplete(displayType);
}
