#include "bluetoothstandartitem.h"

BluetoothStandartItem::BluetoothStandartItem(QString text, QBluetoothDeviceInfo *device):
    QStandardItem(text),
    m_device(device) {

}

QBluetoothDeviceInfo *BluetoothStandartItem::getDevice() const {
    return m_device;
}
