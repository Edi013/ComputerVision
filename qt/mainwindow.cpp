#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDateTime>
#include <opencv2/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->setScaledContents(true);
    toggleButtonsAvailability(false);

    ui->hueLow->setRange(0, 255);
    ui->hueLow->setValue(hueLow);
    ui->hueHigh->setRange(0, 180);
    ui->hueHigh->setValue(hueHigh);

    ui->saturationLow->setRange(0, 255);
    ui->saturationLow->setValue(saturationLow);
    ui->saturationHigh->setRange(0, 255);
    ui->saturationHigh->setValue(saturationHigh);

    ui->valueLow->setRange(0, 255);
    ui->valueLow->setValue(valueLow);
    ui->valueHigh->setRange(0, 255);
    ui->valueHigh->setValue(valueHigh);

    ui->labelValueHigh->setText(QString::number(valueHigh));
    ui->labelValueLow->setText(QString::number(valueLow));
    ui->labelSaturationHigh->setText(QString::number(saturationHigh));
    ui->labelSaturationLow->setText(QString::number(saturationLow));
    ui->labelHueHigh->setText(QString::number(hueHigh));
    ui->labelHueLow->setText(QString::number(hueLow));

    ui->xSliderROI->setValue(0);
    ui->ySliderROI->setValue(0);
    ui->roiSizeSlider->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty()) {
        originalMat = cv::imread(filePath.toStdString());
        modifiedMat = originalMat.clone();
        displayImageToImageLabel(modifiedMat);
        toggleButtonsAvailability(true);
    }

    ui->xSliderROI->setRange(0, originalMat.cols-roiSize);
    ui->ySliderROI->setRange(0, originalMat.rows-roiSize);
    ui->roiSizeSlider->setRange(0, originalMat.cols > originalMat.rows ? originalMat.rows : originalMat.cols);
    drawSquare();
}

void MainWindow::saveImage() {
    QString dateTimeString = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm");

    QString baseFileName = QString("T04_Schimbare_fundal_"+dateTimeString);

    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", baseFileName, "PNG Files (*.png)");

    if (!savePath.isEmpty()) {
        cv::imwrite(savePath.toStdString(), modifiedMat);
    }
}

void MainWindow::on_loadButton_clicked() {
    loadImage();
}

void MainWindow::on_saveButton_clicked() {
    saveImage();
}

void MainWindow::on_colorButton_clicked() {
    QColor selectedColor = QColorDialog::getColor(Qt::white, this, "Select Background Color");
    if (selectedColor.isValid()) {
        cv::Scalar color(selectedColor.blue(), selectedColor.green(), selectedColor.red());
        replaceBackgroundWithColor(color);
    }
}

void MainWindow::on_imageButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open background image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty()) {
        cv::Mat bgImage = cv::imread(filePath.toStdString());
        replaceBackgroundWithImage(bgImage);
    }
}

void MainWindow::displayImageToImageLabel(const cv::Mat &matImage) {
    QImage img(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGB888);
    ui->imageLabel->setPixmap(QPixmap::fromImage(img.rgbSwapped()).scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::toggleButtonsAvailability(bool value) {
    ui->saveButton->setEnabled(value);
    ui->colorButton->setEnabled(value);
    ui->imageButton->setEnabled(value);
}

void MainWindow::replaceBackgroundWithColor(const cv::Scalar &color) {
    if (originalMat.empty()) return;
    lastBackgroundColorUsed = color;
    lastUsed = colorMethodIdentifier;

    cv::Mat hsvROI;
    cv::cvtColor(roi, hsvROI, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsvROI, cv::Scalar(hueLow, saturationLow, valueLow), cv::Scalar(hueHigh, saturationHigh, valueHigh), mask);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat finalMask = cv::Mat::zeros(mask.size(), CV_8UC1);
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        cv::Rect boundingRect = cv::boundingRect(contour);

        if (area > 1000 && boundingRect.width / static_cast<double>(boundingRect.height) < 5) {
            cv::drawContours(finalMask, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), -1);
        }
    }

    modifiedMat = originalMat.clone();
    modifiedMat.setTo(color, modifiedMat);

    modifiedRoi = roi.clone();
    modifiedRoi.setTo(color,finalMask);

    modifiedRoi.copyTo(modifiedMat(cv::Rect(ui->xSliderROI->value(), ui->ySliderROI->value(), roi.cols, roi.rows)));

    displayImageToImageLabel(modifiedMat);
}

