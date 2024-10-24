#include "bleserver.h"

#include <QImage>

BleServer::BleServer(QObject *parent): QObject(parent),
    serviceData(QLowEnergyServiceData()),
    charData(QLowEnergyCharacteristicData()),
    uuid("{00001802-0000-1000-8000-00805F9B34FB}"),
    charUuid("{00005678-0000-1000-8000-00805F9B34FB}") {
    // Создаем данные сервиса
    start();
}

void BleServer::start() {
    if (m_start) {
        m_start = false;
        qDebug() << "start Server";
        serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);

        // Задаем уникальный UUID для сервиса (можно использовать SerialPort, но лучше задать свой)
        serviceData.setUuid(QBluetoothUuid(uuid)); // Пример уникального UUID сервиса

        // Создаем данные для характеристики
        charData.setUuid(QBluetoothUuid(charUuid)); // Пример уникального UUID характеристики
        charData.setProperties(QLowEnergyCharacteristic::Indicate | QLowEnergyCharacteristic::Write);
        charData.setValue(QByteArray(2, 0)); // Стартовое значение

        // Добавляем дескриптор CCCD для уведомлений и индикаций
        // QLowEnergyDescriptorData clientConfig(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration, QByteArray(2, 0));
        // charData.addDescriptor(clientConfig);

        // Добавляем характеристику в сервис
        serviceData.addCharacteristic(charData);

        // Создаем контроллер BLE и сервис
        controller = QLowEnergyController::createPeripheral(this);
        service = controller->addService(serviceData);

        // Настройка рекламных данных
        QLowEnergyAdvertisingData advertisingData;
        advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
        advertisingData.setIncludePowerLevel(true);
        advertisingData.setLocalName("BLE_Server");
        advertisingData.setServices(QList<QBluetoothUuid>() << QBluetoothUuid(uuid)); // Тот же UUID сервиса хз но можно и неодинаково

        // Подключение сигналов
        connect(controller, &QLowEnergyController::connected, this, &BleServer::deviceConnected);
        connect(controller, &QLowEnergyController::disconnected, this, &BleServer::deviceDisconnected);
        connect(service, &QLowEnergyService::characteristicChanged, this, &BleServer::characteristicChanged);

        // Запуск рекламы
        controller->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData, advertisingData);
        qDebug() << "Advertising started!";
    }

}

void BleServer::deviceConnected() {
    qDebug() << "Client connected!";
    emit successConnect(true);
    QThread a;
    a.sleep(1);
    sendNotification("Send from server");
}

void BleServer::deviceDisconnected() {
    qDebug() << "Client disconnected!";
    emit successConnect(false);
}

// данные от клиента
void BleServer::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &data) {
    qDebug() << "Received data from client:" << data;

    const QString dataText = QString::fromUtf8(data);
    if (!dataText.isEmpty() && !dataText.contains(QChar::ReplacementCharacter)) {
        // Это текст, закодированный в UTF-8
        qDebug() << "QByteArray содержит текст: " << dataText;
        if (dataText == "Y29ubmVjdA==") {
            emit successConnect(true);
            return;
        } if (dataText == "ZGlzY29ubmVjdA==") {
            emit successConnect(false);
            return;
        }
        emit getText(dataText);
        return;
    }

    QImage image;
    if (image.loadFromData(data)) {
        // Это изображение
        qDebug() << "QByteArray содержит изображение";
        emit getImage(image);
        return;
    }

    else {
        qDebug() << "QByteArray содержит неизвестный файл";
        emit getData(data);
    }
}

// отправляем клиенту
void BleServer::sendNotification(const QByteArray &data) {
    // Отправляем индикацию
    const QLowEnergyCharacteristic characteristic = service->characteristic(QBluetoothUuid(charUuid)); // Замените на ваш UUID характеристики

    service->writeCharacteristic(characteristic, data);
    qDebug() << "Notification byte sent:" << data;
}

void BleServer::sendNotificationStr(const QString &data) {
    const QLowEnergyCharacteristic characteristic = service->characteristic(QBluetoothUuid(charUuid));
    if (characteristic.isValid()) {
        service->writeCharacteristic(characteristic, data.toUtf8());
        qDebug() << "Notification sent:" << data;
    } else {
        qDebug() << "Invalid characteristic!";
    }
    // const QLowEnergyCharacteristic characteristic = service->characteristic(QBluetoothUuid(charUuid)); // Замените на ваш UUID характеристики

    // if (!characteristic.isValid()) {
    //     qDebug() << "Invalid characteristic!";
    //     // return;
    // }

    // // Проверяем, что дескриптор для уведомлений установлен
    // const QLowEnergyDescriptor notificationDesc = characteristic.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
    // if (!notificationDesc.isValid()) {
    //     qDebug() << "Notification descriptor not found!";
    //     // return;
    // }

    // // Проверяем, что клиент подписан на уведомления
    // if (notificationDesc.value() == QByteArray::fromHex("0000")) {
    //     qDebug() << "Client is not subscribed to notifications!";
    //     // return;
    // }

    // QByteArray dataToSend = QByteArray::fromHex(data.toStdString().c_str());
    // service->writeCharacteristic(characteristic, dataToSend);
    // // QString text2 = "heh";
    // // service->writeCharacteristic(characteristic, text2.toUtf8());
    // // qDebug() << "NotificationStr sent:" << text2;
}
