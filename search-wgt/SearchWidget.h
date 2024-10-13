#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QFrame>

namespace Ui {
class searchWidget;
}

class SearchWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = nullptr);
    ~SearchWidget();

private:
    Ui::searchWidget *ui;
};

#endif // SEARCHWIDGET_H
