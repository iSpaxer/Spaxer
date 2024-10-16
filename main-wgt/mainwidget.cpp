#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QPainter>

MainWidget::MainWidget(QWidget *parent):
    QFrame(parent),
    ui(new Ui::MainWidget) {
    ui->setupUi(this);

    init();
}

void MainWidget::init() {
    ui->onServerButton->hide();

    connect(ui->serverButton, &QPushButton::clicked, this, &MainWidget::activeServer);
    connect(ui->clientButton, &QPushButton::clicked, this, &MainWidget::activeClient);

    connect(ui->onServerButton, &QPushButton::clicked, this, &MainWidget::animationOnServerButton);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWidget::animationConnectButtonAnimation);
}


MainWidget::~MainWidget() {
    delete ui;
}

void MainWidget::activeServer() {
    ui->connectButton->hide();
    ui->onServerButton->show();


    if (ui->connectButton->isChecked()) {
        animationConnectButtonAnimation(false);
        ui->connectButton->setChecked(false);
    }
    ui->clientButton->setChecked(false);

    emit activeDeviceIsServer(true);
}

void MainWidget::activeClient() {
    ui->onServerButton->hide();
    ui->connectButton->show();

    if (ui->onServerButton->isChecked()) {
        animationOnServerButton(false);
        ui->onServerButton->setChecked(false);
    }
    ui->serverButton->setChecked(false);

    emit activeDeviceIsServer(false);
}

void MainWidget::animationOnServerButton(bool checked) {
    if (checked) {
        emit connectToDevices();
        m_movieOnServer = new QMovie(":/mainWidget/server-on.gif");
        // Устанавливаем анимацию как иконку для QPushButton
        QObject::connect(m_movieOnServer, &QMovie::frameChanged, [this]() {
            ui->onServerButton->setIcon(QIcon(m_movieOnServer->currentPixmap()));  // Обновляем иконку для кнопки при каждом изменении кадра
        });
        m_movieOnServer->start();
    } else {
        ui->onServerButton->setIcon(QIcon(":/mainWidget/server-black"));
        m_movieOnServer->stop();
        delete m_movieOnServer;
    }

}


void MainWidget::animationConnectButtonAnimation(bool checked) {
    if (checked) {
        m_movieConnect = new QMovie(":/mainWidget/connect-colored.gif");
        QObject::connect(m_movieConnect, &QMovie::frameChanged, [this]() {
            ui->connectButton->setIcon(QIcon(m_movieConnect->currentPixmap()));  // Обновляем иконку для кнопки при каждом изменении кадра
        });
        m_movieConnect->start();
    } else  {
        ui->connectButton->setIcon(QIcon(":/mainWidget/connect-black"));
        m_movieConnect->stop();
        delete m_movieConnect;
    }
}

