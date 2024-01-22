#include "wordcontent.h"
#include <QSizePolicy>

WordContent::WordContent(QWidget *parent, int w, int h, int r)
    : QWidget{parent}
{
    resize(w, h);
    radius = r;

    text = new QLabel(this); //创建text
    text->resize(w - r, h - r);
    text->move(r/2, r/2);
    text->setStyleSheet("font: 12pt 'Microsoft YaHei UI Light';");
    text->setWordWrap(true); //开启自动换行
    text->setAlignment(Qt::AlignCenter); //居中
}

void WordContent::paintEvent(QPaintEvent * ){
    QPainter painter(this); //实例化画家
    painter.setRenderHint(QPainter::Antialiasing); //设置抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing); //设置字体抗锯齿

    QPen pen(QColor(216,216,216)); //画背景
    painter.setPen(pen);
    QBrush brush(QColor(216,216,216));
    painter.setBrush(brush);
    painter.drawRoundedRect(1, 1, width()-2, height()-2, radius, radius);
}

void WordContent::setText(QString str) {
    text->setText(str);
    double d = 12.0 / 9.0;
    int h = text->heightForWidth(text->width()/d) * d; //根据要显示的文本重新设置高度(由于设置了字体大小，但还未生效，所以要乘个倍率)
    text->resize(text->width(), h);
    resize(width(), radius + h);
}

void WordContent::resizeEvent(QResizeEvent *) {
    text->resize(width()-radius, text->height());
    int h = text->heightForWidth(text->width());
    if(h != text->height()) {
        text->resize(text->width(), h); //重新设置高度
        resize(width(), radius + h);
    }
}
