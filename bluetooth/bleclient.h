#ifndef BLECLIENT_H
#define BLECLIENT_H

#include <QCoreApplication>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>
#include <QDebug>

class BleClient : public QObject {
    Q_OBJECT

public:
    BleClient() {
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BleClient::deviceDiscovered);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BleClient::scanFinished);
    }

    void startScan() {
        qDebug() << "Starting scan...";
        discoveryAgent->start();
    }

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device) {
        // qDebug() << "Found BLE Server:" << device.name() << device.address().toString();
        if (device.name() == "MacBook Pro — Александр") {  // Убедитесь, что имя совпадает с тем, что указано в сервере
            qDebug() << "---Found BLE Server:" << device.name() << device.address().toString();
            controller = QLowEnergyController::createCentral(device, this);
            connect(controller, &QLowEnergyController::connected, this, &BleClient::deviceConnected);
            connect(controller, &QLowEnergyController::disconnected, this, &BleClient::deviceDisconnected);
            controller->connectToDevice();
        }
    }

    void scanFinished() {
        qDebug() << "Scan finished.";
    }

    void deviceConnected() {
        qDebug() << "Connected to BLE Server!";
        // После подключения начинаем искать доступные сервисы
        connect(controller, &QLowEnergyController::serviceDiscovered, this, &BleClient::serviceDiscovered);
        connect(controller, &QLowEnergyController::discoveryFinished, this, &BleClient::serviceScanDone);
        controller->discoverServices();
    }

    void serviceDiscovered(const QBluetoothUuid &uuid) {
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

    void serviceScanDone() {
        qDebug() << "Service scan done.";
    }

    void serviceStateChanged(QLowEnergyService::ServiceState newState) {
        if (newState == QLowEnergyService::ServiceDiscovered) {
            qDebug() << "Service details discovered.";

            // Ищем нужную характеристику по UUID
            const QLowEnergyCharacteristic characteristic = service->characteristic(QBluetoothUuid("{00005678-0000-1000-8000-00805F9B34FB}")); // Используем UUID характеристики
            if (!characteristic.isValid()) {
                qDebug() << "Characteristic not found.";
                return;
            }
            targetCharacteristic = characteristic;

            // Готовы отправить сообщение
            QByteArray message("Hello from client!");
            service->writeCharacteristic(targetCharacteristic, message);

            QLowEnergyDescriptor cccd = characteristic.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration);
            if (!cccd.isValid()) {
                qDebug() << "CCCD not found!";
                return;
            }

            // Подписываемся на уведомления, записывая 0x01 в CCCD
            service->writeDescriptor(cccd, QByteArray::fromHex("0100"));
            qDebug() << "Subscribed to notifications!";


        }
    }

    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value) {
        qDebug() << "Received confirmation from server:" << QString::fromUtf8(value);
    }

    void deviceDisconnected() {
        qDebug() << "Disconnected from BLE server.";
    }

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QBluetoothUuid targetServiceUuid;
    QLowEnergyCharacteristic targetCharacteristic;
};

#endif // BLECLIENT_H
