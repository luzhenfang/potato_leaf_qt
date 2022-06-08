#ifndef IMAGEUPLOADER_H
#define IMAGEUPLOADER_H

#define SERVER_HOST  "https://potato.lzfblog.cn:8080/predict"

#include "result.h"

#include <QObject>
#include <QNetworkAccessManager>



class ImageUploader : public QObject
{
    Q_OBJECT
public:
    explicit ImageUploader(QObject *parent = nullptr);
    void upload(QString imagePath);
private:
    QNetworkAccessManager* networkManager;
    QNetworkReply* reply;
signals:
    void uploadFinished(QString,Result);
};

#endif // IMAGEUPLOADER_H
