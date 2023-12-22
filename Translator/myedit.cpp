#include "myedit.h"


MyEdit::MyEdit(QWidget *parent) : QTextEdit(parent)
{

}

bool MyEdit::event(QEvent *e) {

    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e); // 转为QKeyEvent
        if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) { //按enter翻译
            emit needTranslate();
            return true;
        }
    }

    return QTextEdit::event(e);
}
