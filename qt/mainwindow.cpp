#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    textColor(Qt::black),
    textSize(20),
    textThickness(2),
    textPosX(0),
    textPosY(0)
{
    ui->setupUi(this);

    imageLabel = new QLabel(this);
    textEdit = new QLineEdit(this);
    sizeSlider = new QSlider(Qt::Horizontal, this);
    thicknessSlider = new QSlider(Qt::Horizontal, this);
    colorButton = new QPushButton("Choose Color", this);
    loadButton = new QPushButton("Load Image", this);
    saveButton = new QPushButton("Save Image", this);

    // Set slider ranges
    sizeSlider->setRange(10, 100);
    thicknessSlider->setRange(1, 10);

    // Set layout (skipping detailed layout code for simplicity)

    // // Connect signals to slots
    // connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    // connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
    // connect(textEdit, &QLineEdit::textChanged, this, &MainWindow::updateText);
    // connect(sizeSlider, &QSlider::valueChanged, this, &MainWindow::updateTextAttributes);
    // connect(thicknessSlider, &QSlider::valueChanged, this, &MainWindow::updateTextAttributes);
    // connect(colorButton, &QPushButton::clicked, [=]() {
    //     textColor = QColorDialog::getColor(Qt::black, this);
    //     updateText();
    // });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open image"), ".",
                                                    tr("Image FIles ((*.png *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty()) {
        originalPixmap.load(filePath);
        modifiedPixmap = originalPixmap;
        imageLabel->setPixmap(modifiedPixmap);
    }
}

void MainWindow::setTextPosition(int x, int y) {
    textPosX = x;
    textPosY = y;
    updateText();  // Optionally call updateText to refresh the displayed text position
}


void MainWindow::updateText()
{
    userText = textEdit->text();
    updateTextAttributes();
}

void MainWindow::updateTextAttributes()
{
    // Update text size and thickness
    textSize = sizeSlider->value();
    textThickness = thicknessSlider->value();

    // Render text on image
    modifiedPixmap = originalPixmap; // Reset to original for re-rendering
    QPainter painter(&modifiedPixmap);
    QPen pen(textColor, textThickness);
    QFont font("Arial", textSize);

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(textPosX, textPosY, userText);

    imageLabel->setPixmap(modifiedPixmap);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (imageLabel->geometry().contains(event->pos())) {
        QPoint imagePos = event->pos() - imageLabel->pos(); // Relative position within QLabel
        textPosX = imagePos.x();
        textPosY = imagePos.y();
        updateText();
    }
}

void MainWindow::saveImage()
{
    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!savePath.isEmpty()) {
        modifiedPixmap.save(savePath);
    }
}

void MainWindow::on_saveButton_clicked()
{
    saveImage();
}


void MainWindow::on_loadButton_clicked()
{
    loadImage();
}

