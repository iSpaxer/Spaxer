#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>

// #include <search-wgt/searchwidget.h>
#include <main-wgt/mainwidget.h>
#include <search-wgt/SearchWidget.h>
#include <settings-wgt/settingswidget.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SearchWidget *m_searchWidget;
    MainWidget *m_mainWidget;
    SettingsWidget *m_settingsWidget;

    QPushButton *m_activeButtonStatusBar;

    void init();
    void connectButtonToWidget(QPushButton *button, int index);
    void setActiveButton(QPushButton *button);
};
#endif // MAINWINDOW_H
