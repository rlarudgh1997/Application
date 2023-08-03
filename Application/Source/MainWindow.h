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

public:
    explicit MainWindow();
    ~MainWindow();


private:
    void controlConnect();


protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
    void closeEvent(QCloseEvent* closeEvent) override;
    void moveEvent(QMoveEvent* moveEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;


private:
    QSharedPointer<ivis::common::CheckLib> mCheckLib =
                                    QSharedPointer<ivis::common::CheckLib>(new ivis::common::CheckLib(), &QObject::deleteLater);
    QRect mScreenInfo = QRect();
};


#endif  // MAIN_WINDOW_H
