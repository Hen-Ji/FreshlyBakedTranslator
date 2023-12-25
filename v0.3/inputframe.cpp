#include "inputframe.h"

InputFrame::InputFrame(QWidget *parent, int w, int h, int r)
    : QWidget{parent}
{
    resize(w, h);
    radius = r; //设置背景圆角半径

    button = new InputFrameButton(this); //创建按钮
    button->move(w-button->width()-20, h-button->height()-20);

    languageButton = new LanguageButton(this); //创建语言选择按钮
    languageButton->move(20, h-languageButton->height()-20);
    language = LANGUAGE_ENGLISH; //默认为英语
    connect(languageButton, &LanguageButton::languageChanged, [=](int lang){
        language = lang; //语言改变时，重新设置语言
    });

    textEdit = new MyEdit(this); //创建输入框
    textEdit->setFrameShape(QFrame::NoFrame);
    textEdit->setStyleSheet("font: 12pt 'Microsoft YaHei UI';");
    textEdit->move(r/2, r/2);
    textEdit->resize(w-r, h-r*1.5-button->height());

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
    textEdit->resize(width()-radius, height()-radius-button->height()-10);
    button->move(width()-button->width()-20, height()-button->height()-20);
}

int InputFrame::getLanguage() {return language;}
