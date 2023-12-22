#include "inputframe.h"

InputFrame::InputFrame(QWidget *parent, int w, int h, int r)
    : QWidget{parent}
{
    resize(w, h);
    radius = r; //设置背景圆角半径

    textEdit = new MyEdit(this); //创建输入框
    textEdit->setFrameShape(QFrame::NoFrame);
    textEdit->setStyleSheet("font: 12pt 'Microsoft YaHei UI';");
    textEdit->move(r/2, r/2);
    textEdit->resize(w-r, h-r);

    button = new InputFrameButton(this); //创建按钮
    button->move(w-70, h-70);

}
void InputFrame::paintEvent(QPaintEvent * ){
    QPainter painter(this); //实例化画家
    painter.setRenderHint(QPainter::Antialiasing); //设置抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing); //设置字体抗锯齿

    QPen pen(QColor(255,255,255)); //画背景
    painter.setPen(pen);
    QBrush brush(QColor(255,255,255));
    painter.setBrush(brush);
    painter.drawRoundedRect(0, 0, width(), height(), radius, radius);
}
void InputFrame::resizeEvent(QResizeEvent *) {
    textEdit->resize(width()-radius, height()-radius);
    button->move(width()-70, height()-70);
}
