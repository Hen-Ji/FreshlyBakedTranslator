#ifndef WORDHEADER_H
#define WORDHEADER_H

#include <QWidget>
#include <QLabel>

class WordHeader : public QWidget
{
    Q_OBJECT
public:
    explicit WordHeader(QWidget *parent = nullptr, int w = 560, int h = 100);

    void setWord(QString str);
    void setPron(QString str);

    QLabel *word, *pron;

signals:

};

#endif // WORDHEADER_H
