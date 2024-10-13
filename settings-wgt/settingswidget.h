#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QFrame>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_H
