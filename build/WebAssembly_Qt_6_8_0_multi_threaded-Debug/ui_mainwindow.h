/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionLoad;
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftPanelLayout;
    QVBoxLayout *toolsLayout;
    QToolButton *pencilButton;
    QToolButton *eraserButton;
    QPushButton *colorPicker;
    QListWidget *framesList;
    QHBoxLayout *frameControlsLayout;
    QPushButton *addFrameButton;
    QPushButton *deleteFrameButton;
    QPushButton *duplicateFrameButton;
    QHBoxLayout *undoRedoLayout;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QWidget *drawPanel;
    QVBoxLayout *rightPanelLayout;
    QPushButton *playSpriteButton;
    QPushButton *showTrueSizeButton;
    QSlider *fpsSlider;
    QLabel *fpsValueLabel;
    QToolBar *mainToolBar;
    QAction *actionSave1;
    QAction *actionLoad1;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName("actionLoad");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        leftPanelLayout = new QVBoxLayout();
        leftPanelLayout->setObjectName("leftPanelLayout");
        toolsLayout = new QVBoxLayout();
        toolsLayout->setObjectName("toolsLayout");
        pencilButton = new QToolButton(centralwidget);
        pencilButton->setObjectName("pencilButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/pencil.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pencilButton->setIcon(icon);

        toolsLayout->addWidget(pencilButton);

        eraserButton = new QToolButton(centralwidget);
        eraserButton->setObjectName("eraserButton");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/eraser.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        eraserButton->setIcon(icon1);

        toolsLayout->addWidget(eraserButton);

        colorPicker = new QPushButton(centralwidget);
        colorPicker->setObjectName("colorPicker");

        toolsLayout->addWidget(colorPicker);


        leftPanelLayout->addLayout(toolsLayout);

        framesList = new QListWidget(centralwidget);
        framesList->setObjectName("framesList");
        framesList->setMinimumSize(QSize(100, 200));

        leftPanelLayout->addWidget(framesList);

        frameControlsLayout = new QHBoxLayout();
        frameControlsLayout->setObjectName("frameControlsLayout");
        addFrameButton = new QPushButton(centralwidget);
        addFrameButton->setObjectName("addFrameButton");

        frameControlsLayout->addWidget(addFrameButton);

        deleteFrameButton = new QPushButton(centralwidget);
        deleteFrameButton->setObjectName("deleteFrameButton");

        frameControlsLayout->addWidget(deleteFrameButton);

        duplicateFrameButton = new QPushButton(centralwidget);
        duplicateFrameButton->setObjectName("duplicateFrameButton");

        frameControlsLayout->addWidget(duplicateFrameButton);


        leftPanelLayout->addLayout(frameControlsLayout);

        undoRedoLayout = new QHBoxLayout();
        undoRedoLayout->setObjectName("undoRedoLayout");
        undoButton = new QPushButton(centralwidget);
        undoButton->setObjectName("undoButton");

        undoRedoLayout->addWidget(undoButton);

        redoButton = new QPushButton(centralwidget);
        redoButton->setObjectName("redoButton");

        undoRedoLayout->addWidget(redoButton);


        leftPanelLayout->addLayout(undoRedoLayout);


        mainLayout->addLayout(leftPanelLayout);

        drawPanel = new QWidget(centralwidget);
        drawPanel->setObjectName("drawPanel");
        drawPanel->setMinimumSize(QSize(500, 500));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(drawPanel->sizePolicy().hasHeightForWidth());
        drawPanel->setSizePolicy(sizePolicy);

        mainLayout->addWidget(drawPanel);

        rightPanelLayout = new QVBoxLayout();
        rightPanelLayout->setObjectName("rightPanelLayout");
        playSpriteButton = new QPushButton(centralwidget);
        playSpriteButton->setObjectName("playSpriteButton");

        rightPanelLayout->addWidget(playSpriteButton);

        showTrueSizeButton = new QPushButton(centralwidget);
        showTrueSizeButton->setObjectName("showTrueSizeButton");

        rightPanelLayout->addWidget(showTrueSizeButton);

        fpsSlider = new QSlider(centralwidget);
        fpsSlider->setObjectName("fpsSlider");
        fpsSlider->setOrientation(Qt::Vertical);
        fpsSlider->setMinimum(1);
        fpsSlider->setMaximum(120);
        fpsSlider->setValue(30);

        rightPanelLayout->addWidget(fpsSlider);

        fpsValueLabel = new QLabel(centralwidget);
        fpsValueLabel->setObjectName("fpsValueLabel");

        rightPanelLayout->addWidget(fpsValueLabel);


        mainLayout->addLayout(rightPanelLayout);

        MainWindow->setCentralWidget(centralwidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        actionSave1 = new QAction(mainToolBar);
        actionSave1->setObjectName("actionSave1");
        actionLoad1 = new QAction(mainToolBar);
        actionLoad1->setObjectName("actionLoad1");
        MainWindow->addToolBar(mainToolBar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        mainToolBar->addAction(actionSave1);
        mainToolBar->addAction(actionLoad);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Sprite Editor", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionLoad->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
        pencilButton->setText(QCoreApplication::translate("MainWindow", "Pencil", nullptr));
        eraserButton->setText(QCoreApplication::translate("MainWindow", "Eraser", nullptr));
        colorPicker->setText(QCoreApplication::translate("MainWindow", "Color Picker", nullptr));
        addFrameButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        deleteFrameButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        duplicateFrameButton->setText(QCoreApplication::translate("MainWindow", "Duplicate", nullptr));
        undoButton->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
        redoButton->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
        playSpriteButton->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        showTrueSizeButton->setText(QCoreApplication::translate("MainWindow", "Show True Size", nullptr));
        fpsValueLabel->setText(QCoreApplication::translate("MainWindow", "FPS: 30", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "Main Toolbar", nullptr));
        actionSave1->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionLoad1->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
