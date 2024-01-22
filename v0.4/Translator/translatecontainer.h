#ifndef TRANSLATECONTAINER_H
#define TRANSLATECONTAINER_H

#include <QWidget>
#include "wordtranslate.h"
#include "wordheader.h"

class TranslateContainer : public QWidget
{
    Q_OBJECT
public:
    explicit TranslateContainer(QWidget *parent = nullptr, int w = 560, int h = 600);

    void wheelEvent(QWheelEvent *ev);
    void resizeEvent(QResizeEvent *);

    void clear();
    void addWordTranslate(WordTranslate* wordTranslate);
    void updateY();

    void setTranslation(QString str);
    void setWordHeader(QString word, QString pron, int count = 0);

    QList<WordTranslate*> wordTranslateList;
    QWidget *container;

    WordContent* translation;
    WordHeader* header;

    int nowHeight;
signals:

};

#endif // TRANSLATECONTAINER_H
