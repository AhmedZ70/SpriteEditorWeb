#include "drawingcanvas.h"
#include <QCursor>
#include <QPixmap>
#include <QDebug>

/**
 * @brief DrawingCanvas class implementation adjusted for WebAssembly compatibility.
 */

DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent), currentColor(Qt::black), drawingActive(false) {
    setAttribute(Qt::WA_StaticContents);
    enterDrawingMode();
}

void DrawingCanvas::paintEvent(QPaintEvent *) {
    emit requestCurrentImage();
    QPainter painter(this);

    // Draw checkerboard background to represent transparency
    int tileSize = 10;
    QColor color1(220, 220, 220);
    QColor color2(255, 255, 255);

    QRect background = this->rect();

    for (int y = 0; y < background.height(); y += tileSize) {
        for (int x = 0; x < background.width(); x += tileSize) {
            QRect tileRect(x, y, tileSize, tileSize);
            QColor tileColor = ((x / tileSize) % 2 == (y / tileSize) % 2) ? color1 : color2;
            painter.fillRect(tileRect, tileColor);
        }
    }

    // Draw the current image
    if (!currentImage.isNull()) {
        painter.drawImage(rect(), currentImage);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawingActive = true;
        emit drawingStarted();

        QPoint canvasPoint = event->position().toPoint();
        emit requestPixelChange(canvasPoint, currentColor, width(), height());
    }
}

void DrawingCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (drawingActive && (event->buttons() & Qt::LeftButton)) {
        QPoint canvasPoint = event->position().toPoint();
        emit requestPixelChange(canvasPoint, currentColor, width(), height());
    }
}

void DrawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && drawingActive) {
        drawingActive = false;
    }
}

void DrawingCanvas::receiveCurrentImage(const QImage& image) {
    currentImage = image;
}

void DrawingCanvas::updateDrawing(){
    // Triggers repaint which will call paintEvent
    update();
}

void DrawingCanvas::colorChanged(QColor newColor) {
    currentColor = newColor;
}

void DrawingCanvas::enterDrawingMode() {
    // Set cursor to standard crosshair cursor
    setCursor(Qt::CrossCursor);
}

void DrawingCanvas::enterErasingMode() {
    // Set cursor to standard crosshair cursor
    setCursor(Qt::CrossCursor);
}
