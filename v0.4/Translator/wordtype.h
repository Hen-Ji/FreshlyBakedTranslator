#ifndef WORDTYPE_H
#define WORDTYPE_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>

class WordType : public QWidget
{
    Q_OBJECT
public:
    explicit WordType(QWidget *parent = nullptr, int w = 560, int h = 20, int lineW = 120, int r = 1);

    void paintEvent(QPaintEvent *);
    void setText(QString str);

    QLabel *text;

    int lineWidth; //下划线宽度
    int radius; //圆角矩形半径

signals:

};

#endif // WORDTYPE_H
