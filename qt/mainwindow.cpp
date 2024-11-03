#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDateTime>

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
    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Files (*.png)");
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

void MainWindow::replaceBackgroundWithColor(const cv::Scalar &color) {
    if (originalMat.empty()) return;

    // Split the image into R, G, and B channels
    std::vector<cv::Mat> channels;
    cv::split(originalMat, channels);

    // Calculate histograms for each channel
    cv::Mat histR, histG, histB;
    int histSize = 256;  // Number of bins
    float range[] = {0, 256};  // The range for intensity values
    const float *histRange = {range};
    cv::calcHist(&channels[2], 1, 0, cv::Mat(), histR, 1, &histSize, &histRange);  // Red
    cv::calcHist(&channels[1], 1, 0, cv::Mat(), histG, 1, &histSize, &histRange);  // Green
    cv::calcHist(&channels[0], 1, 0, cv::Mat(), histB, 1, &histSize, &histRange);  // Blue

    // Define thresholds based on histogram peaks
    int lowThresh = 0;
    int highThresh = 100;  // You can dynamically adjust based on histogram data for a more adaptive approach

    // Create a mask based on color range derived from histograms
    cv::Mat mask;
    cv::inRange(originalMat, cv::Scalar(lowThresh, lowThresh, lowThresh),
                cv::Scalar(highThresh, highThresh, highThresh), mask);

    // Replace the background with the selected color
    modifiedMat = originalMat.clone();
    modifiedMat.setTo(color, mask);
    displayImageToImageLabel(modifiedMat);
}

void MainWindow::replaceBackgroundWithImage(const cv::Mat &bgImage) {
    if (originalMat.empty() || bgImage.empty()) return;

    cv::Mat resizedBg;
    cv::resize(bgImage, resizedBg, originalMat.size());

    // Use the same histogram-based mask to blend with the background image
    cv::Mat mask;
    int lowThresh = 0;
    int highThresh = 100;
    cv::inRange(originalMat, cv::Scalar(lowThresh, lowThresh, lowThresh),
                cv::Scalar(highThresh, highThresh, highThresh), mask);

    modifiedMat = originalMat.clone();
    resizedBg.copyTo(modifiedMat, mask);
    displayImageToImageLabel(modifiedMat);
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
