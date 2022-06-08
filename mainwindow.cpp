#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QNetworkRequest>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QPen>
#include <QAction>
#include <typeinfo>
#include <QPainter>
#include <QMessageBox>
#include <filesystem>
#include <type_traits>
#include <QList>
#include <QMap>
#include "imageuploader.h"
#include <iostream>
namespace fs = std::filesystem;

#define PREDICT_SERVER "https://potato.lzfblog.cn:8080/predict"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toolButton_open->installEventFilter(this);
    ui->toolButton_folder->installEventFilter(this);
    ui->toolButton_next->installEventFilter(this);
    ui->toolButton_pre->installEventFilter(this);
    ui->toolButton_save->installEventFilter(this);
    ui->image->setScaledContents(true);
    ui->pushButton_predict->installEventFilter(this);
#if 0
    auto uploader1 =  new ImageUploader(this);
    auto path = QString("/Users/luckyfang/Downloads/testing/Healthy_25.jpg");
    uploader1 ->upload(path);
    connect(uploader1,&ImageUploader::uploadFinished,this,[](auto a,auto b){
        qDebug()<<a;
    });
#endif


    uploader = new ImageUploader(this);
    connect(uploader,&ImageUploader::uploadFinished,this,&MainWindow::finishUpload);
    fileList =new QStringList();
}




bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
#if 0
    if(obj==ui->label && event->type() == QEvent::MouseButtonPress){
        auto mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button()==Qt::LeftButton){
            this->fileName =  QFileDialog::getOpenFileName(this,"选择图片","ImageFiles(*.jpg *.png)");
            ui->label->setPixmap(QPixmap(fileName));
            ui->label->setScaledContents(true);
        }
    }
#endif

    // offset(3,3)
    if(obj->inherits("QToolButton")){
        auto object = qobject_cast<QToolButton*>(obj);

        auto rect = object->geometry();

        if(event->type()==QEvent::Enter){
              object->setGeometry(rect.x()+3,rect.y()+3,rect.width(),rect.height());
        }

        if(event->type()==QEvent::Leave){
            object->setGeometry(rect.x()-3,rect.y()-3,rect.width(),rect.height());
        }

    }

    if(obj->inherits("QPushButton")){
        auto object = qobject_cast<QPushButton*>(obj);
        auto rect = object->geometry();

        if(event->type()== QMouseEvent::MouseButtonPress){
               object->setGeometry(rect.x()+2,rect.y()+2,rect.width(),rect.height());
        }

        if(event->type()==QMouseEvent::MouseButtonRelease){
            object->setGeometry(rect.x()-2,rect.y()-2,rect.width(),rect.height());
        }
    }

    return false;
}





MainWindow::~MainWindow()
{
    delete ui;
}

// 切换当前显示图片
void MainWindow::setCurrentIndex(const int idx)
{
    currentIndex = idx;
    auto filePath = fileList->at(idx);
    ui->current_step->setText(QString("%1/%2").arg(idx+1).arg(fileList->size()));
    ui->label_filepath->setText(filePath);
    ui->image->setPixmap(QPixmap(filePath));
    ui->image->setResult(Result{});
    update();
}

// 结果预测
void MainWindow::predict(QString filePath)
{
    uploader->upload(filePath);
}

void MainWindow::finishUpload(QString,Result result)
{
    ui->image->setResult(result);
    ui->label_result->setText(result.getType()+","+result.getConf());
    ui->pushButton_predict->setDisabled(false);
}


// 打开单个文件
void MainWindow::on_toolButton_open_clicked()
{

    auto fileName =  QFileDialog::getOpenFileName(this,"选择图片",".",tr("Image Files(*.png *.jpg *.jpeg)"));
    if(fileName.length()==0){
        return;
    }
    fileList->clear();
    fileList->append(fileName);
    setCurrentIndex(0);
    QMessageBox::information(this,"提示",QString("加载完成,共%1个文件!").arg(fileList->size()));
}


// 打开文件夹
void MainWindow::on_toolButton_folder_clicked()
{

    QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,"选择一个文件夹",QDir::currentPath()));
    if(directory.length()==0){
        return;
    }
    auto dir = QDir(directory);
    fileList->clear();
    //    for (auto const &p : fs::directory_iterator(directory.toStdString())){
    //        std::cout<<p.path().string()<<std::endl;
    //        if(p.path().has_relative_path()){
    //            auto path = QString::fromLocal8Bit(p.path().c_str());
    //            qDebug()<<path;
    //            if(path.indexOf(".jpg")==-1){
    //                continue;
    //            }
    //            fileList->append(path);

    //        }
    //    }

    auto fileInfoList =  dir.entryInfoList(QDir::Files);
    foreach(auto fileInfo,fileInfoList){
        if(fileInfo.isDir()){
            continue;
        }
        if(!(fileInfo.fileName().indexOf(".jpg"))){
            continue;
        }
        if(!(fileInfo.fileName().indexOf(".png"))){
            continue;
        }
        if(!(fileInfo.fileName().indexOf(".jpeg"))){
            continue;
        }
        fileList->append(fileInfo.absoluteFilePath());
    }





    setCurrentIndex(0);
    QMessageBox::information(this,"提示",QString("加载完成,共%1个文件!").arg(fileList->size()));
}


// 下一张图片
void MainWindow::on_toolButton_next_clicked()
{
    if(currentIndex+1>=this->fileList->size()){
        QMessageBox::information(this,"提示","已是最后一张图片!");
        return;
    }
    this->setCurrentIndex(currentIndex+1);
}


// 上一张图片
void MainWindow::on_toolButton_pre_clicked()
{
    if(currentIndex-1<0){
        QMessageBox::information(this,"提示","已是第一张图片!");
        return;
    }
    this->setCurrentIndex(currentIndex-1);
}


// 保存图片
void MainWindow::on_toolButton_save_clicked()
{

    ui->image->pixmap().save("./temp.png");
    QMessageBox::information(this,"提示","保存图片成功!");

// 根据平台调用不同的打开方式
#ifdef Q_OS_WIN
    system(QString("start %1").arg(QApplication::applicationDirPath()).toStdString().c_str());
#endif

#ifdef Q_OS_MAC
    system(QString("open %1").arg(QApplication::applicationDirPath()).toStdString().c_str());
#endif
}


void MainWindow::on_pushButton_predict_clicked()
{
    if((currentIndex==0&&fileList->length()==0)||currentIndex<0||currentIndex>=fileList->length())
        return;
    ui->pushButton_predict->setDisabled(true);
    auto path = fileList->at(currentIndex);
    predict(path);
}

