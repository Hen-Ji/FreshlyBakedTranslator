#ifndef WORDHEADER_H
#define WORDHEADER_H

#include <QWidget>
#include <QLabel>

class WordHeader : public QWidget
{
    Q_OBJECT
public:
    explicit WordHeader(QWidget *parent = nullptr, int w = 560, int h = 100);
    void resizeEvent(QResizeEvent *);

    void setWord(QString str);
    void setPron(QString str);
    void setCount(int i);
    void hideCount();

    QLabel *word, *pron, *count;

signals:

};

#endif // WORDHEADER_H
