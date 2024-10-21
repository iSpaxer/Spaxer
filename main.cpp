#include "common/mainwindow.h"
#include <QApplication>

#include <QBluetoothPermission>
#include <QCoreApplication>
#include <QPermission>

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

    #ifdef Q_OS_IOS
    requestBluetoothPermission(app);
    #endif

    MainWindow w;
    w.show();
    return app.exec();
}
