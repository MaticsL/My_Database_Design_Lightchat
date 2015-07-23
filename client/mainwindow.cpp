#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit->setPlainText("");
    client = new QTcpSocket(this);
    connect(client,SIGNAL(readyRead()),this,SLOT(get()));
    file_trans = new QTcpSocket(this);
    connect(file_trans,SIGNAL(readyRead()),this,SLOT(trans()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get()
{
    QString Message=client->readAll();
    if(Message!=""&&Message!="1"){if(!Message.contains("http://"))
        ui->textBrowser->append("<h3 style='color:green'>"+ui->user_b->text()+" : </h3> "+Message);
    else if(Message.contains("jpg",Qt::CaseInsensitive)||Message.contains("png",Qt::CaseInsensitive)||Message.contains("bmp",Qt::CaseInsensitive))
    {
            pixmap = new QPixmap;
            manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_replyFinished(QNetworkReply*)));
            QNetworkRequest request;
            request.setUrl(QUrl(Message));
            manager->get(request);
    }
    else ui->textBrowser->append("<h3 style='color:green'>"+ui->user_b->text()+" : </h3> <a href='"+Message+"'>收到文件</a>");}
}

void MainWindow::slot_replyFinished(QNetworkReply* reply){
 QPixmap pix;
 QByteArray data = reply->readAll();
 pix.loadFromData(data, "JPG");
 QString file=QString::number(rand()*rand(), 10);
 pix.save(file, "JPG", 100);
 ui->textBrowser->append("<img src='"+file+"' />");
}

void MainWindow::trans(){
    QByteArray file_full_QBA = file_full.toLatin1();
    char *file_full_char = file_full_QBA.data();
    std::ifstream fin(file_full_char, std::ios::binary);
    char buf[1024]={0};
    while(!fin.eof()){
        memset(buf,0,sizeof(buf));
        fin.read(buf,sizeof(char)*1024);
        file_trans->write(buf,1024);
    }
    fin.close();
    file_trans->close();
    ui->textBrowser->append("<h3 style='color:green'>发送文件 :</h3>");
    ui->textBrowser->append(file_full);
}

void MainWindow::Circle_Request()
{
    client->connectToHost(QHostAddress("172.18.72.21"), 7001);
    connect(client,SIGNAL(readyRead()),this,SLOT(get()));
    QByteArray content_QBA = ui->user_a->text().toLatin1();
    char *content_char = content_QBA.data();
    client->write(content_char);
}
void MainWindow::on_pushButton_clicked()
{
    client->connectToHost(QHostAddress("172.18.72.21"), 7000);
    QString content_QS = ui->user_a->text()+"|"+ui->user_b->text()+"|"+ui->plainTextEdit->toPlainText();
    QByteArray content_QBA = content_QS.toLatin1();
    char *content_char = content_QBA.data();
    client->write(content_char);
    ui->textBrowser->append("<h3 style='color:green'>"+ui->user_a->text()+" : </h3>"+" "+ui->plainTextEdit->toPlainText());
    ui->plainTextEdit->setPlainText("");
}

void MainWindow::on_pushButton_3_clicked()
{
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Circle_Request()));
    timer->start(1000);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textBrowser->append(QString::fromLocal8Bit("<img src='http://172.18.72.21/a.jpg alt='picture'/>"));
}

void MainWindow::on_pushButton_2_clicked()
{
    file_full =  QFileDialog::getOpenFileName(this,"打开文件","","All Files (*.*)");
    QFileInfo fi;
    fi = QFileInfo(file_full);
    file_name = fi.fileName();
    file_path = fi.absolutePath();
    QString content_QS = ui->user_a->text()+"|"+ui->user_b->text()+"|"+file_name+"|";
    QByteArray content_QBA = content_QS.toLatin1();
    char *content_char = content_QBA.data();
    file_trans->connectToHost(QHostAddress("172.18.72.21"), 7003);
    file_trans->write(content_char);
}
