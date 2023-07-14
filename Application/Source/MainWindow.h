#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

#include "CommonDefine.h"
#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(MAINWINDOW)


class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
    void closeEvent(QCloseEvent *closeEvent) override;
    void resizeEvent(QResizeEvent * resizeEvent) override;

private:
    int isConvertKey(const QKeyEvent& keyEvent);
};


#endif  // MAIN_WINDOW_H
