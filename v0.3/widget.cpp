#include "widget.h"
#include "ui_widget.h"
#include <QDir>

#include <QJsonObject> //解析json的头文件
#include <QJsonDocument> //解析json的头文件
#include <QJsonArray> //解析json的头文件

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    resize(600, 800);

    inputFrame = new InputFrame(this);
    inputFrame->move(20, 20);

    container = new TranslateContainer(this);
    container->move(20, inputFrame->y() + inputFrame->height() + 20);

    messageBox = new WordContent(this, width());
    messageBox->setText("使用方法：\n1. 在上方输入要查的单词或句子，按下 查 或 按 回车键\n2. 鼠标选中要查的单词或句子，按 ctrl + c + c + c");
    messageBox->move(20, inputFrame->y() + inputFrame->height() + 20);

    delayTimer = new QTimer(this);
    monitorTimer = new QTimer(this);

    connect(delayTimer, &QTimer::timeout, [=](){
        canTrans = true;
        delayTimer->stop();
    });

    connect(monitorTimer, &QTimer::timeout, this, &Widget::monitorKeys);

    connect(inputFrame->button, &InputFrameButton::needTranslate, [=](){
        transFormEdit();
    });

    connect(inputFrame->textEdit, &MyEdit::needTranslate, [=](){
        transFormEdit();
    });

    sender.create(L"TranslatorPipe", 65536); //创建命名管道
    monitorTimer->start(25); //每25ms检测一次是否需要翻译
}

Widget::~Widget()
{
    delete ui;
    sender.close(); //关闭管道
}
bool Widget::translate(QString str) {
    if(!canTrans) { //需要间隔时间
        return false;
    }

    canTrans = false;
    container->clear(); //清除之前的翻译内容

    //运行py程序
    int lang = inputFrame->getLanguage(); //获取要翻译的语言
    if(lang == LANGUAGE_ENGLISH) { //英语
        ShellExecute(nullptr, L"open", (QCoreApplication::applicationDirPath().toStdWString() + L"/EnglishTranslator.py").c_str(), nullptr, nullptr, SW_HIDE);
    }
    else if(lang == LANGUAGE_JAPANESE) { //日语
        ShellExecute(nullptr, L"open", (QCoreApplication::applicationDirPath().toStdWString() + L"/JapaneseTranslator.py").c_str(), nullptr, nullptr, SW_HIDE);
    }
    else { //不知道就为英语
        ShellExecute(nullptr, L"open", (QCoreApplication::applicationDirPath().toStdWString() + L"/EnglishTranslator.py").c_str(), nullptr, nullptr, SW_HIDE);
    }

    //等待连接
    sender.connect();

    sender.write(str.toStdString()); //发送数据

    QString res = QString::fromStdString(sender.read()); //读取数据

    sender.disconnect(); //断开连接

    //解析json
    QJsonObject resJson = QJsonDocument::fromJson(res.toUtf8()).object();

    if (resJson["isWord"] == true) { //是英语单词
        container->setWordHeader(resJson["word"].toString(), resJson["pronunciation"].toString()); //设置单词头

        QJsonArray meanings = resJson["meanings"].toArray();
        for(int i = 0; i < meanings.size(); i++) {
            QJsonObject meaning = meanings[i].toObject();
            WordTranslate *word = new WordTranslate(this, container->width()); //把内容放进去

            word->setType(meaning["type"].toString()); //词性

            for(int j = 0; j < meaning["meanings"].toArray().size(); j++) { //释义
                word->addContent(meaning["meanings"].toArray()[j].toString());
            }

            container->addWordTranslate(word); //添加到容器中
        }
    }

    else if (resJson["isSentence"] == true) { //是英语句子
        container->setTranslation(resJson["meaning"].toString());  //设置句子翻译
    }

    else { //获取出错
        delayTimer->start(1000); //休息1s
        return false;
    }

    container->updateY();
    delayTimer->start(1000); //休息1s
    return true;
}

void Widget::transFormEdit() {
    container->clear(); //清除之前的翻译内容

    //查
    messageBox->show();
    messageBox->setText("翻译中...");

    QString str = inputFrame->textEdit->toMarkdown();
    str.remove(str.size()-2, 2);
    bool res = translate(str);

    if(!res) {
        messageBox->show();
        messageBox->setText("没查到，请检查拼写是否错误或者等一秒再查");
    }
    else messageBox->hide();
}

void Widget::resizeEvent(QResizeEvent *) {
    ui->bg->resize(width(), height());

    inputFrame->resize(width()-40, inputFrame->height());
    container->resize(width()-40, height() - inputFrame->height()-60);
    messageBox->resize(width()-40, messageBox->height());
}

void Widget::monitorKeys() { //检测按键
    static bool ctrlPressed = false;
    static bool cPressed = false;
    static bool cHadPressed = false;
    static int cClicked = 0;
    if(GetKeyState(VK_CONTROL) < 0) ctrlPressed = true; //ctrl被按下
    else ctrlPressed = false;
    if(GetKeyState('C') < 0) cPressed = true; //C被按下
    else cPressed = false;

    if(!ctrlPressed) { //ctrl没有被按下则全部重置
        cPressed = false;
        cHadPressed = false;
        cClicked = 0;
    }
    else {
        if(!cHadPressed && cPressed) cHadPressed = true; //判断c被按下过几次
        else if(cHadPressed && !cPressed) {
            cClicked += 1;
            cHadPressed = false;
        }
        else if(cClicked >= 2 && cPressed) { //当按下第三次时，开始翻译
            QClipboard *clipboard = QApplication::clipboard(); //获取系统剪贴板指针
            QString originalText = clipboard->text(); //获取剪贴板上文本信息

            inputFrame->textEdit->setMarkdown(originalText);

            container->clear(); //清除之前的翻译内容
            messageBox->show();
            messageBox->setText("翻译中...");

            //查
            bool res = translate(originalText);

            if(!res) {
                messageBox->show();
                messageBox->setText("没查到，请检查拼写是否错误或者等一秒再查");
            }
            else messageBox->hide();

            //重置
            ctrlPressed = false;
            cPressed = false;
            cHadPressed = false;
            cClicked = 0;
        }
    }
}
