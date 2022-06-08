#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "result.h"

#include <QLabel>
#include <QWidget>

class ImageView : public QLabel
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);

    const QString &getText() const;
    void paintEvent(QPaintEvent*) override;
    void setResult(const Result &newResult);

    const Result &getResult() const;
private:
    Result result;

signals:

};

#endif // IMAGEVIEW_H
