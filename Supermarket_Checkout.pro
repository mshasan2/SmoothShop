QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/addeditwindow.cpp \
    src/checkoutwindow.cpp \
    src/landingpage.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/viewitemswindow.cpp

HEADERS += \
    src/addeditwindow.h \
    src/checkoutwindow.h \
    src/landingpage.h \
    src/mainwindow.h \
    src/viewitemswindow.h

FORMS += \
    src/addeditwindow.ui \
    src/checkoutwindow.ui \
    src/landingpage.ui \
    src/mainwindow.ui \
    src/viewitemswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md

RESOURCES += \
    resources.qrc
