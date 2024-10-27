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
    common/filemanager.cpp \
    common/mainwindow.cpp \
    common/staticdata.cpp \
    main-wgt/gradientbutton.cpp \
    main-wgt/mainwidget.cpp \
    main.mm \
    search-wgt/bleclient.cpp \
    search-wgt/bluetoothstandartitem.cpp \
    search-wgt/searchitemdelegate.cpp \
    search-wgt/searchwidget.mm \
    settings-wgt/settingswidget.cpp


HEADERS += \
    # bluetooth/bleclient.h \
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

# QMAKE_INFO_PLIST = $$PWD/macOs/Info.plist

# icons_folder = $$PWD/AppIcons
ios {
    QMAKE_INFO_PLIST = ios/Info.plist
    QMAKE_BUNDLE_DATA += AppIcons/Assets.xcassets
    QMAKE_ASSET_CATALOGS = AppIcons/Assets.xcassets


    QT += gui-private
    QMAKE_CXXFLAGS += -fobjc-arc  # Включаем ARC (Automatic Reference Counting) для Objective-C++
    QMAKE_INFO_PLIST = $$PWD/macOs/Info.plist
    QMAKE_TARGET_BUNDLE_IDENTIFIER = com.mycompany.spaxer
    LIBS += -framework CoreBluetooth
}
# LIBS += -framework IOBluetooth

macx {
    # QMAKE_BUNDLE_DATA += AppIcons.xcassets

# Копируем .xca
    # QMAKE_BUNDLE_DATA += AppIconDir
    # ICON_DIR = $$PWD/AppIcons/Assets.xcassets
    # QMAKE_POST_LINK += xcrun actool --output-format human-readable-text --notices --warnings --platform macosx --compile \"$${OUT_PWD}/$${TARGET}.app/Contents/Resources\" \"$${ICON_DIR}\"

    # ICON = spaxer_icon.icns
    QMAKE_CXXFLAGS += -fobjc-arc  # Включаем ARC (Automatic Reference Counting) для Objective-C++
    QMAKE_INFO_PLIST = $$PWD/macOs/Info.plist
    LIBS += -framework CoreBluetooth -framework IOBluetooth
}

DISTFILES += \
    macOs/Info.plist \
    search.png \

RESOURCES += \
    resources.qrc

# Определите правило копирования
# QMAKE_POST_LINK += cp -r $$source_folder $$build_folder
