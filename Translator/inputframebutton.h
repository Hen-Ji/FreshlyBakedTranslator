#ifndef INPUTFRAMEBUTTON_H
#define INPUTFRAMEBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

class InputFrameButton : public QWidget
{
    Q_OBJECT
public:
    explicit InputFrameButton(QWidget *parent = nullptr, int w = 50, int h = 50, int r = 20);

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

    int radius; //圆角矩形半径

signals:
    void needTranslate();
};

#endif // INPUTFRAMEBUTTON_H
