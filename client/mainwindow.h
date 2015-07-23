#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QObject>
#include <QtNetwork>
#include <QTimer>
#include <fstream>
#include <QImage>
#include <cstdlib>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QTcpSocket *client, *file_trans;
    QString file_full,file_path,file_name;
    QPixmap *pixmap; QNetworkAccessManager *manager;
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void Circle_Request();

    void on_pushButton_4_clicked();

    void get();

    void trans();

    void on_pushButton_2_clicked();

    void slot_replyFinished(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
