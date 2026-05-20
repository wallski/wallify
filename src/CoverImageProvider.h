#ifndef COVERIMAGEPROVIDER_H
#define COVERIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QUrl>
#include <QDebug>
#include "LocalLibrary.h"

class CoverImageProvider : public QQuickImageProvider
{
public:
    explicit CoverImageProvider(LocalLibrary *library)
        : QQuickImageProvider(QQuickImageProvider::Image), m_library(library)
    {
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        Q_UNUSED(requestedSize);
        
        // Percent-decode the ID (e.g. handle %20 for spaces)
        QString filePath = QUrl::fromPercentEncoding(id.toUtf8());
        
        // Strip query parameters if present (e.g. ?t=123456)
        int queryIndex = filePath.indexOf('?');
        if (queryIndex != -1) {
            filePath = filePath.left(queryIndex);
        }
        
        // On Windows, QUrl/QML might prepend a leading slash to drive letters (e.g. /C:/path)
#if defined(Q_OS_WIN)
        if (filePath.startsWith("/") && filePath.length() > 2 && filePath.at(2) == ':') {
            filePath = filePath.mid(1);
        }
#endif
        
        qDebug() << "CoverImageProvider: requestImage path =" << filePath;
        
        QByteArray data = m_library->getCoverData(filePath);
        QImage image;
        if (!data.isEmpty()) {
            image.loadFromData(data);
        }
        
        // Fallback placeholder (a nice gradient with a music note icon)
        if (image.isNull()) {
            image = QImage(300, 300, QImage::Format_ARGB32);
            image.fill(Qt::transparent);
            
            QPainter painter(&image);
            painter.setRenderHint(QPainter::Antialiasing);
            
            // Draw a rounded dark box
            painter.setBrush(QColor("#1e1e28"));
            painter.setPen(QColor("#2a2a35"));
            painter.drawRoundedRect(0, 0, 300, 300, 16, 16);
            
            // Draw text placeholder
            painter.setPen(QColor("#89899f"));
            QFont font("Roboto", 14);
            painter.setFont(font);
            painter.drawText(image.rect(), Qt::AlignCenter, "No Cover Art");
            painter.end();
        }

        if (size) {
            *size = image.size();
        }
        return image;
    }

private:
    LocalLibrary *m_library;
};

#endif // COVERIMAGEPROVIDER_H
