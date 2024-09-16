QT       += core gui sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ExportExcel.cpp \
    FunctionButton.cpp \
    HelpDocument.cpp \
    MySqlite.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ExportExcel.h \
    FunctionButton.h \
    Headerfiles.h \
    HelpDocument.h \
    MySqlite.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    icon.qrc

RC_FILE += logo.rc

DISTFILES += \
    icon/goods.png \
    icon/home.png \
    icon/home1.png \
    icon/income.png \
    icon/logo.ico \
    icon/logo.png \
    icon/profit.png \
    icon/stock.png \
    logo.ico \
    logo.png \
    logo.rc
