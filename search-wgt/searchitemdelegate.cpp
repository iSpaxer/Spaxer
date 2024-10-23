#include "searchitemdelegate.h"

void SearchItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    // Background color
    // painter->fillRect(option.rect, QColor(200, 200, 200));

    // Device name and connection status
    QString deviceName = index.data(Qt::DisplayRole).toString();
    QString connect = index.data(Qt::UserRole + 1).toString();
    QString notConnect = index.data(Qt::UserRole + 2).toString();

    int leftPadding = 26;

    // Drawing device name (larger font)
    QFont deviceFont = option.font;
    deviceFont.setBold(true);
    deviceFont.setPointSize(18);
    painter->setFont(deviceFont);
    painter->setPen(Qt::white);
    painter->drawText(option.rect.adjusted(leftPadding, 10, -10, -30), Qt::AlignLeft | Qt::AlignTop, deviceName);

    // Drawing disconnect (smaller font)
    QFont notConnectFont = option.font;
    notConnectFont.setPointSize(14);
    painter->setFont(notConnectFont);
    if (connect.isEmpty()) {
        painter->setPen(QColor("#F5F5F5"));
        painter->drawText(option.rect.adjusted(leftPadding, 35, -10, -10), Qt::AlignLeft | Qt::AlignBottom, notConnect);
    } else {
        painter->setPen(QColor("#BCFB00"));
        painter->drawText(option.rect.adjusted(leftPadding, 35, -10, -10), Qt::AlignLeft | Qt::AlignBottom, connect);
    }

    painter->setPen(QPen(Qt::white, 1));
    QPoint bottomLeft = option.rect.adjusted(leftPadding, 0, 0, 0).bottomLeft();
    QPoint bottomRight = option.rect.bottomRight();

    // Опускаем линию на 1 пиксель вниз
    painter->drawLine(bottomLeft.x(), bottomLeft.y() + 2, // Опускаем Y на 1
                      bottomRight.x(), bottomRight.y() + 2); // Опускаем Y на 1

    painter->restore();
}

QSize SearchItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // Set the height of each item
    return QSize(option.rect.width(), 60);
}
