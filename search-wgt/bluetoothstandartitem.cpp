#include "bluetoothstandartitem.h"

BluetoothStandartItem::BluetoothStandartItem(const QString &text, QBluetoothDeviceInfo device):
    QStandardItem(text),
    m_device(device) {

}

// утечка памяти
BluetoothStandartItem::BluetoothStandartItem(const BluetoothStandartItem *_copy):
    QStandardItem(_copy->text()),
    m_device(_copy->getDevice()) {

}

QBluetoothDeviceInfo BluetoothStandartItem::getDevice() const {
    return m_device;
}
