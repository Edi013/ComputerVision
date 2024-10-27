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

    //ui->sizeSlider->setRange(10, 100);
    //ui->thicknessSlider->setRange(1, 10);
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
        ui->imageLabel->setPixmap(modifiedPixmap, Qt::KeepAspectRatio);
    }
}

void MainWindow::setTextPosition(int x, int y) {
    textPosX = x;
    textPosY = y;
    updateText();  // Optionally call updateText to refresh the displayed text position
}


void MainWindow::updateText()
{
    userText = ui->textEdit->text();
    updateTextAttributes();
}

void MainWindow::updateTextAttributes()
{
    textSize = ui->sizeSlider->value();
    textThickness = ui->thicknessSlider->value();

    modifiedPixmap = originalPixmap.copy();
    QPainter painter(&modifiedPixmap);
    QPen pen(textColor, textThickness);
    QFont font("Arial", textSize);

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(textPosX, textPosY, userText);

    ui->imageLabel->setPixmap(modifiedPixmap);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ui->imageLabel->geometry().contains(event->pos())) {
        QPoint imagePos = event->pos() - ui->imageLabel->pos(); // Relative position within QLabel
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

