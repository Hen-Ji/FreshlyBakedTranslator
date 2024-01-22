#include "translatecontainer.h"

TranslateContainer::TranslateContainer(QWidget *parent, int w, int h)
    : QWidget{parent}
{
    resize(w, h);
    nowHeight = 0;
    container = new QWidget(this);
    container->resize(w, h);
    container->move(0, 0);

    header = new WordHeader(container, container->width()); //创建单词头
    header->move(0, 0);
    header->hide();

    translation = new WordContent(container, container->width()); //创建翻译
    translation->move(0, 0);
    translation->hide();
}


void TranslateContainer::clear() {
    foreach(auto child, wordTranslateList) {
        delete child;
    }
    wordTranslateList.clear();
    nowHeight = 0;
    container->move(0, 0);

    translation->hide();
    header->hide();
}
void TranslateContainer::addWordTranslate(WordTranslate* wordTranslate) {

    wordTranslate->setParent(container);
    wordTranslate->move(0, nowHeight);
    wordTranslate->show();
    nowHeight += wordTranslate->height();
    container->resize(width(), nowHeight);

    wordTranslateList.append(wordTranslate);
}

void TranslateContainer::wheelEvent(QWheelEvent *ev) {
    int dt = ev->angleDelta().y() / 3;
    int y = container->y() + dt;
    int h = container->height();
    if(h > height()) { //移动
        if(y <= 0 && y + h >= height()) {
            container->move(container->x(), y);
        }
        else if(y > 0) {
            container->move(container->x(), 0);
        }
        else if(y + h < height()) {
            container->move(container->x(), height() - h);
        }
    }
}

void TranslateContainer::resizeEvent(QResizeEvent *) {
    updateY();
}

void TranslateContainer::updateY() {
    header->resize(width(), header->height()); //改变单词头宽度

    if(wordTranslateList.size() > 0) { //是单词翻译

        int hei = header->height();
        foreach(auto child, wordTranslateList) {
            child->resize(width(), child->height());
            child->move(child->x(), hei);
            hei += child->height();
        }
        container->resize(width(), hei);
        nowHeight = hei;


        int y = container->y(); //不能超出范围
        int h = container->height();
        if(y + h < height() && y < 0) container->move(container->x(), height() - h);
    }
    else { //是句子翻译
        translation->resize(width(), translation->height()); //更新高度
        nowHeight = translation->height();
        container->resize(width(), nowHeight);

        int y = container->y(); //不能超出范围
        int h = container->height();
        if(y + h < height() && y < 0) container->move(container->x(), height() - h);
    }
}

void TranslateContainer::setTranslation(QString str) {
    translation->show();
    nowHeight = header->height();

    translation->setText(str); //设置句子翻译
    nowHeight = translation->height();
    container->resize(container->width(), nowHeight);
}

void TranslateContainer::setWordHeader(QString word, QString pron, int count) {
    header->show();
    nowHeight = header->height();

    header->setWord(word); //设置单词头
    header->setPron(pron);
    if(count == 0) {  //之前没有查过就不显示
        header->hideCount();
    }
    else {//之前有查过就显示
        header->setCount(count);
    }
}
