#include "gradientbutton.h"

GradientButton::GradientButton(QWidget *parent): QPushButton{parent}
{}

GradientButton::GradientButton(const QString &text, QWidget *parent) {
    setText(text);
}

void GradientButton::paintEvent(QPaintEvent *event) {
    QPushButton::paintEvent(event);

    // Настройка QPainter для кастомной отрисовки
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!isChecked()) {
        painter.setPen(Qt::white);
        return;
    }

    // Определяем линейный градиент для текста
    QLinearGradient gradient(0, height(), width(), height());  // Горизонтальный градиент
    gradient.setColorAt(0, QColor(255, 255, 0));
    gradient.setColorAt(0.4, QColor(255, 200, 0));
    gradient.setColorAt(1, QColor(255, 0, 0));            // Цвет справа

    // Устанавливаем шрифт и стиль текста
    painter.setFont(this->font());
    painter.setPen(QPen(gradient, 0));

    // Выравниваем текст по центру
    QRect textRect = this->rect();
    QString buttonText = this->text();

    painter.drawText(textRect, Qt::AlignCenter, buttonText);
}
