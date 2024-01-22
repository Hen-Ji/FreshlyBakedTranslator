#include "languagebutton.h"

LanguageButton::LanguageButton(QWidget *parent, int w, int h, int r, int lineX)
    : QWidget{parent}
{
    resize(w, h);
    radius = r;
    this->lineX = lineX;
    language = LANGUAGE_ENGLISH; //默认选英语

    setCursor(Qt::PointingHandCursor); //鼠标移动到按钮时变为手

    enLabel = new QLabel(this);
    enLabel->move(0, 5);
    enLabel->resize(lineX, h-10);
    enLabel->setStyleSheet("font: 13pt 'Microsoft YaHei UI'; color: rgb(255, 255, 255);");
    enLabel->setAlignment(Qt::AlignCenter);
    enLabel->setText("English");

    jpLabel = new QLabel(this);
    jpLabel->move(lineX, 5);
    jpLabel->resize(w - lineX, h-10);
    jpLabel->setStyleSheet("font: 13pt 'Microsoft YaHei UI'; color: rgb(127, 127, 127);");
    jpLabel->setAlignment(Qt::AlignCenter);
    jpLabel->setText("日本語");
}

void LanguageButton::paintEvent(QPaintEvent * ){
    QPainter painter(this); //实例化画家
    painter.setRenderHint(QPainter::Antialiasing); //设置抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing); //设置字体抗锯齿

    QPen pen3(QColor(236,105,65)); //画背景
    painter.setPen(pen3);
    QBrush brush3(QColor(236,105,65));
    painter.setBrush(brush3);

    if(language == LANGUAGE_ENGLISH) {  //英语
        painter.drawRoundedRect(1, 1, lineX-2, height()-2, radius, radius);
    }
    else if(language == LANGUAGE_JAPANESE) {  //日语
        painter.drawRoundedRect(lineX+1, 1, width()-lineX-2, height()-2, radius, radius);
    }
}

void LanguageButton::mousePressEvent(QMouseEvent *ev) {
    if(ev->pos().x() < lineX) {  //点的是英语
        language = LANGUAGE_ENGLISH;
        enLabel->setStyleSheet("font: 13pt 'Microsoft YaHei UI'; color: rgb(255, 255, 255);");  //更改字体颜色（不能放到paintevent里，背景绘制会有延迟）
        jpLabel->setStyleSheet("font: 13pt 'Microsoft YaHei UI'; color: rgb(127, 127, 127);");
    }
    else if(ev->pos().x() > lineX) {  //点的是日语
        language = LANGUAGE_JAPANESE;
        jpLabel->setStyleSheet("font: 13pt 'Microsoft YaHei UI'; color: rgb(255, 255, 255);");
        enLabel->setStyleSheet("font: 13pt 'Microsoft YaHei UI'; color: rgb(127, 127, 127);");
    }
    emit languageChanged(language); //发送语言改变的信号
    update(); //重绘
}
