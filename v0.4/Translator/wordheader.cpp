#include "wordheader.h"

WordHeader::WordHeader(QWidget *parent, int w, int h)
    : QWidget{parent}
{
    resize(w, h);

    word = new QLabel(this); //创建label
    word->resize(w*2/3, 40);
    word->move(10, 0);
    word->setStyleSheet("font: 700 20pt 'Microsoft YaHei UI Light';");

    pron = new QLabel(this);
    pron->resize(w*2/3, 25);
    pron->move(20, 40);
    pron->setStyleSheet("font: 14pt 'Microsoft YaHei UI Light';");

    count = new QLabel(this);
    count->resize(w/3, 25);
    count->move(w*2/3, 10);
    count->setStyleSheet("font: 12pt 'Microsoft YaHei UI Light';");
    count->setAlignment(Qt::AlignRight); //向右对齐
}

void WordHeader::resizeEvent(QResizeEvent *) {
    word->resize(width()*2/3, 40);

    pron->resize(width()*2/3, 25);

    count->resize(width()/3, 25);
    count->move(width()*2/3, 10);

}

void WordHeader::setWord(QString str) {
    word->setText(str);
}
void WordHeader::setPron(QString str) {
    pron->setText(str);
}
void WordHeader::setCount(int i) {
    count->show();
    count->setText("已查 " + QString::number(i) + " 次");
}
void WordHeader::hideCount() {
    count->hide();
}
