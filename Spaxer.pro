QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common/mainwindow.cpp \
    main-wgt/gradientbutton.cpp \
    main-wgt/mainwidget.cpp \
    main.cpp \
    search-wgt/searchwidget.cpp \
    settings-wgt/settingswidget.cpp


HEADERS += \
    common/mainwindow.h \
    main-wgt/gradientbutton.h \
    main-wgt/mainwidget.h \
    search-wgt/SearchWidget.h \
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

DISTFILES += \
    search.png

RESOURCES += \
    resources.qrc
