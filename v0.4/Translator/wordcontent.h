#ifndef WORDCONTENT_H
#define WORDCONTENT_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>

class WordContent : public QWidget
{
    Q_OBJECT
public:
    explicit WordContent(QWidget *parent = nullptr, int w = 560, int h = 75, int r = 30);

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    void setText(QString str);

    QLabel *text;
    int radius;

signals:

};

#endif // WORDCONTENT_H
