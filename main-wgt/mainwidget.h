#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QMovie>

namespace Ui {
class MainWidget;
}

class MainWidget : public QFrame {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void successConnect(bool success);

private slots:
    void activeServer();
    void activeClient();

    void animationConnectButtonAnimation(bool checked);
    void animationOnServerButton(bool checked);

signals:
    void enableServer(bool isEnable);
    void connectToDevices();
    void activeDeviceIsServer(bool isServer);
    void sendMessage(QByteArray message);

private:
    Ui::MainWidget *ui;
    QMovie *m_movieConnect;
    QMovie *m_movieOnServer;
    void init();

};

#endif // MAINWIDGET_H
