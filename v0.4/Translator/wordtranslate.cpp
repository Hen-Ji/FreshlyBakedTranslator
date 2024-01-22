#include "wordtranslate.h"

WordTranslate::WordTranslate(QWidget *parent, int w, int r)
    : QWidget{parent}
{
    resize(w, 20);
    radius = r;
    nowHeight = 40;

    type = new WordType(this);
    type->move(10, 0);
}

void WordTranslate::setType(QString str) {
    type->setText(str);
}
void WordTranslate::addContent(QString str) {
    WordContent *content = new WordContent(this, width());
    content->move(0, nowHeight);
    content->setText(str);
    nowHeight += content->height() + 20;

    contentList.append(content);
    resize(width(), nowHeight);
}

void WordTranslate::resizeEvent(QResizeEvent *) {
    int h = 40;
    foreach(auto child, contentList) {
        child->resize(width(), child->height());
        h += child->height() + 20;
    }
    if(h != nowHeight) {
        nowHeight = 40;
        foreach(auto child, contentList) {
            child->move(child->x(), nowHeight);
            nowHeight += child->height() + 20;
        }
        resize(width(), nowHeight);
    }
}
