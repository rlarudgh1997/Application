#include "ScreenInfo.h"

#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QDebug>
//#include <QGridLayout>
//#include <QSizePolicy>


QSharedPointer<ScreenInfo> ScreenInfo::instance() {
    static QSharedPointer<ScreenInfo> gScreenInfo;

    if (gScreenInfo.isNull()) {
        gScreenInfo = QSharedPointer<ScreenInfo>(new ScreenInfo());
    }

    return gScreenInfo;
}

ScreenInfo::ScreenInfo() {
}

void ScreenInfo::updateRootItem(QWidget *rootItem) {
    if (mRoot == nullptr) {
        mRoot = rootItem;
        mRoot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

void ScreenInfo::controlDisplay(const int& displayType, const bool& show) {
    if (mWidgets[displayType]) {
        (show) ? (mWidgets[displayType]->show()) : (mWidgets[displayType]->hide());
    }
}

QWidget* ScreenInfo::drawDisplay(const int& displayType, const QString& objectName, const bool& show) {
    QWindowList windowList = qApp->allWindows();

    qDebug("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    qDebug("ScreenInfo::drawDisplay(%d, %s, %d)->WindowList=%d", displayType, objectName.toLatin1().data(), show, windowList.count());

     if (windowList.count() > 0) {
        if (mWidgets[displayType] == nullptr) {
            mWidgets[displayType] = new QWidget(mRoot);
            mWidgets[displayType]->setGeometry(0, 0, mRoot->geometry().width(), mRoot->geometry().height());
            mWidgets[displayType]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            mWidgets[displayType]->setStyleSheet("background-color: rgb(50, 50, 100)");
            mWidgets[displayType]->setObjectName(objectName);
            (show) ? (mWidgets[displayType]->show()) : (mWidgets[displayType]->hide());
        }
     }

    qDebug() << "Widget[" << displayType << "] :" << mWidgets[displayType];

    return mWidgets[displayType];
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
    QRect screenRect = (rect.isNull()) ? (mRoot->geometry()) : (rect);
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

#include <QResizeEvent>
void ScreenInfo::resizeEvent(QResizeEvent * resizeEvent) {
//    qDebug() << "\n\tScreenInfo::resizeEvent :" << resizeEvent->oldSize() << " -> " << resizeEvent->size();
    for (auto widget : mWidgets) {
        widget->setGeometry(0, 0, resizeEvent->size().width(), resizeEvent->size().height());
    }
}


void ScreenInfo::loadComplete(const int& displayType) {
    qDebug("ScreenInfo::loadComplete(%d)", displayType);
    emit signalLoadComplete(displayType);
}







