#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QPainter>

MainWidget::MainWidget(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::MainWidget) {
    ui->setupUi(this);

    init();

}

void MainWidget::init() {
    connect(ui->serverButton, &QPushButton::clicked, this, &MainWidget::activeServer);
    connect(ui->clientButton, &QPushButton::clicked, this, &MainWidget::activeClient);
}


MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::activeServer() {
    ui->clientButton->setChecked(false);
}

void MainWidget::activeClient() {
    ui->serverButton->setChecked(false);
}

void MainWidget::startConnectButtonAnimation() {
    m_movie = new QMovie(":/mainWidget/connect-colored.gif");
    // Устанавливаем анимацию как иконку для QPushButton
    ui->connectButton->setIconSize(QSize(150, 150));  // Задаем размер иконки (подогнать под размер GIF)
    QObject::connect(m_movie, &QMovie::frameChanged, [this]() {
        ui->connectButton->setIcon(QIcon(m_movie->currentPixmap()));  // Обновляем иконку для кнопки при каждом изменении кадра
    });

    // Запускаем анимацию GIF
}

void MainWidget::stopConnectButtonAnimation() {
    m_movie->stop();
    delete m_movie;
}
