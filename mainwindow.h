#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageuploader.h"
#include "result.h"

#include <QMainWindow>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject *obj, QEvent *event);
    ~MainWindow();

    void setCurrentIndex(const int);

    void predict(QString filePath);

    void finishUpload(QString path,Result result);
public slots:
//    void replyFinished(QNetworkReply *reply);

private slots:
//    void on_pushButton_clicked();

    void on_toolButton_open_clicked();

    void on_toolButton_folder_clicked();

    void on_toolButton_next_clicked();

    void on_toolButton_pre_clicked();

    void on_toolButton_save_clicked();

    void on_pushButton_predict_clicked();

private:
    Ui::MainWindow *ui;
    QStringList* fileList;
    int currentIndex =0;
    ImageUploader* uploader;
};
#endif // MAINWINDOW_H
