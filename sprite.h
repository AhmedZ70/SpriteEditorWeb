#ifndef SPRITE_H
#define SPRITE_H

#include "frame.h"
#include <vector>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @brief Sprite class adjusted for WebAssembly compatibility.
 * Represents a collection of frames that make up a sprite animation.
 */
class Sprite {
private:
    /// @brief Vector containing the frames of the sprite
    std::vector<Frame> frames;

    /// @brief Width and height of a sprite frame
    int width;
    int height;

public:
    /// @brief Default constructor
    Sprite();

    /// @brief Constructor with specified width and height
    Sprite(int width, int height);

    /// @brief Adds a new empty frame to the sprite
    void addFrame();

    /// @brief Adds a provided frame to the sprite
    void addFrame(const Frame& frame);

    /// @brief Removes the frame at the specified index
    void removeFrame(size_t index);

    /// @brief Gets a reference to the frame at the specified index
    Frame& getFrame(size_t index);

    /// @brief Returns the number of frames in the sprite
    size_t frameCount() const;

    /// @brief Inserts a frame at the specified index
    void insertFrame(size_t index, const Frame& frame);

    /// @brief Serializes the sprite to a JSON object
    QJsonObject toJson() const;

    /// @brief Deserializes a sprite from a JSON object
    static Sprite fromJson(const QJsonObject& json);

    /// @brief Returns a vector of all frames
    std::vector<Frame> returnFrames() const;

    /// @brief Gets the width of the sprite frames
    int getWidth() const;

    /// @brief Gets the height of the sprite frames
    int getHeight() const;
};

#endif // SPRITE_H
