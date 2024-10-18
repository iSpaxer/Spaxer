#ifndef BLUETOOTHSTANDARTITEM_H
#define BLUETOOTHSTANDARTITEM_H

#include <QBluetoothDeviceInfo>
#include <QObject>
#include <QStandardItem>

class BluetoothStandartItem : public QStandardItem {

public:
    explicit BluetoothStandartItem(const QString &text, QBluetoothDeviceInfo device);
    explicit BluetoothStandartItem(const BluetoothStandartItem* _copy);

    QBluetoothDeviceInfo getDevice() const;

private:
    QBluetoothDeviceInfo m_device;
};

#endif // BLUETOOTHSTANDARTITEM_H
