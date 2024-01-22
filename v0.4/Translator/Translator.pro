QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    inputframe.cpp \
    inputframebutton.cpp \
    languagebutton.cpp \
    main.cpp \
    myedit.cpp \
    translatecontainer.cpp \
    widget.cpp \
    PipeSender.cpp \
    wordcontent.cpp \
    wordheader.cpp \
    wordtranslate.cpp \
    wordtype.cpp

HEADERS += \
    inputframe.h \
    inputframebutton.h \
    languagebutton.h \
    myedit.h \
    translatecontainer.h \
    widget.h \
    PipeSender.h \
    wordcontent.h \
    wordheader.h \
    wordtranslate.h \
    wordtype.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

RC_ICONS = icon.ico #程序图标
