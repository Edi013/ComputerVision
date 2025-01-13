#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadVideoButton_clicked()
{
    videoFilePath = QFileDialog::getOpenFileName(this, "Select Video File", "", "Video Files (*.mp4 *.avi *.mkv)").toStdString();
    if (videoFilePath.length() == 0) {
        QMessageBox::warning(this, "Warning", "No video file selected.");
        return;
    }
    QMessageBox::information(this, "Success", "Video file loaded successfully!");
}

void MainWindow::on_processVideoButton_clicked()
{
    if (videoFilePath.empty()) {
        QMessageBox::warning(this, "Error", "No video file loaded.");
        return;
    }

    cv::VideoCapture cap(videoFilePath);
    if (!cap.isOpened()) {
        QMessageBox::critical(this, "Error", "Unable to open video file.");
        return;
    }

    std::string modelPath = "E:\\Projects\\qt_proj\\qt\\yolov11-segmentation.onnx"
;
    cv::dnn::Net net = cv::dnn::readNetFromONNX(modelPath);

    cv::Size frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int fps = cap.get(cv::CAP_PROP_FPS);
    outputFilePath = videoFilePath + "_processed.avi";
    cv::VideoWriter writer(outputFilePath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frameSize);

    cv::Mat frame;
    while (cap.read(frame)) {
        processFrame(frame);
        writer.write(frame);
    }

    cap.release();
    writer.release();
    QMessageBox::information(this, "Success", "Video processed successfully!");
}

void MainWindow::on_saveVideoButton_clicked()
{
    if (outputFilePath.empty()) {
        QMessageBox::warning(this, "Error", "No processed video to save.");
        return;
    }
    QString savePath = QFileDialog::getSaveFileName(this, "Save Video", "output.avi", "AVI Files (*.avi)");
    if (!savePath.isEmpty()) {
        QFile::copy(QString::fromStdString(outputFilePath), savePath);
        QMessageBox::information(this, "Success", "Video saved successfully!");
    }
}

void MainWindow::processFrame(cv::Mat &frame)
{
    cv::dnn::Net net = cv::dnn::readNet("E:\\Projects\\qt_proj\\qt\\yolov11-segmentation.onnx"
);
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0 / 255.0, cv::Size(640, 640), cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    drawSegmentation(frame, outputs);
}

void MainWindow::drawSegmentation(cv::Mat &frame, const std::vector<cv::Mat> &masks)
{
    for (const auto &mask : masks) {
        cv::Mat binaryMask;
        cv::threshold(mask, binaryMask, 0.5, 255, cv::THRESH_BINARY);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binaryMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (const auto &contour : contours) {
            cv::polylines(frame, contour, true, cv::Scalar(0, 255, 0), 2);
        }
    }
}

