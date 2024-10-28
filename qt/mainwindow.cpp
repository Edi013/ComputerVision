#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textSize = 32;
    ui->sizeSlider->setRange(1, 200);
    ui->sizeSlider->setValue(textSize);
    displayTextSizeToLabel(textSize);

    textThickness = 20;
    ui->thicknessSlider->setRange(1, 25);
    ui->thicknessSlider->setValue(textThickness);
    displayTextThicknessToLabel(textThickness);

    textPosX = 0;
    textPosY = 0;
    ui->textXSlider->setRange(0,0);
    ui->textYSlider->setRange(0,0);
    textColor = Qt::white;
    changeUserInputValue("Sample Text");
    toggleButtonsAvailability(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ui->imageLabel->geometry().contains(event->pos())) {

        QPoint localPos = ui->imageLabel->mapFromParent(event->pos());

        int maxWidth = ui->imageLabel->width();
        int maxHeight = ui->imageLabel->height();

        int scaleX = 1000;
        int scaleY = 1000;
        double progressionFactor = 1.5;

        int progressiveX = static_cast<int>(pow(static_cast<double>(localPos.x()) / maxWidth, progressionFactor) * scaleX);
        int progressiveY = static_cast<int>(pow(static_cast<double>(localPos.y()) / maxHeight, progressionFactor) * scaleY);

        ui->textXSlider->setValue(progressiveX);
        ui->textYSlider->setValue(progressiveY);
    }
}



void MainWindow::saveImage() {
    QString dateTimeNow = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    std::string fileName = "T03.Image_" + dateTimeNow.toStdString() + ".png";
    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", QString::fromStdString(fileName), "PNG Files (*.png)");

    if (!savePath.isEmpty()) {
        std::string filePath = savePath.toStdString();
        cv::imwrite(filePath, modifiedMat);
    }
}

void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open image"), ".", tr("Image FIles (*.png *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty()) {
        originalMat = cv::imread(filePath.toStdString());
        modifiedMat = originalMat.clone();
        updateSlidersWithMatSize(modifiedMat);
        displayImageToImageLabel(modifiedMat);
    }
    toggleButtonsAvailability(true);
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

void MainWindow::updateTextAttributes() {
    textSize = ui->sizeSlider->value();
    textThickness = ui->thicknessSlider->value();

    modifiedMat = originalMat.clone();
    cv::Scalar cvTextColor(textColor.blue(), textColor.green(), textColor.red());
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;

    cv::putText(modifiedMat, userText.toStdString(), cv::Point(textPosX, textPosY),
                fontFace, textSize / 20.0, cvTextColor, textThickness);

    displayImageToImageLabel(modifiedMat);
}

void MainWindow::updateSlidersWithMatSize(const cv::Mat &mat) {
    if (mat.empty()) return;

    ui->textXSlider->setRange(10, mat.cols - 10); // Range from 10 to width - 10
    ui->textXSlider->setValue(textPosX);
    displayTextToXLabel(textPosX);

    ui->textYSlider->setRange(10, mat.rows - 10); // Range from 10 to height - 10
    ui->textYSlider->setValue(textPosY);
    displayTextToYLabel(textPosY);
}

void MainWindow::changeUserInputValue(std::string value){
    userText = QString::fromStdString(value);
    ui->textEdit->setText(userText);
}

void MainWindow::toggleButtonsAvailability(bool value) {
    ui->saveButton->setDisabled(!value);
    ui->colorButton->setDisabled(!value);
    ui->textEdit->setDisabled(!value);
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



void MainWindow::displayTextSizeToLabel(int value){
    ui->textSizeLabel->setText(QString("Text Size: %1").arg(value));
}

void MainWindow::displayTextThicknessToLabel(int value){
    ui->textThicknessLabel->setText(QString("Text Size: %1").arg(value));
}

void MainWindow::displayTextToXLabel(int value){
    ui->textXLabel->setText("Text X: " + QString::number(value));

}

void MainWindow::displayTextToYLabel(int value){
    ui->textYLabel->setText("Text Y: " + QString::number(value));
}

void MainWindow::displayImageToImageLabel(const cv::Mat& matImage) {
    ui->imageLabel->setPixmap(CvMatToQPixmap(matImage).scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QPixmap MainWindow::CvMatToQPixmap(const cv::Mat &mat) {
    QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return QPixmap::fromImage(img.rgbSwapped());
}

