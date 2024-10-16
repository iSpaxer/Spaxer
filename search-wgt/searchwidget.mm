#include "searchwidget.h"
#include "ui_searchwidget.h"

#include <QScroller>
#include <QEasingCurve>

const QString SearchWidget::CONNECT = "Подключено";
const QString SearchWidget::NOT_CONNECTED = "Не подключено";

SearchWidget::SearchWidget(QWidget *parent):
    QFrame(parent),
    ui(new Ui::searchWidget),
    m_delegate(new SearchItemDelegate(ui->findedDevicesView)),
    m_modelByFindedDevices(new QStandardItemModel(this)),
    m_modelByDevicesForConnection(new QStandardItemModel(this)),
    m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)),
    m_localDevice(new QBluetoothLocalDevice(this)) {
    ui->setupUi(this);

    initFindedDevices();

    ui->devicesForConnection->setItemDelegate(m_delegate);
    ui->devicesForConnection->setModel(m_modelByDevicesForConnection);

    connect(ui->doFindButton, &QPushButton::clicked, [this](bool checked) {
        m_discoveryAgent->start();
        m_modelByFindedDevices->clear();
        ui->doFindButton->setText("Поиск устройств...");

        // m_localDevice.pa

        // QList<QBluetoothDeviceInfo> pairedDevices = m_localDevice->pairedDevices();

        // // Ищем подключенные устройства среди сопряжённых
        // for (const QBluetoothDeviceInfo &deviceInfo : pairedDevices) {
        //     if (connectedDevices.contains(deviceInfo.address())) {
        //         // Выводим имя и адрес устройства
        //         qDebug() << "Устройство:" << deviceInfo.name() << ", адрес:" << deviceInfo.address().toString();
        //     }
        // } Если у вас есть способ получить имя
        // }
    });

    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &SearchWidget::deviceDiscovered);
    connect(m_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &SearchWidget::scanFinished);

}

void SearchWidget::initFindedDevices() {
    ui->findedDevicesView->setItemDelegate(m_delegate);
    ui->findedDevicesView->setModel(m_modelByFindedDevices);

    QStandardItem *item1 = new QStandardItem("MacBook Pro - Александр");
    item1->setData(NOT_CONNECTED, Qt::UserRole + 2);
    m_modelByFindedDevices->appendRow(item1);

    QStandardItem *item2 = new QStandardItem("iPhone - Александр");
    item2->setData(CONNECT, Qt::UserRole + 1);
    m_modelByFindedDevices->appendRow(item2);


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

SearchWidget::~SearchWidget()
{
    delete ui;
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

}

void SearchWidget::scanFinished()
{

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

void SearchWidget::setConnectedDevicesToServer(QList<QString> connectedDevices) {
    if (!m_modelByConnectionDevice) m_modelByConnectionDevice = new QStandardItemModel(this);
    m_modelByConnectionDevice->clear();
    for (auto device: connectedDevices) {
        QStandardItem *item = new QStandardItem(device);
        m_modelByConnectionDevice->appendRow(item);
    }
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
