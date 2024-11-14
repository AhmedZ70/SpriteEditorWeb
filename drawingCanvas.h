#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>

/**
 * @brief DrawingCanvas class adjusted for WebAssembly compatibility.
 * This class handles the drawing area of the sprite editor.
 */
class DrawingCanvas : public QWidget {
    Q_OBJECT

private:
    /// @brief Current color chosen to draw with
    QColor currentColor;

    /// @brief Image representing a frame
    QImage currentImage;

    /// @brief Flag to indicate if drawing is active
    bool drawingActive;

public:
    /// @brief Constructor that sets the default current color
    explicit DrawingCanvas(QWidget *parent = nullptr);

protected:
    /// @brief Paints the canvas
    void paintEvent(QPaintEvent *event) override;

    /// @brief Handles mouse move events
    void mouseMoveEvent(QMouseEvent *event) override;

    /// @brief Handles mouse press events
    void mousePressEvent(QMouseEvent *event) override;

    /// @brief Handles mouse release events
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    /// @brief Signal that indicates that a pixel has been chosen to be colored
    /// @param point The point that the user clicked (pixel)
    /// @param color The color that the user chose to paint the pixel
    /// @param canvasWidth Width of the canvas
    /// @param canvasHeight Height of the canvas
    void requestPixelChange(const QPoint& point, const QColor& color, int canvasWidth, int canvasHeight);

    /// @brief Signal that requests an update of the new image
    void requestCurrentImage();

    /// @brief Signal to start the drawing
    void drawingStarted();

public slots:
    /// @brief Changes the current color to a new one selected by the user
    void colorChanged(QColor newColor);

    /// @brief Triggers the paint event which updates the drawing shown
    void updateDrawing();

    /// @brief Updates the current image
    /// @param image The image to display
    void receiveCurrentImage(const QImage& image);

    /// @brief Changes the cursor to indicate drawing mode
    void enterDrawingMode();

    /// @brief Changes the cursor to indicate erasing mode
    void enterErasingMode();
};

#endif // DRAWINGCANVAS_H
