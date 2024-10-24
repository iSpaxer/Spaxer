#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "search-wgt/bleclient.h"
#include "common/bleserver.h"
#include "searchitemdelegate.h"

#include <QFrame>
#include <QStandardItemModel>
#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QPointer>

#include <common/clipboardmonitor.h>
#include <common/filemanager.h>


namespace Ui {
class searchWidget;
}

class SearchWidget : public QFrame {
    Q_OBJECT


public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

public slots:
    void enableServer(bool isEnable);
    void connectToDevices();
    void updateConnectedDevices();
    void activeLocalDeviceIsServer(bool isServer);
    void connectedBluetooth();
    void disconnectedBluetooth();
    void sendMessage(const QByteArray &message);
    void sendMessageStr(const QString &message);
    void sendMessageImage(const QImage &image);

private slots:
    void onItemClicked(const QModelIndex &index);
    void onItemDoubleCLicked(const QModelIndex &index);

    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanFinished();

signals:
    void successConnect(bool success);

private:
    const static QString CONNECT;
    const static QString NOT_CONNECTED;

    bool m_serverIsEnable;
    bool m_localDeviceIsServer;
    Ui::searchWidget *ui;
    SearchItemDelegate *m_delegate;
    QStandardItemModel *m_modelByFindedDevices;
    QStandardItemModel *m_modelByDevicesForConnection;
    QStandardItemModel *m_modelByConnectionDevice;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;

    FileManager *m_fileManager;
    QPointer<BleClient> m_bleClient;
    QPointer<BleServer> m_bleServer;
    ClipboardMonitor *m_clibBoardMonitor;
    // QBluetoothLocalDevice *m_localDevice;

    QLowEnergyController* m_controller;

    void createBleClient();
    void createBleServer();
    void initFindedDevices();
    void updateVisibility();
    bool checkOnUnicModel(const QStandardItemModel *model, const QString deviceBleName);
    QByteArray convertImageToByteArray(const QImage &image);
};

#endif // SEARCHWIDGET_H
