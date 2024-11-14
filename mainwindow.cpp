#include "mainwindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QFileDialog>
#include <emscripten.h>
#include <emscripten/val.h>

/**
 * @brief MainWindow class implementation adjusted with button labels and visible save/load buttons.
 */

extern "C" {
void processLoadedData(unsigned char* data, int length);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , lastUsedColor(Qt::black)
{
    // Initialize sprite editor and canvas
    spriteEditor = new SpriteModel(this);
    canvas = new DrawingCanvas(this);

    // Set up the UI components and layout
    setupUI();

    // Set up signal-slot connections
    setupConnections();

    // Set up initial dimensions for the sprite
    setupInitialDimensions();

    // Initialize JavaScript functions for file handling
    initializeJavaScript();
}

MainWindow::~MainWindow()
{
    // No need to delete UI components manually; Qt handles it
}

void MainWindow::setupUI()
{
    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main vertical layout
    mainLayout = new QVBoxLayout(centralWidget);

    // --- Top Toolbar ---
    topToolbar = new QHBoxLayout();

    // Create tool buttons with text labels
    pencilButton = new QPushButton(QIcon(":/icons/penIcon.png"), "Pencil", this);
    eraserButton = new QPushButton(QIcon(":/icons/eraserIcon.png"), "Eraser", this);
    colorPickerButton = new QPushButton(QIcon(":/icons/colorpickerIcon.png"), "Color Picker", this);
    undoButton = new QPushButton(QIcon(":/icons/undoIcon.png"), "Undo", this);
    redoButton = new QPushButton(QIcon(":/icons/redoIcon.png"), "Redo", this);

    // Style the buttons
    pencilButton->setToolTip("Pencil Tool");
    eraserButton->setToolTip("Eraser Tool");
    colorPickerButton->setToolTip("Select Color");
    undoButton->setToolTip("Undo Last Action");
    redoButton->setToolTip("Redo Last Action");

    // Add buttons to top toolbar
    topToolbar->addWidget(pencilButton);
    topToolbar->addWidget(eraserButton);
    topToolbar->addWidget(colorPickerButton);
    topToolbar->addStretch(); // Push undo/redo to the right
    topToolbar->addWidget(undoButton);
    topToolbar->addWidget(redoButton);

    // --- Central Area ---
    centralSplitter = new QSplitter(Qt::Horizontal, this);

    // Left pane: Drawing canvas
    canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Right pane: Frames and playback controls
    rightPane = new QWidget(this);
    rightLayout = new QVBoxLayout(rightPane);

    // Frame list
    framesList = new QListWidget(this);
    framesList->setIconSize(QSize(64, 64));
    framesList->setFixedWidth(150);

    // Frame management buttons with text labels
    addFrameButton = new QPushButton(QIcon(":/icons/addFrameIcon.png"), "Add Frame", this);
    deleteFrameButton = new QPushButton(QIcon(":/icons/deleteFrameIcon.png"), "Delete Frame", this);
    duplicateFrameButton = new QPushButton(QIcon(":/icons/duplicateFrameIcon.png"), "Duplicate Frame", this);

    addFrameButton->setToolTip("Add New Frame");
    deleteFrameButton->setToolTip("Delete Selected Frame");
    duplicateFrameButton->setToolTip("Duplicate Selected Frame");

    QHBoxLayout *frameButtonsLayout = new QHBoxLayout();
    frameButtonsLayout->addWidget(addFrameButton);
    frameButtonsLayout->addWidget(deleteFrameButton);
    frameButtonsLayout->addWidget(duplicateFrameButton);

    // Playback controls
    playButton = new QPushButton(QIcon(":/icons/playIcon.png"), "Play", this);
    fpsValueLabel = new QLabel("16 FPS", this);
    fpsSlider = new QSlider(Qt::Horizontal, this);
    fpsSlider->setRange(1, 30);
    fpsSlider->setValue(16);

    playButton->setToolTip("Play Animation");

    playbackControls = new QHBoxLayout();
    playbackControls->addWidget(playButton);
    playbackControls->addWidget(fpsValueLabel);
    playbackControls->addWidget(fpsSlider);

    // Add frame list and controls to right layout
    rightLayout->addWidget(framesList);
    rightLayout->addLayout(frameButtonsLayout);
    rightLayout->addLayout(playbackControls);

    // Add panes to splitter
    centralSplitter->addWidget(canvas);
    centralSplitter->addWidget(rightPane);

    // --- Bottom Toolbar ---
    bottomToolbar = new QHBoxLayout();

    // File actions and true size preview with text labels
    saveButton = new QPushButton(QIcon(":/icons/saveIcon.png"), "Save", this);
    loadButton = new QPushButton(QIcon(":/icons/loadIcon.png"), "Load", this);
    trueSizeButton = new QPushButton("True Size Preview", this);

    saveButton->setToolTip("Save Sprite");
    loadButton->setToolTip("Load Sprite");
    trueSizeButton->setToolTip("Show True Size Preview");

    // Add buttons to bottom toolbar
    bottomToolbar->addWidget(saveButton);
    bottomToolbar->addWidget(loadButton);
    bottomToolbar->addStretch(); // Push true size button to the right
    bottomToolbar->addWidget(trueSizeButton);

    // --- Assemble Main Layout ---
    mainLayout->addLayout(topToolbar);
    mainLayout->addWidget(centralSplitter);
    mainLayout->addLayout(bottomToolbar);

    // --- Set Initial Sizes and Policies ---
    centralSplitter->setStretchFactor(0, 3); // Drawing canvas gets more space
    centralSplitter->setStretchFactor(1, 1); // Right pane gets less space

    // Adjust window properties
    setWindowTitle("Sprite Editor");
    resize(1024, 768);

    // Set initial drawing mode
    pencilButton->setEnabled(false);
    eraserButton->setEnabled(true);

    // Apply styles (optional)
    QString buttonStyle = "QPushButton { padding: 5px; }";
    pencilButton->setStyleSheet(buttonStyle);
    eraserButton->setStyleSheet(buttonStyle);
    colorPickerButton->setStyleSheet(buttonStyle);
    undoButton->setStyleSheet(buttonStyle);
    redoButton->setStyleSheet(buttonStyle);
    addFrameButton->setStyleSheet(buttonStyle);
    deleteFrameButton->setStyleSheet(buttonStyle);
    duplicateFrameButton->setStyleSheet(buttonStyle);
    playButton->setStyleSheet(buttonStyle);
    saveButton->setStyleSheet(buttonStyle);
    loadButton->setStyleSheet(buttonStyle);
    trueSizeButton->setStyleSheet(buttonStyle);
}

void MainWindow::setupConnections()
{
    // Canvas and sprite editor connections
    connect(canvas, &DrawingCanvas::requestPixelChange, spriteEditor, &SpriteModel::updatePixel);
    connect(spriteEditor, &SpriteModel::spriteChanged, canvas, &DrawingCanvas::updateDrawing);
    connect(spriteEditor, &SpriteModel::spriteChanged, this, &MainWindow::updateFrameList);
    connect(canvas, &DrawingCanvas::requestCurrentImage, this, &MainWindow::provideCurrentImage);
    connect(this, &MainWindow::colorSelected, canvas, &DrawingCanvas::colorChanged);
    connect(this, &MainWindow::dimensionsSet, spriteEditor, &SpriteModel::setInitialFrame);
    connect(spriteEditor, &SpriteModel::loaded, this, &MainWindow::updateFrameList);
    connect(canvas, &DrawingCanvas::drawingStarted, spriteEditor, &SpriteModel::onDrawingStarted);

    // Tool buttons
    connect(pencilButton, &QPushButton::clicked, this, &MainWindow::onPencilButtonClicked);
    connect(eraserButton, &QPushButton::clicked, this, &MainWindow::onEraserButtonClicked);
    connect(colorPickerButton, &QPushButton::clicked, this, &MainWindow::onColorPickerClicked);
    connect(undoButton, &QPushButton::clicked, spriteEditor, &SpriteModel::undo);
    connect(redoButton, &QPushButton::clicked, spriteEditor, &SpriteModel::redo);

    // Frame management buttons
    connect(addFrameButton, &QPushButton::clicked, this, &MainWindow::onAddFrameButtonClicked);
    connect(deleteFrameButton, &QPushButton::clicked, this, &MainWindow::onDeleteFrameButtonClicked);
    connect(duplicateFrameButton, &QPushButton::clicked, this, &MainWindow::onDuplicateFrameButtonClicked);

    // Frames list
    connect(framesList, &QListWidget::itemClicked, this, &MainWindow::OnFrameListWidgetItemClicked);

    // Playback controls
    connect(playButton, &QPushButton::clicked, spriteEditor, &SpriteModel::playAnimation);
    connect(fpsSlider, &QSlider::valueChanged, [&](int value) {
        spriteEditor->setFPS(value);
        updateFpsLabel(value);
    });

    // Bottom toolbar buttons
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveTriggered);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadTriggered);
    connect(trueSizeButton, &QPushButton::clicked, spriteEditor, &SpriteModel::showTrueSize);

    // Drawing and erasing signals
    connect(this, &MainWindow::Drawing, canvas, &DrawingCanvas::enterDrawingMode);
    connect(this, &MainWindow::Erasing, canvas, &DrawingCanvas::enterErasingMode);
}

