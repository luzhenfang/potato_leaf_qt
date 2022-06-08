#include "imageview.h"

#include <QPainter>


namespace DefaultColor{
auto NORMAL = Qt::green;
auto EARELY = QColor("#f97f51");
auto LATELY = Qt::red;
}

ImageView::ImageView(QWidget *parent)
    : QLabel{parent}
{

}



void ImageView::paintEvent(QPaintEvent*){

    //    std::optional<QPixmap> pix = this->pixmap();

    auto pix = this->pixmap();
    // 没有图片则 不进行任何绘制
    if(pix.isNull()){
        return;
    }

    QPainter painter{this};
    QPainter pixPainter{&pix};

    auto getPen = [&](){
        QPen pen;
        return pen;
    };

    auto getBrush = [&](){
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        return brush;
    };
    auto brush = getBrush();
    auto pen = getPen();


    //    if(result.getType()=="健康"){
    //        pen.setColor(DefaultColor::NORMAL);
    //        brush.setColor(DefaultColor::NORMAL);
    //    }else if(result.getType()=="早疫病"){
    //        pen.setColor(DefaultColor::EARELY);
    //        brush.setColor(DefaultColor::EARELY);
    //    }else if(result.getType()=="晚疫病"){
    //        pen.setColor(DefaultColor::LATELY);
    //        brush.setColor(DefaultColor::LATELY);
    //    }else{
    //        pen.setColor(Qt::transparent);
    //        brush.setColor(Qt::transparent);
    //    }
    QMap<QString,std::function<void()>> map;
    map.insert("早疫病",[&](){
        pen.setColor(DefaultColor::EARELY);
        brush.setColor(DefaultColor::EARELY);
    });

    map.insert("晚疫病",[&](){
        pen.setColor(DefaultColor::LATELY);
        brush.setColor(DefaultColor::LATELY);
    });

    map.insert("健康",[&](){
        pen.setColor(DefaultColor::NORMAL);
        brush.setColor(DefaultColor::NORMAL);
    });


    if(!map.count(result.getType())){
        pen.setColor(Qt::transparent);
        brush.setColor(Qt::transparent);
    }else{
        map[result.getType()]();
    }




    pixPainter.setPen(pen);
    pixPainter.setBrush(brush);
    QFont font;
    font.setPointSize(20);
    font.setFamily("苹方-简");
    font.setWeight(QFont::Weight::Normal);
    pixPainter.setFont(font);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
    pixPainter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
    auto val = QString("%1,%2").arg(result.getType(),result.getConf());
    pixPainter.drawText(QRect(pix.rect().left(),5,pix.width(),pix.rect().height()),Qt::AlignLeft,val);
    pixPainter.drawRect(pix.rect().right()-20,0,20,20);
    // 输出绘制好的图片
    painter.drawPixmap(rect(),pix);
    this->setPixmap(pix);
}
const Result &ImageView::getResult() const{
    return result;
}

void ImageView::setResult(const Result &newResult){
    result=newResult;
    qDebug()<<result.getType()<<result.getConf();
    this->update();
}


