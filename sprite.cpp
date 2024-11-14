#include "sprite.h"
#include <stdexcept>

/**
 * @brief Sprite class implementation adjusted for WebAssembly compatibility.
 */

Sprite::Sprite() : width(64), height(64) {
    // Default constructor initializes with default width and height
}

Sprite::Sprite(int width, int height) : width(width), height(height) {
    // Constructor with specified dimensions
}

void Sprite::addFrame() {
    // Adds a new empty frame with the sprite's dimensions
    Frame frame(width, height);
    frames.push_back(frame);
}

void Sprite::addFrame(const Frame& frame) {
    // Adds a provided frame to the sprite
    frames.push_back(frame);
}

void Sprite::removeFrame(size_t index) {
    // Removes the frame at the specified index if valid
    if (index < frames.size()) {
        frames.erase(frames.begin() + index);
    }
}

Frame& Sprite::getFrame(size_t index) {
    // Returns a reference to the frame at the specified index
    if (index < frames.size()) {
        return frames.at(index);
    }
    // Throw an exception if the index is out of range
    throw std::out_of_range("Frame index out of range");
}

size_t Sprite::frameCount() const {
    // Returns the number of frames in the sprite
    return frames.size();
}

void Sprite::insertFrame(size_t index, const Frame& frame) {
    // Inserts a frame at the specified index
    if (index <= frames.size()) {
        frames.insert(frames.begin() + index, frame);
    } else {
        // If index is out of bounds, add to the end
        frames.push_back(frame);
    }
}

QJsonObject Sprite::toJson() const {
    QJsonObject jsonObj;
    jsonObj["width"] = width;
    jsonObj["height"] = height;

    QJsonArray framesArray;
    for (const Frame& frame : frames) {
        framesArray.append(frame.toJson());
    }
    jsonObj["frames"] = framesArray;

    return jsonObj;
}

Sprite Sprite::fromJson(const QJsonObject& json) {
    int width = json["width"].toInt();
    int height = json["height"].toInt();

    Sprite sprite(width, height);

    QJsonArray framesArray = json["frames"].toArray();
    for (const QJsonValue& value : framesArray) {
        QJsonObject frameObj = value.toObject();
        Frame frame = Frame::fromJson(frameObj);
        sprite.addFrame(frame);
    }

    return sprite;
}

std::vector<Frame> Sprite::returnFrames() const {
    // Returns a copy of the frames vector
    return frames;
}

int Sprite::getWidth() const {
    return width;
}

int Sprite::getHeight() const {
    return height;
}
