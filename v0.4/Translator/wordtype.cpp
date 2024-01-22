#include "wordtype.h"

WordType::WordType(QWidget *parent, int w, int h, int lineW, int r)
    : QWidget{parent}
{
    resize(w, h);
    lineWidth = lineW;
    radius = r;

    text = new QLabel(this); //创建text
    text->resize(w, h);
    text->move(10, 0);
    text->setStyleSheet("font: 14pt 'Microsoft YaHei UI Light';");
}

void WordType::paintEvent(QPaintEvent *) {
    QPainter painter(this); //实例化画家
    painter.setRenderHint(QPainter::Antialiasing); //设置抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing); //设置字体抗锯齿

    QPen pen(QColor(236,105,65)); //画下划线
    painter.setPen(pen);
    QBrush brush(QColor(236,105,65));
    painter.setBrush(brush);
    painter.drawRoundedRect(0, height()-2, lineWidth, 2, radius, radius);
}

void WordType::setText(QString str) {
    text->setText(str);
}