void MainWindow::setupInitialDimensions()
{
    // For the web version, we'll use default dimensions
    int width = 64;  // Default width
    int height = 64; // Default height

    // Emit dimensions set signal
    emit dimensionsSet(width, height);
}

void MainWindow::initializeJavaScript()
{
    // Initialize JavaScript functions for file handling
    EM_ASM({
        Module.onFileSelected = function(event) {
            var file = event.target.files[0];
            if (file) {
                var reader = new FileReader();
                reader.onload = function(e) {
                    var array = new Uint8Array(e.target.result);
                    var dataPtr = Module._malloc(array.length);
                    Module.HEAPU8.set(array, dataPtr);
                    Module.ccall('processLoadedData', null, ['number', 'number'], [dataPtr, array.length]);
                    Module._free(dataPtr);
                };
                reader.readAsArrayBuffer(file);
            }
        };
    });
}

// C function to process loaded data
extern "C" void processLoadedData(unsigned char* data, int length)
{
    QByteArray spriteData(reinterpret_cast<char*>(data), length);
    QMetaObject::invokeMethod(qApp, [spriteData]() {
        MainWindow *mainWindow = qobject_cast<MainWindow*>(qApp->activeWindow());
        if (mainWindow) {
            mainWindow->processLoadedData(spriteData);
        }
    });
}

