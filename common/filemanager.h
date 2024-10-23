#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QApplication>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QDebug>

#include <search-wgt/bluetoothstandartitem.h>

class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = nullptr);

    void saveDeviceList(const QList<QBluetoothDeviceInfo> &devices) {
        QFile file("devices.json");
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Could not open file for writing:" << file.errorString();
            return;
        }

        QJsonArray jsonArray;
        for (const QBluetoothDeviceInfo &device : devices) {
            QJsonObject jsonObject;
            jsonObject["name"] = device.name();
            jsonObject["address"] = device.address().toString();
            jsonObject["uuid"] = device.deviceUuid().toString(); // Сохраняем UUID устройства
            // jsonObject["classOfDevice"] = 0; // Сохраняем класс устройства
            jsonArray.append(jsonObject);
        }

        QJsonDocument jsonDoc(jsonArray);
        file.write(jsonDoc.toJson());
        file.close();
    }

    QList<QBluetoothDeviceInfo> loadDeviceList() {
        QFile file("devices.json");
        QList<QBluetoothDeviceInfo> devices;

        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Could not open file for reading:" << file.errorString();
            return devices; // Возвращаем пустой список, если файл не открыт
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isArray()) {
            QJsonArray jsonArray = jsonDoc.array();
            for (const QJsonValue &value : jsonArray) {
                if (value.isObject()) {
                    QJsonObject jsonObject = value.toObject();
                    QString name = jsonObject["name"].toString();
                    QBluetoothAddress address(jsonObject["address"].toString());
                    QBluetoothDeviceInfo deviceInfo(address, name, 0); // Создание устройства из адреса и имени
                    devices.append(deviceInfo);
                }
            }
        }

        return devices;
    }
signals:
};

#endif // FILEMANAGER_H
