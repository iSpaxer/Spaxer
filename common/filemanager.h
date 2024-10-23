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

#include <QStandardPaths>
#include <QDir>

#include <search-wgt/bluetoothstandartitem.h>
#include <common/staticdata.h>


class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = nullptr);



    QString getDocumentsPath() {
        QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        return documentsPath;
    }

    QString getLibraryPath() {
        QString libraryPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        return libraryPath;
    }

    QString getTemporaryPath() {
        QString tempPath = QDir::tempPath();
        return tempPath;
    }

    void saveDeviceList(const QList<QBluetoothDeviceInfo> &devices) {
        QString filePath = "devices.json";
        #ifdef Q_OS_IOS
        filePath = getDocumentsPath() + "/" + filePath;
        #endif

        QFile file(filePath);
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
        QString filePath = "devices.json";
        #ifdef Q_OS_IOS
        filePath = getDocumentsPath() + "/" + filePath;
        #endif

        QFile file(filePath);
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
                    QBluetoothUuid uuid(jsonObject["uuid"].toString());
                    qDebug() <<" load: " << name << " " << address << " " << uuid.toString();

                    int id = 0;
                    if (!jsonObject["uuid"].isNull() && jsonObject["uuid"] == "{00000000-0000-0000-0000-000000000000}" && jsonObject["address"].toString().isEmpty()) {
                        qDebug() << "невозможно подключиться uuid и адресс пусты";
                        return devices;
                    }
                    qDebug() << jsonObject["uuid"].toString();
                    if (jsonObject["uuid"] == "{00000000-0000-0000-0000-000000000000}" ) {
                        qDebug() << "uuid пуст пробуем по MAC подключиться";
                        QBluetoothDeviceInfo deviceInfo(address, name, id);
                        devices.append(deviceInfo);
                        continue;
                    }
                    if (jsonObject["uuid"] != "{00000000-0000-0000-0000-000000000000}" ) {
                        qDebug() << "Пробуем uuid ";
                        QBluetoothDeviceInfo deviceInfo(uuid, name, id);
                        devices.append(deviceInfo);
                        continue;
                    }
                }
            }
        }

        return devices;
    }
signals:
};

#endif // FILEMANAGER_H
