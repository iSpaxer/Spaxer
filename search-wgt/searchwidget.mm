#include "bluetoothstandartitem.h"
#include "searchwidget.h"
#include "ui_searchwidget.h"

#include <QScroller>
#include <QEasingCurve>
#include <IOBluetooth/IOBluetooth.h>

const QString SearchWidget::CONNECT = "Подключено";
const QString SearchWidget::NOT_CONNECTED = "Не подключено";

SearchWidget::SearchWidget(QWidget *parent):
    QFrame(parent),
    ui(new Ui::searchWidget),
    m_delegate(new SearchItemDelegate(ui->findedDevicesView)),
    // m_(nullptr),
    m_modelByFindedDevices(new QStandardItemModel(this)),
    m_modelByDevicesForConnection(new QStandardItemModel(this)),
    m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)) {
    ui->setupUi(this);

    initFindedDevices();
    updateConnectedDevices();

    ui->devicesForConnection->setItemDelegate(m_delegate);
    ui->devicesForConnection->setModel(m_modelByDevicesForConnection);

    connect(ui->doFindButton, &QPushButton::clicked, [this](bool checked) {
        m_discoveryAgent->start();
        m_modelByFindedDevices->clear();
        ui->doFindButton->setText("Поиск устройств...");
        updateConnectedDevices();
    });

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &SearchWidget::deviceDiscovered);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &SearchWidget::scanFinished);

}

void SearchWidget::initFindedDevices() {
    ui->findedDevicesView->setItemDelegate(m_delegate);
    ui->findedDevicesView->setModel(m_modelByFindedDevices);

    QScroller::grabGesture(ui->findedDevicesView, QScroller::LeftMouseButtonGesture);

    QScroller *scroller = QScroller::scroller(ui->findedDevicesView);
    QScrollerProperties props;

    // Настройки плавного скроллинга с инерцией
    props.setScrollMetric(QScrollerProperties::ScrollingCurve, QEasingCurve(QEasingCurve::InOutQuad));
    props.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.05);  // Плавное замедление
    props.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.5);      // Ограничение максимальной скорости
    props.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.3);  // Инерция за пределы

    // Применяем настройки
    scroller->setScrollerProperties(props);

    connect(ui->findedDevicesView, &QListView::clicked, this, &SearchWidget::onItemClicked);
    connect(ui->devicesForConnection, &QListView::activated, this, &SearchWidget::onItemDoubleCLicked);
}

SearchWidget::~SearchWidget() {
    delete ui;
}

/*
 * Остановился на том, чтобы в BluetoothStandartItem хранить QBluetoothDeviceInfo,
 * чтобы потом доставать их и с ними пробовать подключиться к устройству через кнопку
 * в главном меню. Скорее всего нужно будет сначала протестировать чтобы Линукс был
 * сервером, я был заранее к нему подключен (чтобы по быстрее было)
 * Сначала нужно будет добавить его в подключаемы потом попробовать подключиться
 *
 */

void SearchWidget::connectToDevices() {
  for (int row = 0; row < m_modelByDevicesForConnection->rowCount(); ++row) {
      BluetoothStandartItem* item = dynamic_cast<BluetoothStandartItem*>(m_modelByDevicesForConnection->item(row));
      QString deviceName = item->text();
  }
}

void SearchWidget::onItemClicked(const QModelIndex &index) {
    // Извлекаем элемент по индексу
    if (!m_localDeviceIsServer) {
        QString deviceBleName = index.data(Qt::DisplayRole).toString();
        QString connect = index.data(Qt::UserRole + 1).toString();
        QString notConnect = index.data(Qt::UserRole + 2).toString();
        qDebug() << "Clicked item custom data:" << deviceBleName << " " << connect << notConnect;

        if (checkOnUnicModel(m_modelByDevicesForConnection, deviceBleName)) {

            qDebug() << connect.isEmpty() << " " << connect;

            QStandardItem *item = new QStandardItem(deviceBleName);
            if (connect.isEmpty())
                item->setData(NOT_CONNECTED, Qt::UserRole + 2);
            else
                item->setData(CONNECT, Qt::UserRole + 1);
            m_modelByDevicesForConnection->appendRow(item);
        }
    }
}

