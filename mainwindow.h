#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QListWidget>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "drawingcanvas.h"
#include "spritemodel.h"

/**
 * @brief MainWindow class adjusted with a new layout for better web app appearance.
 * Manages the main application window and user interface.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void colorSelected(const QColor &color);
    void dimensionsSet(int width, int height);
    void Load(const QByteArray &data);
    void Save(const QByteArray &data);
    void Erasing();
    void Drawing();
    void spriteUpdated();

public slots:
    void processLoadedData(const QByteArray &data);

private slots:
    void onAddFrameButtonClicked();
    void onDeleteFrameButtonClicked();
    void onEraserButtonClicked();
    void onPencilButtonClicked();
    void onDuplicateFrameButtonClicked();
    void onColorPickerClicked();
    void OnFrameListWidgetItemClicked(QListWidgetItem *item);
    void provideCurrentImage();
    void updateFpsLabel(int value);
    void onSaveTriggered();
    void onLoadTriggered();
    void updateFrameList();

private:
    void setupUI();
    void setupConnections();
    void setupInitialDimensions();
    void initializeJavaScript();

    SpriteModel *spriteEditor;
    DrawingCanvas *canvas;
    QColor lastUsedColor;

    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topToolbar;
    QHBoxLayout *bottomToolbar;
    QSplitter *centralSplitter;
    QWidget *rightPane;
    QVBoxLayout *rightLayout;
    QListWidget *framesList;
    QHBoxLayout *playbackControls;

    // Buttons and Controls
    QPushButton *pencilButton;
    QPushButton *eraserButton;
    QPushButton *colorPickerButton;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QPushButton *addFrameButton;
    QPushButton *deleteFrameButton;
    QPushButton *duplicateFrameButton;
    QPushButton *playButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *trueSizeButton;

    QSlider *fpsSlider;
    QLabel *fpsValueLabel;

    // For JavaScript integration
    void initializeJavaScriptFunctions();
};

#endif // MAINWINDOW_H
