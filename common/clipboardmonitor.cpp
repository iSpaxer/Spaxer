#include "clipboardmonitor.h"


ClipboardMonitor::ClipboardMonitor(QObject *parent)
    : QObject(parent), m_bufferText(""), lastClipboardImage() {
    clipboard = QApplication::clipboard();

    #ifdef Q_OS_MAC
    startClipboardTimer();
    #else
    startClipboardSignals();
    #endif
}

void ClipboardMonitor::setText(const QString &text) {
    if (checkRepeatText(text)) {
        QApplication::clipboard()->setText(text);
    }
}

void ClipboardMonitor::setImage(const QImage &image) {
    QApplication::clipboard()->setImage(image);
}

void ClipboardMonitor::setData(const QByteArray &data) {
    auto mimeData = new QMimeData();
    mimeData->setData("application/octet-stream", data);
    QApplication::clipboard()->setMimeData(mimeData);
}

void ClipboardMonitor::checkClipboardTimer() {
    const QMimeData *mimeData = clipboard->mimeData();

    // Проверяем текст
    if (mimeData->hasText()) {
        QString currentText = mimeData->text();
        if (!checkRepeatText(currentText) && !currentText.isEmpty()) {
            qDebug() << "Clipboard updated (Text): " << currentText;
            emit copyText(currentText);
            // Здесь можно вызывать свою функцию для обработки текста
        }
    }

    // Проверяем изображения
    if (mimeData->hasImage()) {
        QImage currentImage = qvariant_cast<QImage>(mimeData->imageData());
        if (currentImage != lastClipboardImage) {
            lastClipboardImage = currentImage;
            qDebug() << "Clipboard updated (Image)";
            emit copyImage(currentImage);
            // Здесь можно вызывать свою функцию для обработки изображений
        }
    }

    // Проверяем документы (ссылки на файлы)
    if (mimeData->hasUrls()) {
        QList<QUrl> currentUrls = mimeData->urls();
        if (currentUrls != lastClipboardUrls) {
            lastClipboardUrls = currentUrls;
            qDebug() << "Clipboard updated (Files): ";
            for (const QUrl &url : currentUrls) {
                qDebug() << url.toLocalFile();
                // Здесь можно вызывать свою функцию для обработки документов
            }
        }
    }
}

void ClipboardMonitor::startClipboardTimer() {
    // Настраиваем таймер для проверки изменений в буфере обмена
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClipboardMonitor::checkClipboardTimer);
    timer->start(1000);  // Проверка каждые 1 секунду
}

void ClipboardMonitor::startClipboardSignals() {
    const QMimeData *mimeData = clipboard->mimeData();

    // Проверяем текст
    if (mimeData->hasText()) {
        QString currentText = mimeData->text();
        if (!checkRepeatText(currentText)) {
            qDebug() << "Clipboard updated (Text): " << currentText;
            emit copyText(currentText);
        }
    }

    // Проверяем изображения
    if (mimeData->hasImage()) {
        QImage currentImage = qvariant_cast<QImage>(mimeData->imageData());
        if (currentImage != lastClipboardImage) {
            lastClipboardImage = currentImage;
            qDebug() << "Clipboard updated (Image)";
            emit copyImage(currentImage);
            // Здесь можно вызывать свою функцию для обработки изображений
        }
    }
}

// текст повторяется?
bool ClipboardMonitor::checkRepeatText(const QString &text) {
    if (m_bufferText == text) {
        return true;
    }
    m_bufferText = text;
    return false;
}
