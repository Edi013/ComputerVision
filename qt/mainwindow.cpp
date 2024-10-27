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

    textSize = 10;
    displayTextSizeToLabel(textSize);
    textThickness = 10;
    displayTextThicknessToLabel(textThickness);
    toggleButtonsAvailability(false);
    ui->sizeSlider->setRange(1, 1000);
    ui->thicknessSlider->setRange(1, 1000);
}


MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::setTextPosition(int x, int y) {
    textPosX = x;
    textPosY = y;
    updateText();
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

void MainWindow::saveImage()
{
    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!savePath.isEmpty()) {
        modifiedPixmap.save(savePath);
    }
}

void MainWindow::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open image"), ".",
                                                    tr("Image FIles ((*.png *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty()) {
        originalPixmap.load(filePath);
        modifiedPixmap = originalPixmap;
        displayImageToImageLabel(modifiedPixmap);
    }

    toggleButtonsAvailability(true);
}

void MainWindow::displayImageToImageLabel(QPixmap image) {
    ui->imageLabel->setPixmap(image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::toggleButtonsAvailability(bool value) {
    ui->saveButton->setDisabled(!value);
    ui->colorButton->setDisabled(!value);
    ui->textEdit->setDisabled(!value);
}

void MainWindow::displayTextSizeToLabel(int value){
    ui->textSizeLabel->setText(QString("Text Size: %1").arg(value));
}

void MainWindow::displayTextThicknessToLabel(int value){
    ui->textThicknessLabel->setText(QString("Text Size: %1").arg(value));
}

void MainWindow::on_saveButton_clicked()
{
    saveImage();
}

void MainWindow::on_loadButton_clicked()
{
    loadImage();
}

void MainWindow::on_colorButton_clicked()
{
    QColor selectedColor = QColorDialog::getColor(textColor, this, tr("Select Text Color"));

    if (selectedColor.isValid()) {
        textColor = selectedColor;
        updateText();
    }
}

void MainWindow::on_sizeSlider_valueChanged(int value)
{
    textSize = value;
    displayTextSizeToLabel(value);
}

void MainWindow::on_thicknessSlider_valueChanged(int value)
{
    textThickness = value;
    displayTextThicknessToLabel(value);
}

