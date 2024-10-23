#include "bluetoothstandartitem.h"
#include "searchwidget.h"
#include "ui_searchwidget.h"

#include <QScroller>
#include <QEasingCurve>
#include <QBuffer>

// #include <IOBluetooth/IOBluetooth.h>

const QString SearchWidget::CONNECT = "Подключено";
const QString SearchWidget::NOT_CONNECTED = "Не подключено";

SearchWidget::SearchWidget(QWidget *parent):
    QFrame(parent),
    ui(new Ui::searchWidget),
    m_delegate(new SearchItemDelegate(ui->findedDevicesView)),
    m_modelByFindedDevices(new QStandardItemModel(this)),
    m_modelByDevicesForConnection(new QStandardItemModel(this)),
    m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)),
    m_fileManager(new FileManager(this)),
    m_bleClient(nullptr),
    m_bleServer(nullptr),
    m_clibBoardMonitor(new ClipboardMonitor(this)) {
    ui->setupUi(this);

    auto r = m_fileManager->loadDeviceList();
    // загрузка из файла
    for(auto deviceInfo: m_fileManager->loadDeviceList()) {
        auto item = new BluetoothStandartItem(deviceInfo.name(), deviceInfo);
        item->setData("Неизвестно", Qt::UserRole + 2);
        m_modelByDevicesForConnection->appendRow(item);
    }
    // QObject::connect(qApp, &QApplication::aboutToQuit, [&devices]() {
    //     saveDeviceList(devices);
    // });


    // скрол
    updateVisibility();
    ui->scroll->setWidgetResizable(true);
    ui->scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scroll->setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);

    if (m_localDeviceIsServer) {
      m_bleServer = new BleServer(this);
    } else {
      m_bleClient = new BleClient(this);
      connect(m_bleClient, &BleClient::connected, this, &SearchWidget::connectedBluetooth);
      connect(m_bleClient, &BleClient::disconnected, this, &SearchWidget::disconnectedBluetooth);
    }

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

    // клиент отправляет данные и отправляем TODO протестировать фотки
    connect(m_clibBoardMonitor, &ClipboardMonitor::copyText, this, &SearchWidget::sendMessageStr);
    connect(m_clibBoardMonitor, &ClipboardMonitor::copyImage, this, &SearchWidget::sendMessageImage);

    // получение с клиента данных и запись их в буффер обмена
}

void SearchWidget::initFindedDevices() {
    ui->findedDevicesView->setItemDelegate(m_delegate);
    ui->findedDevicesView->setModel(m_modelByFindedDevices);

    connect(ui->findedDevicesView, &QListView::clicked, this, &SearchWidget::onItemClicked);
    connect(ui->devicesForConnection, &QListView::activated, this, &SearchWidget::onItemDoubleCLicked);
}

SearchWidget::~SearchWidget() {
    QList<QBluetoothDeviceInfo> itemList;
    // Проходим по всем строкам и столбцам модели
    for (int row = 0; row < m_modelByDevicesForConnection->rowCount(); ++row) {
        BluetoothStandartItem *item = dynamic_cast<BluetoothStandartItem*>(m_modelByDevicesForConnection->item(row));
        if (item) {
            itemList.append(item->getDevice());
        }
    }
    m_fileManager->saveDeviceList(itemList);

    delete ui;
}

void SearchWidget::updateVisibility() {
    // Скрываем или показываем ListView в зависимости от количества элементов
    ui->findedDevicesView->setVisible(m_modelByFindedDevices->rowCount() > 0);
}

void SearchWidget::enableServer(bool isEnable) {
    if (isEnable) {
      m_bleServer->start();
    }
}

void SearchWidget::connectToDevices() {
  for (int row = 0; row < m_modelByDevicesForConnection->rowCount(); ++row) {
    auto _item = m_modelByDevicesForConnection->item(row);
    const BluetoothStandartItem* item = dynamic_cast<BluetoothStandartItem*>(m_modelByDevicesForConnection->item(row));
    if (item) {
        auto device = item->getDevice();
        if (m_bleClient) m_bleClient->connectToDevice(item->getDevice());
        qDebug() << "connecting to " << item->text();
    } else qDebug() << "не найден элемент для подключения...";
  }
}

// кладем вниз
void SearchWidget::onItemClicked(const QModelIndex &index) {
    // Извлекаем элемент по индексу
    if (!m_localDeviceIsServer) {
        QString deviceBleName = index.data(Qt::DisplayRole).toString();
        QString connect = index.data(Qt::UserRole + 1).toString();
        QString notConnect = index.data(Qt::UserRole + 2).toString();
        qDebug() << "Clicked item custom data:" << deviceBleName << " " << connect << notConnect;

        if (checkOnUnicModel(m_modelByDevicesForConnection, deviceBleName)) {

            qDebug() << connect.isEmpty() << " " << connect;

            BluetoothStandartItem *_item = dynamic_cast<BluetoothStandartItem*>(m_modelByFindedDevices->itemFromIndex(index));
            if (_item) {
              auto item = new BluetoothStandartItem(_item);
              auto device = item->getDevice();
             qDebug() << device.name()  << " " << device.deviceUuid() << " " << device.address() << " " << device.majorDeviceClass() << " " << device.minorDeviceClass() << " " << device.serviceClasses() << " " << device.rssi() << " ";

              if (connect.isEmpty())
                  item->setData(NOT_CONNECTED, Qt::UserRole + 2);
              else
                  item->setData(CONNECT, Qt::UserRole + 1);
              m_modelByDevicesForConnection->appendRow(item);
            } else qDebug() << "не найден элемент для подключения...";
        }
    }
}

