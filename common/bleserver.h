#ifndef BLESCANNER_H
#define BLESCANNER_H

#include <QObject>
#include <QLowEnergyController>
#include <QLowEnergyCharacteristic>
#include <QLowEnergyService>
#include <QLowEnergyAdvertisingData>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QBluetoothUuid>
#include <QLowEnergyDescriptorData>
#include <QLowEnergyAdvertisingParameters>
#include <QDebug>
#include <QThread>

class BleServer : public QObject {
    Q_OBJECT

public:
    explicit BleServer(QObject *parent);

private slots:
    void deviceConnected();
    void deviceDisconnected();
    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue);
    void sendNotification(const QByteArray &data);

private:
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyServiceData serviceData;     // Данные о сервисе
    QLowEnergyCharacteristicData charData;

    QString uuid;
    QString charUuid;
};




#endif // BLESCANNER_H
