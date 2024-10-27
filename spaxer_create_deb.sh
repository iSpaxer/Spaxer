#!/bin/bash

# Параметры пакета
DEB_PACKAGE_NAME="Spaxer"
DEB_VERSION="1.0.0"
DEB_ARCH="all"  # Укажите 'all' для платформонезависимого пакета
DEB_MAINTAINER="Alexandr Sybachin <a.v.sybahcin@mail.ru>"
DEB_DESCRIPTION="Application for transferring copied text."
DESTDIR="deb"

# Удаляем предыдущие сборки
rm -rf $DESTDIR
mkdir -p $DESTDIR/DEBIAN
mkdir -p $DESTDIR/usr/bin

# Компиляция приложения
# make

# Проверка, что скомпилированный файл существует
if [ -f "$PWD/Spaxer" ]; then
    # Копирование исполняемого файла в директорию пакета
    cp "$PWD/Spaxer" "$DESTDIR/usr/bin/"
    
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