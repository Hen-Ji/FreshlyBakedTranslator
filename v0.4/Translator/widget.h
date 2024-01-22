#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QTimer>
#include "PipeSender.h"
#include <QClipboard>
#include <QList>
#include <QHash>

#include "inputframe.h"
#include "translatecontainer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void resizeEvent(QResizeEvent *);

    PipeSender sender;

    bool translate(QString str);
    void transFormEdit();

public slots:
    void monitorKeys();

private:
    Ui::Widget *ui;

    QTimer* delayTimer;
    QTimer* monitorTimer;

    InputFrame* inputFrame;
    TranslateContainer *container;
    WordContent* messageBox;

    QString wordFileName;
    QHash<QString, int> wordList;

    bool canTrans = true;
};
#endif // WIDGET_H
