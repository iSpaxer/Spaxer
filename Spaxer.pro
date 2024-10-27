QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    common/bleserver.cpp \
    common/clipboardmonitor.cpp \
    common/filemanager.cpp \
    common/mainwindow.cpp \
    common/staticdata.cpp \
    main-wgt/gradientbutton.cpp \
    main-wgt/mainwidget.cpp \
    main.cpp \
    search-wgt/bleclient.cpp \
    search-wgt/bluetoothstandartitem.cpp \
    search-wgt/searchitemdelegate.cpp \
    search-wgt/searchwidget.cpp \
    settings-wgt/settingswidget.cpp


HEADERS += \
    common/bleserver.h \
    common/clipboardmonitor.h \
    common/filemanager.h \
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

linux {
    DESKTOP_FILE.source = $$PWD/spaxer.desktop
    DESKTOP_FILE.path = $$[QT_INSTALL_PREFIX]/../share/applications
    INSTALLS += DESKTOP_FILE
    DESKTOP_FILE.path = $$HOME/.local/share/applications

    source_file = $$PWD/spaxer_create_deb.sh
    target_dir = $$OUT_PWD
    QMAKE_POST_LINK += cp $$source_file $$target_dir
}

ios {
    QMAKE_CXXFLAGS += -fobjc-arc  # Включаем ARC (Automatic Reference Counting) для Objective-C++
    QMAKE_INFO_PLIST = $$PWD/macOs/Info.plist
    QMAKE_TARGET_BUNDLE_IDENTIFIER = com.mycompany.spaxer
    LIBS += -framework CoreBluetooth
}

macx {
    QMAKE_CXXFLAGS += -fobjc-arc  # Включаем ARC (Automatic Reference Counting) для Objective-C++
    QMAKE_INFO_PLIST = $$PWD/macOs/Info.plist
    LIBS += -framework CoreBluetooth -framework IOBluetooth
}

DISTFILES += \
    macOs/Info.plist \
    search.png \
    spaxer.desktop \
    spaxer_create_deb.sh

RESOURCES += \
    resources.qrc
