#!/bin/bash

# Параметры пакета
DEB_PACKAGE_NAME="untitled"
DEB_VERSION="1.0.0"
DEB_ARCH="all"  # Укажите 'all' для платформонезависимого пакета
DEB_MAINTAINER="Your Name <your.email@example.com>"
DEB_DESCRIPTION="Brief description of untitled.\nLong description of the untitled application."
DESTDIR="build"

# Удаляем предыдущие сборки
rm -rf $DESTDIR
mkdir -p $DESTDIR/DEBIAN
mkdir -p $DESTDIR/usr/bin

# Компиляция приложения
# make

# Проверка, что скомпилированный файл существует
if [ -f "$PWD/build/Desktop_Qt_6_7_2-Debug/untitled" ]; then
    # Копирование исполняемого файла в директорию пакета
    cp "$PWD/build/Desktop_Qt_6_7_2-Debug/untitled" "$DESTDIR/usr/bin/"
    
    # Создание файла управления пакетом (control)
    echo "Package: $DEB_PACKAGE_NAME" > "$DESTDIR/DEBIAN/control"
    echo "Version: $DEB_VERSION" >> "$DESTDIR/DEBIAN/control"
    echo "Section: utils" >> "$DESTDIR/DEBIAN/control"
    echo "Priority: optional" >> "$DESTDIR/DEBIAN/control"
    echo "Architecture: $DEB_ARCH" >> "$DESTDIR/DEBIAN/control"
    echo "Maintainer: $DEB_MAINTAINER" >> "$DESTDIR/DEBIAN/control"
    echo -e "Description: $DEB_DESCRIPTION" >> "$DESTDIR/DEBIAN/control"

    # Построение пакета .deb
    dpkg-deb --build "$DESTDIR" "${DEB_PACKAGE_NAME}-${DEB_VERSION}.deb"
    
    echo "Debian package created: ${DEB_PACKAGE_NAME}-${DEB_VERSION}.deb"
else
    echo "Compilation failed. Make sure your project compiles correctly."
fi