// #include "common/mainwindow.h"
#include <QApplication>

#include <QBluetoothPermission>
#include <QCoreApplication>
#include <QPermission>

#include <common/mainwindow.h>

// Включите необходимый заголовок для работы с iOS
#ifdef Q_OS_IOS
#import <UIKit/UIKit.h>

// Функция для предотвращения отключения экрана
void preventScreenLock() {
    [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
}

// Функция для разрешения отключения экрана
void allowScreenLock() {
    [[UIApplication sharedApplication] setIdleTimerDisabled:NO];
}
#endif

#ifdef Q_OS_LINUX
void setupDesktopEntry() {
    // Путь к файлу .desktop
    QString desktopFilePath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + "/spaxer.desktop";

    // Проверяем, если файл уже существует, то выходим
    if (QFile::exists(desktopFilePath)) {
        qDebug() << "Desktop entry already exists.";
        return;
    }

    QString desktopFileContent =
R"([Desktop Entry]
Name=Spaxer
Exec=/home/alex/CProject/clone/Spaxer/build/Desktop_Qt_6_7_2-Release/Spaxer
Icon=/home/alex/CProject/clone/Spaxer/AppIcons/spaxer_icon.png
Type=Application
Categories=Utility;)";

    // Сохраняем .desktop файл
    QFile desktopFile(desktopFilePath);
    if (desktopFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        desktopFile.write(desktopFileContent.toUtf8());
        desktopFile.close();
        qDebug() << "Desktop entry created at:" << desktopFilePath;
    } else {
        qWarning() << "Failed to create desktop entry.";
    }
}

#endif

void requestBluetoothPermission(QApplication &app) {
    // Создание объекта разрешений для Bluetooth
    QBluetoothPermission permission;

    // Проверка текущего статуса разрешения
    auto status = app.checkPermission(permission);

    if (status == Qt::PermissionStatus::Undetermined) {
        // Запрос разрешения у пользователя
        app.requestPermission(permission, [](const QPermission &permission){
            if (permission.status() == Qt::PermissionStatus::Granted) {
                qDebug() << "Bluetooth разрешение предоставлено.";
            } else {
                qDebug() << "Bluetooth разрешение отклонено.";
            }
        });
    } else if (status == Qt::PermissionStatus::Granted) {
        qDebug() << "Bluetooth уже разрешен.";
    } else {
        qDebug() << "Bluetooth разрешение отклонено.";
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/AppIcons/spaxer_icon.png"));

#ifdef Q_OS_LINUX
    // устанавливаем иконку
    setupDesktopEntry();
#endif
    #ifdef Q_OS_IOS
    requestBluetoothPermission(app);
    preventScreenLock();
    #endif

    MainWindow w;
    w.show();
    return app.exec();
}
