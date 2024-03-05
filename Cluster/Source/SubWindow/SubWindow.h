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

// class DetailInfo {
// public:
//     DetailInfo() {}
//     DetailInfo(const bool& state, const QString& date) : mState(state), mData(date) {}
//     ~DetailInfo() {}
//     bool isDetailInfoState() { return mState; }
//     QString isDetailInfoData() { return mData; }

// private:
//     bool mState = false;
//     QString mData = QString();
// };

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
    QString isDataType(const QString& value);
    QString createToScript(const QString& file);
    QStringList isVsmFileInfo(const QString& vehicleName, const QStringList& categoryName);
    QStringList isVsmFileInfo(const QStringList& powerTrainList, const QStringList& signalList);
    QList<QPair<QString, QString>> isCanSignal(const QStringList& abstractionList, const QStringList& vsmFileList);
    QMap<int, QStringList> isCanSignal(const bool& sfcSignal, const QString& signalName,
                                       const QMap<int, QStringList>& vsmFileList);

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void mouseMoveEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
    void moveEvent(QMoveEvent* moveEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;

private:
    Ui::SubWindow* mGui;
    bool mInit = false;
    QStandardItemModel mModel;
    bool mStart = false;
    QMap<int, QString> mDetailInfo = QMap<int, QString>();
    QString mSelectFile = QString();
    QString mPreviousTavData = QString();
};

#endif  // MAINWINDOW_H
