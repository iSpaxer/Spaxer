#ifndef GRADIENTLABEL_H
#define GRADIENTLABEL_H

#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QWidget>

class GradientButton : public QPushButton
{
    Q_OBJECT
public:
    explicit GradientButton(QWidget *parent = nullptr);
    explicit GradientButton(const QString& text, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
};

#endif // GRADIENTLABEL_H
