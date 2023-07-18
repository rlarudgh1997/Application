#include "ScreenInfo.h"

#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QDebug>
#include <QResizeEvent>


QSharedPointer<ScreenInfo>& ScreenInfo::instance() {
    static QSharedPointer<ScreenInfo> gScreenInfo;
    if (gScreenInfo.isNull()) {
        gScreenInfo = QSharedPointer<ScreenInfo>(new ScreenInfo());
    }
    return gScreenInfo;
}

ScreenInfo::ScreenInfo() {
}

void ScreenInfo::updateRootItem(QWidget *rootItem) {
    if (mRootScreen == nullptr) {
        mRootScreen = rootItem;
        mRootScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

QWidget* ScreenInfo::drawScreen(const int& displayType, const QString& objectName, const bool& show) {
    QWindowList windowList = qApp->allWindows();

    qDebug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    qDebug("ScreenInfo::drawScreen(%d, %s, %d)", displayType, objectName.toLatin1().data(), show);

     if (windowList.count() > 0) {
        if (mSubScreens[displayType] == nullptr) {
            mSubScreens[displayType] = new QWidget(mRootScreen);
            mSubScreens[displayType]->setGeometry(0, 0, mRootScreen->geometry().width(), mRootScreen->geometry().height());
            mSubScreens[displayType]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            // mSubScreens[displayType]->setStyleSheet("background-color: rgb(50, 50, 100)");
            // mSubScreens[displayType]->setStyleSheet("background-color: rgb(0, 0, 255)");
            mSubScreens[displayType]->setStyleSheet("color: rgb(50, 50, 255)");
            mSubScreens[displayType]->setObjectName(objectName);
            (show) ? (mSubScreens[displayType]->show()) : (mSubScreens[displayType]->hide());
        }
     }

    qDebug() << "Widget :" << mSubScreens[displayType] << "\n\n";
    return mSubScreens[displayType];
}

void ScreenInfo::controlScreen(const int& displayType, const bool& show) {
    if (mSubScreens[displayType]) {
        (show) ? (mSubScreens[displayType]->show()) : (mSubScreens[displayType]->hide());
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
        qDebug("CaptrueScreen - OK -> %s", savePath.toLatin1().data());
    } else {
        qDebug("CaptrueScreen - Fail");
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
    // qDebug("languageFileName=%s", languageFileName.toLatin1().data());

    qApp->removeTranslator(mTranslator);

    if (mTranslator->load(languageFileName)) {
        // qApp->installTranslator(mTranslator);
        // qobject_cast<QQuickView*>(qApp->allWindows().at(0))->engine()->retranslate();
        // const QQuickView* view = qobject_cast<QQuickView*>(qApp->allWindows().at(0));
        // QQmlEngine* engine = view->engine();
        // engine->retranslate();
        // result = true;
    }

    qDebug("ScreenInfo::updateMultiLanguage(%d, %s)->LanguageChange-%s !!!!!!!\n\n\n\n", changeLanguage,
                                            languageFileName.toLatin1().data(), (result)?("Sucess"):("Fail"));

    return result;
}

void ScreenInfo::resizeEvent(QResizeEvent * resizeEvent) {
    // qDebug() << "\n\tScreenInfo::resizeEvent :" << resizeEvent->oldSize() << " -> " << resizeEvent->size();
    foreach(const auto& widget, mSubScreens) {
        widget->setGeometry(0, 0, resizeEvent->size().width(), resizeEvent->size().height());
        // widget->update();
        // qDebug() << "ScreenInfo::WidgetSize:" << widget->geometry();
    }
}

void ScreenInfo::loadComplete(const int& displayType) {
    qDebug("ScreenInfo::loadComplete(%d)", displayType);
    emit signalLoadComplete(displayType);
}
