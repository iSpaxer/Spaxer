#ifndef SEARCHITEMDELEGATE_H
#define SEARCHITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class SearchItemDelegate : public QStyledItemDelegate {

public:
    explicit SearchItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};


#endif // SEARCHITEMDELEGATE_H