void SearchWidget::onItemDoubleCLicked(const QModelIndex &index) {
    if (!m_localDeviceIsServer) {
        qDebug() << "Отключение устройств не реализовано";
        m_modelByDevicesForConnection->removeRow(index.row());
    }
}

// найдено устройство
void SearchWidget::deviceDiscovered(const QBluetoothDeviceInfo &device) {
  if (!device.name().isEmpty() && device.name() != device.address().toString()
    && checkOnUnicModel(m_modelByFindedDevices, device.name())) {

    qDebug() << device.name();
    BluetoothStandartItem *item1 = new BluetoothStandartItem(device.name(), device);
qDebug() << device.name()  << " " << device.deviceUuid() << " " << device.address() << " " << device.majorDeviceClass() << " " << device.minorDeviceClass() << " " << device.serviceClasses() << " " << device.rssi() << " ";    item1->setData(NOT_CONNECTED, Qt::UserRole + 2);
    m_modelByFindedDevices->appendRow(item1);
    if (ui->findedDevicesView->isHidden()) ui->findedDevicesView->show();
  }
}

void SearchWidget::scanFinished() {
  ui->doFindButton->setText("Начать поиск");
}

void SearchWidget::activeLocalDeviceIsServer(bool isServer) {
  m_localDeviceIsServer = isServer;
  if (isServer) {
      if (!m_modelByConnectionDevice) m_modelByConnectionDevice = new QStandardItemModel(this);
      ui->devicesForConnection->setModel(m_modelByConnectionDevice);
      if (m_bleClient) m_bleClient->deleteLater();
      if (!m_bleServer) {
          m_bleServer = new BleServer(this);
          connect(m_bleServer, &BleServer::getText, m_clibBoardMonitor, &ClipboardMonitor::setText);
          connect(m_bleServer, &BleServer::getImage, m_clibBoardMonitor, &ClipboardMonitor::setImage);
          connect(m_bleServer, &BleServer::getData, m_clibBoardMonitor, &ClipboardMonitor::setData);
      }
  } else {
      ui->devicesForConnection->setModel(m_modelByDevicesForConnection);
      if (m_bleServer) m_bleServer->deleteLater();
      if (!m_bleClient) m_bleClient = new BleClient(this);
  }
}

void SearchWidget::connectedBluetooth() {
  qDebug() << "connected to Server";
}

void SearchWidget::disconnectedBluetooth() {
  qDebug() << "disconnect to Server";
}

void SearchWidget::sendMessage(const QByteArray &message) {
  if (m_bleClient) m_bleClient->sendMessage(message);
}

void SearchWidget::sendMessageStr(const QString &message) {
  if (m_bleClient) m_bleClient->sendMessage(message.toUtf8());
}

void SearchWidget::sendMessageImage(const QImage &image) {
  if (m_bleClient) m_bleClient->sendMessage(convertImageToByteArray(image));
}

void SearchWidget::updateConnectedDevices() {
  #ifdef Q_OS_MAC
    // NSArray *connectedDevices = [IOBluetoothDevice pairedDevices];
    //     for (IOBluetoothDevice *device in connectedDevices) {
    //         if ([device isConnected]) {
    //           BluetoothClassOfDevice classOfDevice = [device classOfDevice];
    //           quint32 deviceClassAsQuint32 = (quint32)classOfDevice;
    //           QString deviceName = QString::fromNSString([device name]);
    //           QString deviceAddress = QString::fromNSString([device addressString]);
    //           qDebug() << "quint32: " << deviceClassAsQuint32 << " deviceName: " << deviceName << " deviceAddress: " << deviceAddress;
    //           if (checkOnUnicModel(m_modelByFindedDevices, deviceName)) {
    //             BluetoothStandartItem *item = new BluetoothStandartItem(deviceName, QBluetoothDeviceInfo(QBluetoothAddress(deviceName), deviceAddress, deviceClassAsQuint32));
    //             item->setData(CONNECT, Qt::UserRole + 1);
    //             m_modelByFindedDevices->appendRow(item);
    //           }
    //         }
    //     }
  #else
// todo достать q32 и сделать с BluetoothStandartItem
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

QByteArray SearchWidget::convertImageToByteArray(const QImage &image) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    // Сохраняем изображение в его исходном формате
    if (image.save(&buffer)) {
        // Если формат не задан, QImage сам использует формат исходного изображения
        return byteArray;  // Возвращаем данные как QByteArray
    }
    else {
        // Обработка ошибок сохранения
        qDebug() << "Failed to save image to QByteArray.";
        return QByteArray();  // Пустой QByteArray для ошибки
    }
}



        // qDebug() << device.deviceUuid() << " " << device.address() << " " << device.majorDeviceClass() << " " << device.minorDeviceClass() << " " << device.serviceClasses() << " " << device.rssi() << " ";

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
