#include "bleserver.h"

BleServer::BleServer(QObject *parent): QObject(parent),
    serviceData(QLowEnergyServiceData()),
    charData(QLowEnergyCharacteristicData()),
    uuid("{00001802-0000-1000-8000-00805F9B34FB}"),
    charUuid("{00005678-0000-1000-8000-00805F9B34FB}") {
    // Создаем данные сервиса
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);

    // Задаем уникальный UUID для сервиса (можно использовать SerialPort, но лучше задать свой)
    serviceData.setUuid(QBluetoothUuid(uuid)); // Пример уникального UUID сервиса

    // Создаем данные для характеристики
    charData.setUuid(QBluetoothUuid(charUuid)); // Пример уникального UUID характеристики
    charData.setProperties(QLowEnergyCharacteristic::Indicate | QLowEnergyCharacteristic::Write);
    charData.setValue(QByteArray(2, 0)); // Стартовое значение

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
    // advertisingData.setLocalName("BLE_Server");
    advertisingData.setServices(QList<QBluetoothUuid>() << QBluetoothUuid(uuid)); // Тот же UUID сервиса хз но можно и неодинаково

    // Подключение сигналов
    connect(controller, &QLowEnergyController::connected, this, &BleServer::deviceConnected);
    connect(controller, &QLowEnergyController::disconnected, this, &BleServer::deviceDisconnected);
    connect(service, &QLowEnergyService::characteristicChanged, this, &BleServer::characteristicChanged);

    // Запуск рекламы
    controller->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData, advertisingData);
    qDebug() << "Advertising started!";
}

void BleServer::deviceConnected() {
    qDebug() << "Client connected!";
    QThread a;
    a.sleep(1);
    sendNotification("Send from server");
}

void BleServer::deviceDisconnected() {
    qDebug() << "Client disconnected!";
}

void BleServer::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue) {
    qDebug() << "Received data from client:" << newValue;
    // Здесь можно обработать данные, пришедшие от клиента
}

void BleServer::sendNotification(const QByteArray &data) {
    // Отправляем индикацию
    const QLowEnergyCharacteristic characteristic = service->characteristic(QBluetoothUuid(charUuid)); // Замените на ваш UUID характеристики

    QByteArray valueToSend = QByteArray::fromHex("01");
    service->writeCharacteristic(characteristic, data);
    qDebug() << "Notification sent:" << data;
}