void MainWindow::replaceBackgroundWithImage(const cv::Mat &bgImage) {
    if (originalMat.empty() || bgImage.empty()) return;
    lastBackgroundImageUsed = bgImage;
    lastUsed = imageMethodIdentifier;

    cv::Mat resizedBg;
    cv::resize(bgImage, resizedBg, originalMat.size());

    // cv::Mat hsvImage;
    // cv::cvtColor(originalMat, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat hsvROI;
    cv::cvtColor(roi, hsvROI, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsvROI, cv::Scalar(hueLow, saturationLow, valueLow), cv::Scalar(hueHigh, saturationHigh, valueHigh), mask);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat finalMask = cv::Mat::zeros(mask.size(), CV_8UC1);
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        cv::Rect boundingRect = cv::boundingRect(contour);

        if (area > 1000 && boundingRect.width / static_cast<double>(boundingRect.height) < 5) {
            cv::drawContours(finalMask, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), -1);
        }
    }

    modifiedMat = originalMat.clone();
    resizedBg.copyTo(modifiedMat);
    cv::Mat bgImgRoiCropped = modifiedMat(cv::Rect(ui->xSliderROI->value(), ui->ySliderROI->value(), roi.cols, roi.rows)).clone();

    modifiedRoi = roi.clone();
    cv::bitwise_and(modifiedRoi, bgImgRoiCropped, modifiedRoi, finalMask);

    modifiedRoi.copyTo(modifiedMat(cv::Rect(ui->xSliderROI->value(), ui->ySliderROI->value(), roi.cols, roi.rows)));
    cv::imshow("Matrix Display", finalMask);

    displayImageToImageLabel(modifiedMat);
}

void MainWindow::displayMask(const cv::Mat &mask) {
    cv::Mat displayMask;
    mask.convertTo(displayMask, CV_8U);

    displayMask = displayMask * 255;

    cv::imshow("Mask", displayMask);
    cv::waitKey(3000);
}

void MainWindow::lastMethodUsed(){
    if(lastUsed == noneIdentifier)
        return;
    else if(lastUsed == colorMethodIdentifier)
        replaceBackgroundWithColor(lastBackgroundColorUsed);
    else if(lastUsed == imageMethodIdentifier)
        replaceBackgroundWithImage(lastBackgroundImageUsed);
}

void MainWindow::on_hueLow_valueChanged(int value)
{
    hueLow = value;
    ui->labelHueLow->setText(QString::number(hueLow));
    lastMethodUsed();
}

void MainWindow::on_hueHigh_valueChanged(int value)
{
    hueHigh = value;
    ui->labelHueHigh->setText(QString::number(hueHigh));
    lastMethodUsed();
}

void MainWindow::on_saturationLow_valueChanged(int value)
{
    saturationLow = value;
    ui->labelSaturationLow->setText(QString::number(saturationLow));
    lastMethodUsed();
}

void MainWindow::on_saturationHigh_valueChanged(int value)
{
    saturationHigh = value;
    ui->labelSaturationHigh->setText(QString::number(saturationHigh));
    lastMethodUsed();
}

void MainWindow::on_valueLow_valueChanged(int value)
{
    valueLow = value;
    ui->labelValueLow->setText(QString::number(valueLow));
    lastMethodUsed();
}

void MainWindow::on_valueHigh_valueChanged(int value)
{
    valueHigh = value;
    ui->labelValueHigh->setText(QString::number(valueHigh));
    lastMethodUsed();
}

void MainWindow::drawSquare(){
    int startX = ui->xSliderROI->value(), startY = ui->ySliderROI->value();
    cv::Point topLeft(startX, startY);
    cv::Point bottomRight(startX+roiSize, startY + roiSize);

    cv::rectangle(modifiedMat, topLeft, bottomRight, cv::Scalar(0, 255, 0), 2);
    cv::Rect roiShape(topLeft, bottomRight);
    roi = modifiedMat(roiShape);
}

void MainWindow::onSliderChange() {
    modifiedMat = originalMat.clone();
    drawSquare();

    displayImageToImageLabel(modifiedMat);
}

void MainWindow::on_xSliderROI_valueChanged(int value)
{
    onSliderChange();

}

void MainWindow::on_ySliderROI_valueChanged(int value)
{
    onSliderChange();
}

void MainWindow::on_roiSizeSlider_valueChanged(int value)
{
    roiSize = value;
    ui->xSliderROI->setRange(0, originalMat.cols-roiSize);
    ui->ySliderROI->setRange(0, originalMat.rows-roiSize);
    ui->xSliderROI->setValue(0);
    ui->ySliderROI->setValue(0);
    onSliderChange();
}

