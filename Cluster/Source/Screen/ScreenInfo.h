#ifndef SCREEN_INFO_H
#define SCREEN_INFO_H

#include <QObject>
#include <QQuickItem>
#include <QMap>
#include <QTranslator>

class ScreenInfo : public QQuickItem {
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
    void updateRootItem(QQuickItem* rootItem);
    QQuickItem* drawScreen(const int& displayType, const QString& qml, const QString& objectName);
    void controlScreen(const int& displayType, const bool& show = true);
    void resizeScreenInfo(QResizeEvent& resizeEvent);
    bool updateLanguage(const int& changeLanguage, QString languageFileName = QString());
    Q_INVOKABLE void captureScreen(const QRect& rect = QRect());
    Q_INVOKABLE void loadComplete(const int& displayType);

private:
    explicit ScreenInfo();

signals:
    void signalLoadComplete(const int& displayType);

private:
    QQuickItem* mRootScreen = nullptr;
    QQuickItem* mAlwaysTopScreen = nullptr;
    QMap<int, QQuickItem*> mSubScreens = QMap<int, QQuickItem*>();
    QTranslator* mTranslator = new QTranslator();
};

#endif  // SCREEN_INFO_H