void MainWindow::processLoadedData(const QByteArray &data)
{
    emit Load(data);
}

void MainWindow::onSaveTriggered()
{
    QByteArray spriteData = spriteEditor->serializeSprite();
    QString fileName = "sprite.ssp";

    emscripten::val Uint8Array = emscripten::val::global("Uint8Array");
    emscripten::val data = Uint8Array.new_(emscripten::typed_memory_view(spriteData.size(), reinterpret_cast<unsigned char*>(spriteData.data())));

    // Create a JavaScript array containing the data
    emscripten::val dataArray = emscripten::val::array();
    dataArray.call<void>("push", data);

    // Create options object if needed (e.g., set MIME type)
    emscripten::val options = emscripten::val::object();
    options.set("type", "application/octet-stream");

    // Create the Blob object
    emscripten::val blob = emscripten::val::global("Blob").new_(dataArray, options);

    emscripten::val window = emscripten::val::global("window");
    emscripten::val URL = emscripten::val::global("URL");
    emscripten::val url = URL.call<emscripten::val>("createObjectURL", blob);

    emscripten::val document = window["document"];
    emscripten::val a = document.call<emscripten::val>("createElement", std::string("a"));
    a.set("href", url);
    a.set("download", fileName.toStdString());
    a.call<void>("click");

    // Revoke the object URL after use
    URL.call<void>("revokeObjectURL", url);
}

void MainWindow::onLoadTriggered()
{
    emscripten::val document = emscripten::val::global("document");
    emscripten::val fileInput = document.call<emscripten::val>("createElement", std::string("input"));
    fileInput.set("type", "file");
    fileInput.set("accept", ".ssp");
    fileInput.set("onchange", emscripten::val::module_property("onFileSelected"));
    fileInput.call<void>("click");
}

void MainWindow::onColorPickerClicked()
{
    // Use QColorDialog to pick a color
    QColor color = QColorDialog::getColor(lastUsedColor, this, "Select Color");
    if (color.isValid()) {
        lastUsedColor = color;
        emit colorSelected(lastUsedColor);
    }
}

void MainWindow::updateFrameList()
{
    framesList->clear();
    for (int i = 0; i < spriteEditor->getFrameCount(); ++i) {
        QImage thumbnail = spriteEditor->getFrameThumbnail(i);
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(QPixmap::fromImage(thumbnail)));
        item->setData(Qt::UserRole, QVariant::fromValue(i));
        framesList->addItem(item);
    }
}

void MainWindow::provideCurrentImage()
{
    QImage image = spriteEditor->getCurrentFrameImage(spriteEditor->getCurrentFrameIndex());
    if (image.isNull()) {
        qDebug() << "Current frame image is null";
        return;
    }
    canvas->receiveCurrentImage(image);
}

void MainWindow::OnFrameListWidgetItemClicked(QListWidgetItem *item)
{
    int frameIndex = item->data(Qt::UserRole).toInt();
    spriteEditor->setCurrentFrameIndex(frameIndex);
    emit spriteUpdated();
}

void MainWindow::onAddFrameButtonClicked()
{
    spriteEditor->addFrame();
    emit spriteUpdated();
}

void MainWindow::onDeleteFrameButtonClicked()
{
    spriteEditor->deleteFrame();
    emit spriteUpdated();
}

void MainWindow::onEraserButtonClicked()
{
    eraserButton->setEnabled(false);
    pencilButton->setEnabled(true);
    emit colorSelected(QColor(Qt::transparent));
    emit Erasing();
}

void MainWindow::onPencilButtonClicked()
{
    pencilButton->setEnabled(false);
    eraserButton->setEnabled(true);
    if (!lastUsedColor.isValid()) {
        lastUsedColor = QColor(Qt::black);
    }
    emit colorSelected(lastUsedColor);
    emit Drawing();
}

void MainWindow::onDuplicateFrameButtonClicked()
{
    spriteEditor->duplicateFrame();
    emit spriteUpdated();
}

void MainWindow::updateFpsLabel(int value)
{
    fpsValueLabel->setText(QString::number(value) + " FPS");
}
