#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textColor = Qt::black;
    textPosX=0;
    textPosY=0;
    textSize = 20;
    displayTextSizeToLabel(textSize);
    textThickness = 2;
    displayTextThicknessToLabel(textThickness);
    changeUserInputValue("Sample Text");

    toggleButtonsAvailability(false);
    ui->sizeSlider->setRange(0, 200);
    ui->sizeSlider->setValue(textSize);
    ui->thicknessSlider->setRange(0, 25);
    ui->thicknessSlider->setValue(textThickness);
    ui->textXSlider->setRange(0, imageLabel.size().width());
    ui->textYSlider->setRange(0, imageLabel.size().height());
    displayTextToXLabel(imageLabel.size().width()/2);
    displayTextToYLabel(imageLabel.size().height()/2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeUserInputValue(std::string value){
    userText = QString::fromStdString(value);
    ui->textEdit->setText(userText);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint imagePos = event->pos() - ui->imageLabel->pos();
    if (ui->imageLabel->geometry().contains(imagePos)) {
        setTextPosition(imagePos.x(), imagePos.y());
        updateText();
    }
}


void MainWindow::setTextPosition(int x, int y) {
    textPosX = x;
    textPosY = y;
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

    modifiedPixmap = originalPixmap;
    QPainter painter(&modifiedPixmap);
    QPen pen(textColor, textThickness);
    QFont font("Arial", textSize);

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(textPosX, textPosY, userText);

    displayImageToImageLabel(modifiedPixmap);
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
    updateText();
    displayTextSizeToLabel(value);
}

void MainWindow::on_thicknessSlider_valueChanged(int value)
{
    textThickness = value;
    updateText();
    displayTextThicknessToLabel(value);
}

void MainWindow::on_textEdit_textChanged(const QString &value)
{
    userText = value;
    updateText();
}

void MainWindow::on_textXSlider_valueChanged(int value)
{
    textPosX = value;
    updateText();
    displayTextToXLabel(value);
}

void MainWindow::on_textYSlider_valueChanged(int value)
{
    textPosY = value;
    updateText();
    displayTextToYLabel(value);
}

void MainWindow::displayTextToXLabel(int value){
    ui->textXLabel->setText("Text X: " + QString::number(value));

}
void MainWindow::displayTextToYLabel(int value){
    ui->textYLabel->setText("Text Y: " + QString::number(value));
}
