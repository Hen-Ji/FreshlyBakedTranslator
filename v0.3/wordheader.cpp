#include "wordheader.h"

WordHeader::WordHeader(QWidget *parent, int w, int h)
    : QWidget{parent}
{
    resize(w, h);

    word = new QLabel(this); //创建label
    word->resize(w, 40);
    word->move(10, 0);
    word->setStyleSheet("font: 700 20pt 'Microsoft YaHei UI';");

    pron = new QLabel(this);
    pron->resize(w, 25);
    pron->move(20, 40);
    pron->setStyleSheet("font: 14pt 'Microsoft YaHei UI';");
}


void WordHeader::setWord(QString str) {
    word->setText(str);
}
void WordHeader::setPron(QString str) {
    pron->setText(str);
}
