#include "bleclient.h"
#include <QThread>

BleClient::BleClient(QObject *parent): QObject(parent)/*,
    targetServiceUuid(StaticData::SERVICE_UUID)*/ {

}

void BleClient::connectToDevice(const QBluetoothDeviceInfo &device) {
    qDebug() << device.deviceUuid() << " " << device.address() << " " << device.name();
    controller = QLowEnergyController::createCentral(device, this);
    connect(controller, &QLowEnergyController::connected, this, &BleClient::deviceConnected);
    connect(controller, &QLowEnergyController::disconnected, this, &BleClient::deviceDisconnected);

    // connect(controller, &QLowEnergyController::connected, this, &BleClient::connected);
    // connect(controller, &QLowEnergyController::disconnected, this, &BleClient::disconnected);

    controller->connectToDevice();
}

void BleClient::deviceConnected() {
    qDebug() << "Connected to BLE Server!";
    QThread::sleep(2);
    // После подключения начинаем искать доступные сервисы
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &BleClient::serviceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished, this, &BleClient::serviceScanDone);
    controller->discoverServices();
}

void BleClient::serviceDiscovered(const QBluetoothUuid &uuid) {
    qDebug() << "Discovered service:" << uuid.toString();
    // Проверяем, если это нужная нам служба
    if (uuid == QBluetoothUuid("{00001802-0000-1000-8000-00805F9B34FB}")) {  // Используем тот же UUID, что и на сервере
        qDebug() << "Found matching service.";
        targetServiceUuid = uuid;

        service = controller->createServiceObject(targetServiceUuid);
        if (service) {
            connect(service, &QLowEnergyService::stateChanged, this, &BleClient::serviceStateChanged);
            connect(service, &QLowEnergyService::characteristicChanged, this, &BleClient::characteristicChanged);
            service->discoverDetails();
        }
    }
}

void BleClient::serviceScanDone() {
    qDebug() << "Service scan done.";
}

void BleClient::serviceStateChanged(QLowEnergyService::ServiceState newState) {
    if (newState == QLowEnergyService::ServiceDiscovered) {
        qDebug() << "Service details discovered.";

        // Ищем нужную характеристику по UUID
        const QLowEnergyCharacteristic characteristic = service->characteristic(QBluetoothUuid(StaticData::CHARACTERISTIC_UUID)); // Используем UUID характеристики

        if (!characteristic.isValid()) {
            qDebug() << "Characteristic not found.";
            return;
        }
        targetCharacteristic = characteristic;

        // Готовы отправить сообщение
        QByteArray message("First connect message");
        service->writeCharacteristic(targetCharacteristic, message);

        QLowEnergyDescriptor cccd = characteristic.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
        if (!cccd.isValid()) {
            qDebug() << "CCCD not found!";
            return;
        }

        // Подписываемся на уведомления, записывая 0x01 в CCCD
        service->writeDescriptor(cccd, QByteArray::fromHex("0100"));
        qDebug() << "Subscribed to notifications!";
        emit successConnect(true);
        sendMessage("Y29ubmVjdA==");
    }
}

void BleClient::sendMessage(const QByteArray &message) {
    if (service) {
        service->writeCharacteristic(targetCharacteristic, message);
    }
}

// обрабатываем полученные данные с сервера
void BleClient::characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &data) {
    qDebug() << "Получены данные с сервера:" << QString::fromUtf8(data);

    const QString dataText = QString::fromUtf8(data);
    if (!dataText.isEmpty() && !dataText.contains(QChar::ReplacementCharacter)) {
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
}

void BleClient::deviceDisconnected() {
    qDebug() << "Disconnected from BLE server.";
    emit successConnect(false);
}
