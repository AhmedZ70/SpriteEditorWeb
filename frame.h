#ifndef FRAME_H
#define FRAME_H

#include <QColor>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include <QJsonObject>
#include <QStack>

/**
 * @brief Frame class adjusted for WebAssembly compatibility.
 * Represents a single frame in the sprite animation.
 */
class Frame {
private:
    /// @brief Image representing the frame
    QImage image;

    /// @brief Undo and redo stacks for image states
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;

public:
    /// @brief Constructor with specified width and height
    Frame(int width, int height);

    /// @brief Returns a reference to the frame's image
    QImage& getImage();

    /// @brief Sets the color of a pixel at (x, y)
    void setPixel(int x, int y, const QColor& color);

    /// @brief Serializes the frame to a JSON object
    QJsonObject toJson() const;

    /// @brief Deserializes a frame from a JSON object
    static Frame fromJson(const QJsonObject& json);

    /// @brief Takes a snapshot of the current image state for undo functionality
    void takeSnapshot();

    /// @brief Undoes the last action
    void undo();

    /// @brief Redoes the last undone action
    void redo();
};

#endif // FRAME_H
