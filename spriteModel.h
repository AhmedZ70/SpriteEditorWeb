#ifndef SPRITEMODEL_H
#define SPRITEMODEL_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <vector>
#include <QLabel>
#include "sprite.h"

/**
 * @brief SpriteModel class adjusted for WebAssembly compatibility.
 * This class manages the sprite's frames, playback, and interactions with the drawing canvas.
 */
class SpriteModel : public QObject {
    Q_OBJECT

private:
    /// @brief Instance variable of a sprite
    Sprite sprite;

    /// @brief Current frame index of the sprite
    int currentFrameIndex;

    /// @brief Current frames per second for playback
    int fps;

    /// @brief Width and height of the frames
    int spriteWidth;
    int spriteHeight;

    /// @brief Vector of all frames
    std::vector<Frame> allFrames;

    /// @brief Tracks the current frame index for playback
    int currentPlaybackFrameIndex = 0;

    /// @brief Manages the playback timing
    QTimer* playbackTimer = nullptr;

    /// @brief Label for displaying playback (since QDialog may not work)
    QLabel* playbackLabel = nullptr;

public:
    /// @brief Constructor for SpriteModel
    explicit SpriteModel(QObject *parent = nullptr);

    /// @brief Destructor for SpriteModel
    ~SpriteModel();

    /// @brief Plays the sprite animation
    void playAnimation();

    /// @brief Plays the sprite animation at its true pixel size
    void showTrueSize();

    /// @brief Adds a new frame to the sprite
    void addFrame();

    /// @brief Duplicates the current frame
    void duplicateFrame();

    /// @brief Deletes the current frame
    void deleteFrame();

    /// @brief Gets the image of the frame at the given index
    QImage getCurrentFrameImage(size_t index);

    /// @brief Gets the number of frames in the sprite
    int getFrameCount() const;

    /// @brief Sets the frames per second for playback
    void setFPS(int newVal);

    /// @brief Gets the thumbnail image of the frame at the given index
    QImage getFrameThumbnail(size_t index);

    /// @brief Gets the current frame index
    int getCurrentFrameIndex() const;

    /// @brief Sets the current frame index
    void setCurrentFrameIndex(size_t index);

    /// @brief Retrieves all frames
    std::vector<Frame> getAllFrames();

    /// @brief Serializes the sprite data to a QByteArray
    QByteArray serializeSprite() const;

signals:
    /// @brief Signal emitted when the sprite changes
    void spriteChanged();

    /// @brief Signal emitted when a sprite is loaded
    void loaded();

public slots:
    /// @brief Updates a pixel in the current frame
    void updatePixel(const QPoint& position, const QColor& color, int width, int height);

    /// @brief Sets the initial frame dimensions and creates the first frame
    void setInitialFrame(int width, int height);

    /// @brief Loads sprite data from a QByteArray
    void loadFromData(const QByteArray& data);

    /// @brief Saves sprite data to a QByteArray
    QByteArray saveToData() const;

    /// @brief Handles when drawing starts (e.g., for undo stack)
    void onDrawingStarted();

    /// @brief Undoes the last action
    void undo();

    /// @brief Redoes the last undone action
    void redo();
};

#endif // SPRITEMODEL_H
