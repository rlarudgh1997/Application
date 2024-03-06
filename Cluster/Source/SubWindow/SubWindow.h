#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRect>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class SubWindow;
}
QT_END_NAMESPACE

class SubWindow : public QMainWindow {
    Q_OBJECT

private:
    enum {
        DisplayTypeInvalid = 0,
        DisplayTypeListMain,
        DisplayTypeListSub,
        DisplayTypeListAltonService,
        DisplayTypeListHmi,
        DisplayTypeETC,
    };
    enum {
        DetailInfoInvalid = 0,
        DetailInfoDescription,
        DetailInfoPowerTrain,
        DetailInfoPrecondition,
        DetailInfoPreconditionGroup,
        DetailInfoListen,
        DetailInfoStep,
        DetailInfoStepGroup,
        DetailInfoExpectedResult,
        DetailInfoMax,
    };
    enum {
        ViewTypeInvalid = 0,
        ViewTypeTAV,
        ViewTypeScript,
        ViewTypeRedrawTAV,
    };

public:
    SubWindow(QWidget* parent = nullptr);
    ~SubWindow();

private:
    void init();
    void controlConnect(const bool& state = true);
    void drawDisplay(const int& type, const QString& text = QString());
    void updateFileList();
    void updateDetailFileInfo(const int& viewType, const QString& info);
    void updateDetailDataInfo(const QString& filePath);
    void writeOriginalData(const QString& filePath, const QMap<int, QPair<QString, QString>>& mergeDataInfo);
    QString createToScript(const QString& file);
    QStringList isVsmFileInfo(const QStringList& powerTrainList, const QStringList& signalList);
    QList<QPair<QString, QString>> isReplaceSignal(const QStringList& abstractionList, const QStringList& vsmFileList);
    QString isDataType(const QString& value);
    QMap<int, QPair<QString, QString>> isMergeDataInfo(const QString& oldData, const QString& newData);

// protected:
//     void mousePressEvent(QMouseEvent* mouseEvent) override;
//     void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
//     void mouseMoveEvent(QMouseEvent* mouseEvent) override;
//     void keyPressEvent(QKeyEvent* keyEvent) override;
//     void keyReleaseEvent(QKeyEvent* keyEvent) override;
//     void moveEvent(QMoveEvent* moveEvent) override;
//     void resizeEvent(QResizeEvent* resizeEvent) override;

private:
    Ui::SubWindow* mGui;
    bool mInit = false;
    QStandardItemModel mModel;
    bool mTavSave = false;
    bool mStart = false;
    QMap<int, QString> mDetailInfo = QMap<int, QString>();
    QStringList mOriginalData = QStringList();
    QString mSelectFile = QString();
    QString mPreviousTavData = QString();
};

#endif  // MAINWINDOW_H
