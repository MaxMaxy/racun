#-------------------------------------------------
#
# Project created by QtCreator 2018-01-30T18:55:17
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Racun
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    novracun.cpp \
    dodajpodjetje.cpp \
    dodajprodukt.cpp \
    oprogramu.cpp \
    numofitems.cpp \
    login.cpp \
    arhiv.cpp \
    terjatve.cpp \
    vnosobveznosti.cpp \
    placiloracuna.cpp \
    settings.cpp

HEADERS += \
        mainwindow.h \
    novracun.h \
    dodajpodjetje.h \
    dodajprodukt.h \
    oprogramu.h \
    numofitems.h \
    login.h \
    arhiv.h \
    terjatve.h \
    vnosobveznosti.h \
    placiloracuna.h \
    settings.h

FORMS += \
        mainwindow.ui \
    novracun.ui \
    dodajpodjetje.ui \
    dodajprodukt.ui \
    oprogramu.ui \
    numofitems.ui \
    login.ui \
    arhiv.ui \
    terjatve.ui \
    vnosobveznosti.ui \
    placiloracuna.ui \
    settings.ui

RESOURCES += \
    files.qrc

DISTFILES += \
    arhiv_files.txt \
    arhiv_login.txt \
    arhiv_novRacun.txt \
    arhiv_produkti.txt \
    arhiv_stRacuna.txt \
    arhiv_stranke.txt \
    arhiv_upniki.txt \
    arhiv_upnikiSeznam.txt \
    login.txt \
    num.txt \
    test.txt
