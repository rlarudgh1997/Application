#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

// #include <QLoggingCategory>
// Q_DECLARE_LOGGING_CATEGORY(MAINWINDOW)

class MainWindow : public QWidget {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QStringList, Arguments, QStringList())

public:
    explicit MainWindow(const QStringList& arguments = QStringList());
    ~MainWindow();

private:
    void controlConnect(const bool& graphicsMode = true);
    bool isRunningInDocker();

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
    void closeEvent(QCloseEvent* closeEvent) override;
    void moveEvent(QMoveEvent* moveEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;

signals:
    void signalBootingCompleted();

private:
    QSharedPointer<ivis::common::CheckLib> mCheckLib =
        QSharedPointer<ivis::common::CheckLib>(new ivis::common::CheckLib(), &QObject::deleteLater);
    QRect mScreenInfo = QRect();
};

#endif  // MAIN_WINDOW_H
