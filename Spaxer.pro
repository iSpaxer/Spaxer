QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    # bluetooth/bleclient.cpp \
    common/bleserver.cpp \
    common/clipboardmonitor.cpp \
    common/mainwindow.cpp \
    common/staticdata.cpp \
    main-wgt/gradientbutton.cpp \
    main-wgt/mainwidget.cpp \
    main.cpp \
    search-wgt/bleclient.cpp \
    search-wgt/bluetoothstandartitem.cpp \
    search-wgt/searchitemdelegate.cpp \
    search-wgt/searchwidget.mm \
    settings-wgt/settingswidget.cpp


HEADERS += \
    # bluetooth/bleclient.h \
    common/bleserver.h \
    common/clipboardmonitor.h \
    common/mainwindow.h \
    common/staticdata.h \
    main-wgt/gradientbutton.h \
    main-wgt/mainwidget.h \
    search-wgt/SearchWidget.h \
    search-wgt/bleclient.h \
    search-wgt/bluetoothstandartitem.h \
    search-wgt/searchitemdelegate.h \
    settings-wgt/settingswidget.h


FORMS += \
    common/mainwindow.ui \
    main-wgt/mainwidget.ui \
    search-wgt/searchwidget.ui \
    settings-wgt/settingswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx {
    QMAKE_INFO_PLIST = $$PWD/macOs/Info.plist

    LIBS += -framework IOBluetooth
}

DISTFILES += \
    macOs/Info.plist \
    search.png

RESOURCES += \
    resources.qrc
