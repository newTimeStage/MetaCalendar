QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    c_core_metaclndr.cpp \
    c_ui_refreshthrd.cpp \
    main.cpp \
    c_form_clndr.cpp

HEADERS += \
    c_core_metaclndr.h \
    c_form_clndr.h \
    c_ui_refreshthrd.h

FORMS += \
    c_form_clndr.ui

TRANSLATIONS += \
    MetaCalendar_yue_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
