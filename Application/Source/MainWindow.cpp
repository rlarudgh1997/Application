#include "MainWindow.h"
#include "CommonEnum.h"

#include <QApplication>
#include <QDebug>
#include <QVector>
#include <QMap>
#include <QList>


MainWindow::MainWindow() {
    qDebug() << "\n\n\n================================================================================================";
    qDebug() << "[[[ Application : Path=" << QApplication::applicationDirPath().toLatin1().data() << "]]]";
    qDebug() << "QT_VERSION_STR : " << QT_VERSION_STR;

    // Setting Screen
// #if defined(PLATFORM_TARGET)
//     QGuiApplication::setOverrideCursor(Qt::BlankCursor);    // Cusor - Disable
//     this->setFlags(Qt::FramelessWindowHint);
// #else
//    QCoreApplication::setApplicationName("APPLICATION : X86");
// #endif
    this->setColor(QColor(Qt::black));
    this->setResizeMode(QQuickView::SizeViewToRootObject);
    this->setGeometry(QRect(SCREEN_POSITION_X, SCREEN_POSITION_Y, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT));
    this->setMinimumSize(QSize(SCREEN_MINIMUM_SIZE_WIDTH, SCREEN_MINIMUM_SIZE_HEIGHT));
    this->setMaximumSize(QSize(SCREEN_MAXIMUM_SIZE_WIDTH, SCREEN_MAXIMUM_SIZE_HEIGHT));
    this->show();
}

MainWindow::~MainWindow() {
    qDebug() << "================================================================================================\n\n";
}

void MainWindow::mousePressEvent(QMouseEvent* mouseEvent) {
    qDebug() << "mousePressEvent()";
    if (mouseEvent->button() == Qt::RightButton) {
    } else {
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    qDebug() << "mousePressEvent()";
    if (mouseEvent->button() == Qt::RightButton) {
    } else {
    }
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    qDebug() << "keyPressEvent()";
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    qDebug() << "keyReleaseEvent()";
}

void MainWindow::closeEvent(QCloseEvent *closeEvent) {
    qDebug() << "closeEvent()";
    // if (maybeSave()) {
    //     writeSettings();
    //     event->accept();
    // } else {
    //     event->ignore();
    // }
}

void MainWindow::resizeEvent(QResizeEvent * resizeEvent) {
    qDebug() << "resizeEvent :" << resizeEvent->oldSize() << " -> " << resizeEvent->size();
    // if (mWidget) {
    //     int posX = (this->width() - 500) / 2;
    //     int posY = (this->height() - 300) / 2;
    //     mWidget->setGeometry(posX, posY, 500, 300);
    // }
}






void MainWindow::slotPowerOff(bool reboot) {
    this->close();
}


