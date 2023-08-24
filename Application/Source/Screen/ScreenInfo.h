#ifndef SCREEN_INFO_H
#define SCREEN_INFO_H

#include <QWidget>
#include <QMap>
#include <QTranslator>



class ScreenInfo : public QWidget {
    Q_OBJECT

public:
    enum Language {
        LanguageInvalid,
        LanguageKorea,
        LanguageEnglish,
    };


public:
    static QSharedPointer<ScreenInfo>& instance();
    ~ScreenInfo();
    void updateRootItem(QWidget *rootItem);
    QWidget* drawScreen(const int& displayType, const QString& objectName);
    void controlScreen(const int& displayType, const bool& show = true);
    Q_INVOKABLE void captureScreen(const QRect& rect = QRect());
    bool updateLanguage(const int& changeLanguage, QString languageFileName = QString());
    Q_INVOKABLE void loadComplete(const int& displayType);
    void resizeScreenInfo(QResizeEvent& resizeEvent);


private:
    explicit ScreenInfo();


signals:
    void signalLoadComplete(const int& displayType);


private:
    QWidget* mRootScreen = nullptr;
    QWidget* mAlwaysTopScreen = nullptr;
    QMap<int, QWidget*> mSubScreens = QMap<int, QWidget*>();
    QTranslator *mTranslator = new QTranslator();
};

#endif    // SCREEN_INFO_H
