#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QMovie>

namespace Ui {
class MainWidget;
}

class MainWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void activeServer();
    void activeClient();

private:
    Ui::MainWidget *ui;
    QMovie *m_movie;

    void init();
    void startConnectButtonAnimation();
    void stopConnectButtonAnimation();
    void setGradient(QLabel *label);
};

#endif // MAINWIDGET_H
