#ifndef BLECLIENT_H
#define BLECLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>
#include <QDebug>

#include <common/staticdata.h>

class BleClient : public QObject
{
    Q_OBJECT
public:
    explicit BleClient(QObject *parent = nullptr);

signals:
    void connected();
    void disconnected();

public slots:
    void connectToDevice(const QBluetoothDeviceInfo &device);
    void deviceConnected();
    void serviceDiscovered(const QBluetoothUuid &uuid);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState newState);
    void sendMessage(const QByteArray &message);
    void characteristicChanged(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);
    void deviceDisconnected();

private:
    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QBluetoothUuid targetServiceUuid;
    QLowEnergyCharacteristic targetCharacteristic;
};


#endif // BLECLIENT_H
