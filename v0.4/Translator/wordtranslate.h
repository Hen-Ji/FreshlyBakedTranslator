#ifndef WORDTRANSLATE_H
#define WORDTRANSLATE_H

#include <QWidget>
#include <QList>
#include "wordcontent.h"
#include "wordtype.h"


class WordTranslate : public QWidget
{
    Q_OBJECT
public:
    explicit WordTranslate(QWidget *parent = nullptr, int w = 560, int r = 30);

    void setType(QString str);
    void addContent(QString str);

    void resizeEvent(QResizeEvent *);

    WordType *type;
    QList<WordContent*> contentList;

    int radius;
    int nowHeight;

signals:

};

#endif // WORDTRANSLATE_H
