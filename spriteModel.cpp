#include "spritemodel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

/**
 * @brief SpriteModel class implementation adjusted for WebAssembly compatibility.
 */

SpriteModel::SpriteModel(QObject *parent)
    : QObject(parent), sprite(), currentFrameIndex(0), fps(16), spriteWidth(64), spriteHeight(64) {
    // Initialize with a default frame
    addFrame();
}

SpriteModel::~SpriteModel() {
    delete playbackTimer;
    delete playbackLabel;
}

void SpriteModel::addFrame() {
    // Add a new empty frame to the sprite
    sprite.addFrame(Frame(spriteWidth, spriteHeight));
    setCurrentFrameIndex(sprite.frameCount() - 1);
    emit spriteChanged();
}

void SpriteModel::duplicateFrame() {
    // Duplicate the current frame
    if (currentFrameIndex < sprite.frameCount()) {
        Frame currentFrame = sprite.getFrame(currentFrameIndex);
        sprite.insertFrame(currentFrameIndex + 1, currentFrame);
        setCurrentFrameIndex(currentFrameIndex + 1);
        emit spriteChanged();
    }
}

void SpriteModel::deleteFrame() {
    // Delete the current frame if there is more than one frame
    if (sprite.frameCount() > 1) {
        sprite.removeFrame(currentFrameIndex);
        if (currentFrameIndex >= sprite.frameCount()) {
            currentFrameIndex = sprite.frameCount() - 1;
        }
        emit spriteChanged();
    }
}

void SpriteModel::setCurrentFrameIndex(size_t index) {
    if (index < sprite.frameCount()) {
        currentFrameIndex = index;
        emit spriteChanged();
    }
}

int SpriteModel::getCurrentFrameIndex() const {
    return currentFrameIndex;
}

QImage SpriteModel::getCurrentFrameImage(size_t index) {
    if (index < sprite.frameCount()) {
        return sprite.getFrame(index).getImage();
    }
    return QImage();
}

void SpriteModel::updatePixel(const QPoint& canvasPoint, const QColor& color, int width, int height) {
    // Map the canvas coordinates to the frame coordinates
    QPoint scaledPosition(canvasPoint.x() * spriteWidth / width,
                          canvasPoint.y() * spriteHeight / height);

    if (currentFrameIndex < sprite.frameCount()) {
        Frame& currentFrame = sprite.getFrame(currentFrameIndex);
        currentFrame.setPixel(scaledPosition.x(), scaledPosition.y(), color);
        emit spriteChanged();
    }
}

int SpriteModel::getFrameCount() const {
    return sprite.frameCount();
}

QImage SpriteModel::getFrameThumbnail(size_t index) {
    if (index < sprite.frameCount()) {
        return sprite.getFrame(index).getImage().scaled(64, 64, Qt::KeepAspectRatio, Qt::FastTransformation);
    }
    return QImage();
}

void SpriteModel::setInitialFrame(int width, int height) {
    spriteWidth = width;
    spriteHeight = height;
    // Clear existing frames and add a new one
    sprite = Sprite(spriteWidth, spriteHeight);
    addFrame();
}

std::vector<Frame> SpriteModel::getAllFrames() {
    return sprite.returnFrames();
}

void SpriteModel::playAnimation() {
    // Display the animation using a QLabel in the main window
    if (!playbackLabel) {
        playbackLabel = new QLabel();
        playbackLabel->setWindowTitle("Animation Preview");
        playbackLabel->setAlignment(Qt::AlignCenter);
        playbackLabel->setAttribute(Qt::WA_DeleteOnClose);
        playbackLabel->setFixedSize(400, 400);

        allFrames = getAllFrames();
        currentPlaybackFrameIndex = 0;

        // Create a timer to handle frame updates
        playbackTimer = new QTimer(this);
        connect(playbackTimer, &QTimer::timeout, this, [this]() {
            if (!allFrames.empty()) {
                QImage img = allFrames[currentPlaybackFrameIndex].getImage();
                if (!img.isNull()) {
                    // Scale the image to fit the label
                    img = img.scaled(playbackLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
                    playbackLabel->setPixmap(QPixmap::fromImage(img));
                }
                currentPlaybackFrameIndex = (currentPlaybackFrameIndex + 1) % allFrames.size();
            }
        });

        // Start the timer
        playbackTimer->start(1000 / fps);

        // Ensure resources are cleaned up when the label is closed
        connect(playbackLabel, &QLabel::destroyed, this, [this]() {
            playbackTimer->stop();
            playbackTimer->deleteLater();
            playbackTimer = nullptr;
            playbackLabel = nullptr;
        });

        playbackLabel->show();
    }
}

void SpriteModel::showTrueSize() {
    // Similar to playAnimation but without scaling the image
    if (!playbackLabel) {
        playbackLabel = new QLabel();
        playbackLabel->setWindowTitle("True Size Preview");
        playbackLabel->setAlignment(Qt::AlignCenter);
        playbackLabel->setAttribute(Qt::WA_DeleteOnClose);

        allFrames = getAllFrames();
        currentPlaybackFrameIndex = 0;

        // Adjust the label size to match the sprite dimensions
        playbackLabel->setFixedSize(spriteWidth, spriteHeight);

        // Create a timer to handle frame updates
        playbackTimer = new QTimer(this);
        connect(playbackTimer, &QTimer::timeout, this, [this]() {
            if (!allFrames.empty()) {
                QImage img = allFrames[currentPlaybackFrameIndex].getImage();
                if (!img.isNull()) {
                    playbackLabel->setPixmap(QPixmap::fromImage(img));
                }
                currentPlaybackFrameIndex = (currentPlaybackFrameIndex + 1) % allFrames.size();
            }
        });

        // Start the timer
        playbackTimer->start(1000 / fps);

        // Ensure resources are cleaned up when the label is closed
        connect(playbackLabel, &QLabel::destroyed, this, [this]() {
            playbackTimer->stop();
            playbackTimer->deleteLater();
            playbackTimer = nullptr;
            playbackLabel = nullptr;
        });

        playbackLabel->show();
    }
}

void SpriteModel::onDrawingStarted() {
    // Take a snapshot for undo functionality
    if (currentFrameIndex < sprite.frameCount()) {
        sprite.getFrame(currentFrameIndex).takeSnapshot();
    }
}

void SpriteModel::setFPS(int newVal) {
    fps = newVal;
    if (playbackTimer && playbackTimer->isActive()) {
        playbackTimer->setInterval(1000 / fps);
    }
}

void SpriteModel::undo() {
    if (currentFrameIndex < sprite.frameCount()) {
        sprite.getFrame(currentFrameIndex).undo();
        emit spriteChanged();
    }
}

void SpriteModel::redo() {
    if (currentFrameIndex < sprite.frameCount()) {
        sprite.getFrame(currentFrameIndex).redo();
        emit spriteChanged();
    }
}

QByteArray SpriteModel::serializeSprite() const {
    QJsonObject jsonObj = sprite.toJson();
    QJsonDocument doc(jsonObj);
    return doc.toJson();
}

void SpriteModel::loadFromData(const QByteArray& data) {
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject jsonObj = doc.object();
    Sprite newSprite = Sprite::fromJson(jsonObj);
    sprite = newSprite;
    spriteWidth = sprite.getWidth();
    spriteHeight = sprite.getHeight();
    currentFrameIndex = 0;
    emit spriteChanged();
    emit loaded();
}

QByteArray SpriteModel::saveToData() const {
    return serializeSprite();
}
