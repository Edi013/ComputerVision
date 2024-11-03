#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDateTime>
#include <opencv2/imgproc.hpp> // For histogram and threshold functions

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
    saveImage();  // Use the custom saveImage method here
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

    // Create a mask for white areas in the image
    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(0, 0, 200), cv::Scalar(180, 20, 255), mask); // Adjust HSV range for white

    // Expand mask to cover more background area
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
    cv::dilate(mask, mask, kernel);

    // Set the background color in the modified image
    modifiedMat = originalMat.clone();
    modifiedMat.setTo(color, mask); // Apply the color to the areas identified by the mask

    displayImageToImageLabel(modifiedMat);
}

void MainWindow::replaceBackgroundWithImage(const cv::Mat &bgImage) {
    if (originalMat.empty() || bgImage.empty()) return;

    // Convert the image to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(originalMat, hsvImage, cv::COLOR_BGR2HSV);

    // Create a mask for white areas in the image
    cv::Mat mask;
    cv::inRange(hsvImage, cv::Scalar(0, 0, 200), cv::Scalar(180, 20, 255), mask); // Adjust HSV range for white

    // Expand mask to cover more background area
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
    cv::dilate(mask, mask, kernel);

    // Resize the background image to match the original size
    cv::Mat resizedBg;
    cv::resize(bgImage, resizedBg, originalMat.size());

    // Replace the background in the modified image
    modifiedMat = originalMat.clone();
    resizedBg.copyTo(modifiedMat, mask); // Use the mask to overlay the background image

    displayImageToImageLabel(modifiedMat);
}
