#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QQuickView>
#include <QSharedPointer>

#include "CommonDefine.h"

class MainWindow : public QQuickView {
    Q_OBJECT

public:
    explicit MainWindow(QQuickView* parent = nullptr);
    ~MainWindow();

private:
    void controlConnect();
    void setContextProperty();
    void setQmlRegisterType();

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
    void moveEvent(QMoveEvent* moveEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;

private:
    QRect mScreenInfo = QRect();
};

#endif  // MAIN_WINDOW_H
