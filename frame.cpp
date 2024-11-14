#include "frame.h"
#include <QBuffer>
#include <QDebug>

/**
 * @brief Frame class implementation adjusted for WebAssembly compatibility.
 */

Frame::Frame(int width, int height) : image(width, height, QImage::Format_ARGB32) {
    // Initialize image with transparent pixels
    image.fill(Qt::transparent);
}

QImage& Frame::getImage() {
    // Returns a reference to the image
    return image;
}

void Frame::setPixel(int x, int y, const QColor &color) {
    // Sets the pixel color if the coordinates are within bounds
    if (x >= 0 && x < image.width() && y >= 0 && y < image.height()) {
        image.setPixelColor(x, y, color);
    }
}

QJsonObject Frame::toJson() const {
    QJsonObject jsonObj;

    // Serialize the image to a byte array in PNG format
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    // Encode the byte array to Base64 to store in JSON
    QString base64Data = QString::fromLatin1(byteArray.toBase64());

    jsonObj["imageData"] = base64Data;
    jsonObj["width"] = image.width();
    jsonObj["height"] = image.height();

    return jsonObj;
}

Frame Frame::fromJson(const QJsonObject& json) {
    QString base64Data = json["imageData"].toString();
    QByteArray byteArray = QByteArray::fromBase64(base64Data.toLatin1());

    QImage image;
    image.loadFromData(byteArray, "PNG");

    if (image.isNull()) {
        qDebug() << "Failed to load image from data.";
    }

    Frame frame(image.width(), image.height());
    frame.image = image;

    return frame;
}

void Frame::takeSnapshot() {
    // Stores a copy of the image onto the undo stack
    undoStack.push(image.copy());
    // Clear the redo stack when a new action is taken
    redoStack.clear();
}

void Frame::undo() {
    if (!undoStack.isEmpty()) {
        // Push the current image onto the redo stack
        redoStack.push(image);
        // Restore the image from the undo stack
        image = undoStack.pop();
    }
}

void Frame::redo() {
    if (!redoStack.isEmpty()) {
        // Push the current image onto the undo stack
        undoStack.push(image);
        // Restore the image from the redo stack
        image = redoStack.pop();
    }
}
