#include "common/mainwindow.h"
#include <QApplication>

#include <QBluetoothPermission>
#include <QCoreApplication>
#include <QPermission>
// #include <QPlatformNativeInterface>

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

void configureWindowForFullscreen() {
    UIWindow *window = [UIApplication sharedApplication].keyWindow;
    if (@available(iOS 11.0, *)) {
        window.rootViewController.view.insetsLayoutMarginsFromSafeArea = NO;
        window.rootViewController.additionalSafeAreaInsets = UIEdgeInsetsZero;
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
    #ifdef Q_OS_IOS
    requestBluetoothPermission(app);
    preventScreenLock();
    // configureWindowForFullscreen();
    #endif

    MainWindow w;
    w.show();
    return app.exec();
}
