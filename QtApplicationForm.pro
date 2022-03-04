QT       += core gui
QT      += serialport
QT      += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    qtapp.cpp

HEADERS += \
    qtapp.h

FORMS += \
    qtapp.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Pictures/ComPort.png \
    Pictures/Qt_logo_2016.svg.png \
    Pictures/logoCPP.png \
    Pictures/logoVTX.jpg \
    Pictures/multithread.jpg \
    Pictures/tcpip.jpg \
    Pictures/udp.jpg

RC_ICONS = Pictures/app.ico
