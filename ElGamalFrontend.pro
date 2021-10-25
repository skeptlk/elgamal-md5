QT       += core gui
QT       += designer


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/elgamal.cpp \
    core/md5.cpp \
    main.cpp \
    mainwindow.cpp \
    stepwidget.cpp

HEADERS += \
    core/InfInt.h \
    core/elgamal.h \
    core/md5.h \
    mainwindow.h \
    stepwidget.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    ElGamalFrontend_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target