void SearchWidget::onItemDoubleCLicked(const QModelIndex &index) {
    if (!m_localDeviceIsServer) {
        qDebug() << "Отключение устройств не реализовано";
        m_modelByDevicesForConnection->removeRow(index.row());
    }
}

void SearchWidget::deviceDiscovered(const QBluetoothDeviceInfo &device) {
  if (!device.name().isEmpty() && device.name() != device.address().toString()) {

    BluetoothStandartItem *item1 = new BluetoothStandartItem(device.name(), new QBluetoothDeviceInfo(device));
    item1->setData(NOT_CONNECTED, Qt::UserRole + 2);
    m_modelByFindedDevices->appendRow(item1);\
  }
}

void SearchWidget::scanFinished() {
  ui->doFindButton->setText("Начать поиск");
}

void SearchWidget::activeLocalDeviceIsServer(bool isServer) {
    m_localDeviceIsServer = isServer;
    if (isServer) {
        if (!m_modelByConnectionDevice) m_modelByConnectionDevice = new QStandardItemModel(this);
        ui->nameForActiveLocalDevice->setText("Подключенные устройства к серверу");
        ui->devicesForConnection->setModel(m_modelByConnectionDevice);
    } else {
        ui->nameForActiveLocalDevice->setText(" Устройства для подключения");
        ui->devicesForConnection->setModel(m_modelByDevicesForConnection);
    }
}

// todo ?
void SearchWidget::setConnectedDevicesToServer(QList<QString> connectedDevices) {
    if (!m_modelByConnectionDevice) m_modelByConnectionDevice = new QStandardItemModel(this);
    m_modelByConnectionDevice->clear();
    for (auto device: connectedDevices) {
        QStandardItem *item = new QStandardItem(device);
        m_modelByConnectionDevice->appendRow(item);
    }
}

void SearchWidget::updateConnectedDevices() {
  #ifdef Q_OS_MAC
    NSArray *connectedDevices = [IOBluetoothDevice pairedDevices];
        for (IOBluetoothDevice *device in connectedDevices) {
            if ([device isConnected]) {
              BluetoothClassOfDevice classOfDevice = [device classOfDevice];
              quint32 deviceClassAsQuint32 = (quint32)classOfDevice;
              QString deviceName = QString::fromNSString([device name]);
              QString deviceAddress = QString::fromNSString([device addressString]);
              qDebug() << "quint32: " << deviceClassAsQuint32 << " deviceName: " << deviceName << " deviceAddress: " << deviceAddress;
              if (checkOnUnicModel(m_modelByFindedDevices, deviceName)) {
                BluetoothStandartItem *item = new BluetoothStandartItem(deviceName, new QBluetoothDeviceInfo(QBluetoothAddress(deviceName), deviceAddress, deviceClassAsQuint32));
                item->setData(CONNECT, Qt::UserRole + 1);
                m_modelByFindedDevices->appendRow(item);
              }
            }
        }
  #else
// todo достать q32
      // QList<QBluetoothAddress> connectedDevices = m_localDevice->connectedDevices();
      // QObject::connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
      //                  [connectedDevices, this](const QBluetoothDeviceInfo &deviceInfo) {
      //                      // Проверяем, является ли найденное устройство подключённым
      //                      if (connectedDevices.contains(deviceInfo.address())) {
      //                          // Выводим имя и адрес подключённого устройства
      //                         qDebug() << "Подключённое устройство:" << deviceInfo.name() << ", адрес:" << deviceInfo.address().toString();
      //                         QString deviceName = deviceInfo.name();
      //                         if (checkOnUnicModel(m_modelByFindedDevices, deviceName)) {
      //                           QStandardItem *item = new QStandardItem(deviceName);
      //                           item->setData(CONNECT, Qt::UserRole + 1);
      //                           m_modelByFindedDevices->appendRow(item);
      //                         }
      //                       }
      //                  });
  #endif
}


bool SearchWidget::checkOnUnicModel(const QStandardItemModel *model, const QString deviceBleName) {
    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem* item = model->item(row);
        if (item->text() == deviceBleName) {
            qDebug() <<  "Элемент с таким значением уже существует"; // todo вывести на экран?
            return false;
        }
    }
    return true;
}
