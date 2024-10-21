#ifndef CLIPBOARDMONITOR_H
#define CLIPBOARDMONITOR_H

#include <QApplication>
#include <QClipboard>
#include <QTimer>
#include <QDebug>
#include <QMimeData>
#include <QImage>
#include <QUrl>

class ClipboardMonitor : public QObject {
    Q_OBJECT

public:
    explicit ClipboardMonitor(QObject *parent = nullptr);

public slots:
    void setText(const QString &text);
    void setImage(const QImage &image);
    void setData(const QByteArray &data);

private slots:
    void checkClipboardTimer();

signals:
    void copyText(const QString &text);
    void copyImage(const QImage &image);
    void copyData(const QByteArray &data);

private:
    QClipboard *clipboard;
    QTimer *timer;
    QString lastClipboardText;
    QImage lastClipboardImage;
    QList<QUrl> lastClipboardUrls;

    void startClipboardTimer();
    void startClipboardSignals();
};


#endif // CLIPBOARDMONITOR_H
