#include "inputframebutton.h"

InputFrameButton::InputFrameButton(QWidget *parent, int w, int h, int r)
    : QWidget{parent}
{
    resize(w, h);
    radius = r; //设置背景圆角半径

    setCursor(Qt::PointingHandCursor); //鼠标移动到按钮时变为手
}

void InputFrameButton::paintEvent(QPaintEvent * ){
    QPainter painter(this); //实例化画家
    painter.setRenderHint(QPainter::Antialiasing); //设置抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing); //设置字体抗锯齿

    QPen pen(QColor(236,105,65)); //画背景
    painter.setPen(pen);
    QBrush brush(QColor(236,105,65));
    painter.setBrush(brush);
    painter.drawRoundedRect(0, 0, width(), height(), radius, radius);

    QPen pen2(QColor(255,255,255)); //画箭头
    painter.setPen(pen2);
    QBrush brush2(QColor(255,255,255));
    painter.setBrush(brush2);

    painter.translate(width()/2 + 5, height()/2 - 2); //将原点移到按钮中心靠右的位置
    painter.rotate(45); //旋转45度
    painter.drawRoundedRect(-12, 0, 16, 4, 2, 2); //绘制
    painter.drawRoundedRect(0, 0, 4, 16, 2, 2);

    painter.resetTransform(); //重置坐标系
}

void InputFrameButton::mousePressEvent(QMouseEvent *) {
    emit needTranslate(); //发送需要翻译的信号
}
