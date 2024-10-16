#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "searchitemdelegate.h"

#include <QFrame>
#include <QStandardItemModel>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>


namespace Ui {
class searchWidget;
}

class SearchWidget : public QFrame
{
    Q_OBJECT


public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

public slots:
    void activeLocalDeviceIsServer(bool isServer);
    void setConnectedDevicesToServer(QList<QString> connectedDevices);

private slots:
    void onItemClicked(const QModelIndex &index);
    void onItemDoubleCLicked(const QModelIndex &index);

    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void scanFinished();

private:
    const static QString CONNECT;
    const static QString NOT_CONNECTED;

    bool m_localDeviceIsServer;
    Ui::searchWidget *ui;
    SearchItemDelegate *m_delegate;
    QStandardItemModel *m_modelByFindedDevices;
    QStandardItemModel *m_modelByDevicesForConnection;
    QStandardItemModel *m_modelByConnectionDevice;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
    QBluetoothLocalDevice *m_localDevice;

    void initFindedDevices();
    bool checkOnUnicModel(const QStandardItemModel *model, const QString deviceBleName);
};

#endif // SEARCHWIDGET_H
