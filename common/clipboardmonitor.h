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
    void setTextIn(const QString &text);
    void setImageIn(const QImage &image);

private slots:
    void checkClipboardTimer();

signals:
    void changeText(const QString &text);
    void changeImage(const QImage &image);

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
