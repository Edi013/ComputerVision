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

    // Convert the image to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(originalMat, hsvImage, cv::COLOR_BGR2HSV);

    // Create a mask for bright regions
    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(0, 0, 200), cv::Scalar(180, 50, 255), mask);

    // // Morphological operation to close small holes
    // cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    // cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Create a new mask to isolate the area of interest (the cat)
    cv::Mat finalMask = cv::Mat::zeros(mask.size(), CV_8UC1);

    // Filter and draw relevant contours to the final mask
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > 500) { // Adjust based on your image
            cv::drawContours(finalMask, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), -1);
        }
    }

    // Set the background color in the modified image
    modifiedMat = originalMat.clone();

    modifiedMat.setTo(color, finalMask); // Apply the color to the identified areas
    displayMask(finalMask);
    // Display the updated image
    displayImageToImageLabel(modifiedMat);
}



void MainWindow::replaceBackgroundWithImage(const cv::Mat &bgImage) {
    if (originalMat.empty() || bgImage.empty()) return;

    cv::Mat hsvImage;
    cv::cvtColor(originalMat, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(0, 0, 200), cv::Scalar(180, 50, 255), mask);

    // Optionally apply morphological operations to improve the mask
    //cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    //cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel); // Close small holes in the mask

    // Resize the background image to match the original size
    cv::Mat resizedBg;
    cv::resize(bgImage, resizedBg, originalMat.size());

    // Replace the background in the modified image
    modifiedMat = originalMat.clone();
    resizedBg.copyTo(modifiedMat, mask); // Use the mask to overlay the background image

    displayImageToImageLabel(modifiedMat);
}

void MainWindow::displayMask(const cv::Mat &mask) {
    // Convert the mask to an 8-bit single channel image for visualization
    cv::Mat displayMask;
    mask.convertTo(displayMask, CV_8U); // Convert to 8-bit format (0-255)

    // Scale the values to be visible in the range [0, 255]
    displayMask = displayMask * 255;

    // Display the mask
    cv::imshow("Mask", displayMask);
    cv::waitKey(0); // Wait indefinitely for a key press
}
