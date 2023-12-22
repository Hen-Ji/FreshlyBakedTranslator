#ifndef INPUTFRAME_H
#define INPUTFRAME_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "myedit.h"
#include "inputframebutton.h"

class InputFrame : public QWidget
{
    Q_OBJECT
public:
    explicit InputFrame(QWidget *parent = nullptr, int w = 560, int h = 200, int r = 30);

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);


    int radius; //圆角矩形半径

    MyEdit *textEdit;
    InputFrameButton *button;

signals:

};

#endif // INPUTFRAME_H
