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
    m_activeButton(nullptr) {
    ui->setupUi(this);

    init();



}

void MainWindow::init() {

    // TODO логику, если выключен блютус, включаем настройки

    m_activeButton = ui->mainButton;
    m_activeButton->setChecked(true);

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
    m_activeButton->setChecked(false);
    m_activeButton = button;
    m_activeButton->setChecked(true);
}
