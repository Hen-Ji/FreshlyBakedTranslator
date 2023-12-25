#ifndef LANGUAGEBUTTON_H
#define LANGUAGEBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QLabel>

#define LANGUAGE_ENGLISH 1
#define LANGUAGE_JAPANESE 2

class LanguageButton : public QWidget
{
    Q_OBJECT
public:
    explicit LanguageButton(QWidget *parent = nullptr, int w = 160, int h = 40, int r = 20, int lineX = 80);

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);

    int language;  //现在选的是哪种语言
    int radius;  //圆角半径
    int lineX;  //竖线位置

    QLabel *enLabel, *jpLabel;

signals:
    void languageChanged(int lang);

};

#endif // LANGUAGEBUTTON_H
