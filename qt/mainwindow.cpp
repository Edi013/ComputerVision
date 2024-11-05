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
    ui->hueHigh->setRange(0, 180);

    ui->saturationLow->setRange(0, 255);
    ui->saturationHigh->setRange(0, 255);

    ui->valueLow->setRange(0, 255);
    ui->valueHigh->setRange(0, 255);
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
}

void MainWindow::saveImage() {
    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", "T04.Schimbare-fundal", "PNG Files (*.png)");
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

    // Step 1: Convert the image to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(originalMat, hsvImage, cv::COLOR_BGR2HSV);

    // Step 2: Create a mask for bright, low-saturation regions (background)
    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(0, 0, 220), cv::Scalar(180, 40, 255), mask);

    // Step 4: Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Step 5: Create a refined mask to isolate the background areas
    cv::Mat finalMask = cv::Mat::zeros(mask.size(), CV_8UC1);
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        cv::Rect boundingRect = cv::boundingRect(contour);

        // Filter based on contour area and aspect ratio to avoid small objects or misclassified regions
        if (area > 1000 && boundingRect.width / static_cast<double>(boundingRect.height) < 5) { // adjust as needed
            cv::drawContours(finalMask, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), -1);
        }
    }

    // Step 6: Set the background color in the modified image based on refined mask
    modifiedMat = originalMat.clone();
    modifiedMat.setTo(color, finalMask);  // Apply the color to areas marked as background

    // Optional: Display the mask for debugging
    displayMask(finalMask);

    // Step 7: Display the updated image
    displayImageToImageLabel(modifiedMat);
}



void MainWindow::replaceBackgroundWithImage(const cv::Mat &bgImage) {
    if (originalMat.empty() || bgImage.empty()) return;

    // Step 1: Resize the background image to match the original image's dimensions
    cv::Mat resizedBg;
    cv::resize(bgImage, resizedBg, originalMat.size());

    // Step 2: Convert the image to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(originalMat, hsvImage, cv::COLOR_BGR2HSV);

    // Step 3: Create a mask for bright, low-saturation regions (background)
    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(0, 0, 220), cv::Scalar(180, 40, 255), mask);

    // Step 5: Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Step 6: Create a refined mask to isolate the background areas
    cv::Mat finalMask = cv::Mat::zeros(mask.size(), CV_8UC1);
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        cv::Rect boundingRect = cv::boundingRect(contour);

        // Filter based on contour area and aspect ratio to avoid small objects or misclassified regions
        if (area > 1000 && boundingRect.width / static_cast<double>(boundingRect.height) < 5) { // adjust as needed
            cv::drawContours(finalMask, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), -1);
        }
    }

    // Step 7: Replace background with the resized background image
    modifiedMat = originalMat.clone();
    resizedBg.copyTo(modifiedMat, finalMask);  // Overlay the background image onto areas marked as background

    // Optional: Display the mask for debugging
    displayMask(finalMask);

    // Step 8: Display the updated image
    displayImageToImageLabel(modifiedMat);
}


void MainWindow::displayMask(const cv::Mat &mask) {
    cv::Mat displayMask;
    mask.convertTo(displayMask, CV_8U);

    displayMask = displayMask * 255;

    cv::imshow("Mask", displayMask);
    cv::waitKey(3000);
}



void MainWindow::on_hueLow_valueChanged(int value)
{
    hueLow = value;
}

void MainWindow::on_hueHigh_valueChanged(int value)
{
    hueHigh = value;
}

void MainWindow::on_saturationLow_valueChanged(int value)
{
    saturationLow = value;
}

void MainWindow::on_saturationHigh_valueChanged(int value)
{
    saturationHigh = value;
}

void MainWindow::on_valueLow_valueChanged(int value)
{
    valueLow = value;
}

void MainWindow::on_valueHigh_valueChanged(int value)
{
    valueHigh = value;
}

