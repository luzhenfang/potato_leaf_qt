#include "imageuploader.h"


#include <QJsonObject>
#include <QFile>
#include <QHttpPart>

#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QNetworkRequest>
#include <QVariant>
#include <QJsonParseError>
#include <QColor>
#include <QNetworkReply>
#include <QMessageBox>

namespace ResultCode {
    static int OK =200;
}


ImageUploader::ImageUploader(QObject *parent)
    : QObject{parent}
{
    this->networkManager = new QNetworkAccessManager(this);

}


// 生成 filepart 和 file
auto genFilePartAndFile(QString imagePath){
    auto file = new QFile(imagePath);
    QHttpPart filePart;
    QString header=QString("form-data; name=\"file\";filename=\"%1\"").arg(imagePath);
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(header));
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    return  std::make_tuple(filePart,file);
}


// 上传图片
void ImageUploader::upload(QString imagePath)
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    auto [filePart,_] = genFilePartAndFile(imagePath);
    multiPart->append(filePart);
    QNetworkRequest request;
    request.setUrl(QUrl(SERVER_HOST));
    reply = networkManager->post(request, multiPart);

    connect(networkManager,&QNetworkAccessManager::finished,this,[=](QNetworkReply* reply){
        auto resp = reply->readAll();
        QJsonParseError err_rpt;
        QJsonDocument doc = QJsonDocument::fromJson(resp,&err_rpt);
        if(err_rpt.error){
            return;
        }
        auto obj = doc.object();
        auto code =obj.value("code").toInt();
        auto msg = obj.value("msg").toString();
        auto type = obj.value("type").toString();
        auto conf = obj.value("conf").toString();
        auto result =  Result(this,code,msg,type,conf);
        if(code!=ResultCode::OK){
            QMessageBox::information(nullptr,"提示",msg);
            return;
        }
        emit this->uploadFinished(imagePath,result);
    });
}
