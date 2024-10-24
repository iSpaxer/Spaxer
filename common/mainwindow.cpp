#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMovie>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_searchWidget(new SearchWidget),
    m_mainWidget(new MainWidget),
    m_settingsWidget(new SettingsWidget),
    m_activeButtonStatusBar(nullptr) {

    ui->setupUi(this);

    init();

    connect(m_mainWidget, &MainWidget::activeDeviceIsServer,
            m_searchWidget, &SearchWidget::activeLocalDeviceIsServer);

    connect(m_mainWidget, &MainWidget::connectToDevices,
            m_searchWidget, &SearchWidget::connectToDevices);

    connect(m_mainWidget, &MainWidget::sendMessage,
            m_searchWidget, &SearchWidget::sendMessage);

    connect(m_mainWidget, &MainWidget::enableServer,
            m_searchWidget, &SearchWidget::enableServer);

    connect(m_searchWidget, &SearchWidget::successConnect,
            m_mainWidget, &MainWidget::successConnect)  ;

}

void MainWindow::init() {

    // TODO логику, если выключен блютус, включаем настройки
    m_activeButtonStatusBar = ui->mainButton;
    m_activeButtonStatusBar->setChecked(true);


    ui->stackedWidget->addWidget(m_searchWidget);
    ui->stackedWidget->addWidget(m_mainWidget);
    ui->stackedWidget->addWidget(m_settingsWidget);

    connectButtonToWidget(ui->searchButton, 0);  // Для Виджета 1
    connectButtonToWidget(ui->mainButton, 1);  // Для Виджета 2
    connectButtonToWidget(ui->settingsButton, 2);  // Для Виджета 3

    ui->stackedWidget->setCurrentIndex(1);
    // m_mainWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectButtonToWidget(QPushButton *button, int index) {
    connect(button, &QPushButton::clicked, [=]() {
        setActiveButton(button);
        ui->stackedWidget->setCurrentIndex(index);  // Переключаем на виджет с заданным индексом
    });
}

void MainWindow::setActiveButton(QPushButton *button) {
    m_activeButtonStatusBar->setChecked(false);
    m_activeButtonStatusBar = button;
    m_activeButtonStatusBar->setChecked(true);
}



