#ifndef MYEDIT_H
#define MYEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>
#include <QEvent>

class MyEdit : public QTextEdit
{
    Q_OBJECT  // 添加宏时需要重新qmake（或者直接把生成的文件全删了，再生成一遍）
public:
    explicit MyEdit(QWidget *parent = nullptr);
    ~MyEdit(){};

    bool event(QEvent *e); //需要使用event, 用keyPressEvent会输不进去东西

signals:
    void needTranslate();
};

#endif // MYEDIT_H
