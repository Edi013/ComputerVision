#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->executeButton->setDisabled(true);
    ui->playVideoButton->setDisabled(true);
    ui->saveVideoButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectInOutFoldersButton_clicked()
{
    // sourceFolder = QFileDialog::getExistingDirectory(this, tr("Select Source Folder"), "");
    // if (sourceFolder.isEmpty())
    // {
    //     showUserInfo("Source: " + sourceFolder + " was NOT selected.");
    // }else
    //     showUserInfo("Source: " + sourceFolder + " was selected successfully.");

    // destinationFolder = QFileDialog::getExistingDirectory(this, tr("Select Destination Folder"), "");
    // if (destinationFolder.isEmpty())
    // {
    //     showUserInfo("Destination: " + destinationFolder + " was NOT selected.");
    // }else
    //     showUserInfo("Destination: " + destinationFolder + " was selected successfully.");
    sourceFolder =QStringLiteral("E:\\Projects\\qt_proj\\qt\\images");
    destinationFolder = sourceFolder;

    if(sourceFolder.isEmpty() || destinationFolder.isEmpty()){
        showUserInfo("Operation will be repeted. Select the source and destination again. For EXIT select any folders, than press x in the main window.");
        on_selectInOutFoldersButton_clicked();
        return;
    }

    ui->executeButton->setDisabled(false);
    ui->playVideoButton->setDisabled(false);
    ui->saveVideoButton->setDisabled(false);
}

void MainWindow::on_executeButton_clicked()
{
    if (sourceFolder.isEmpty() || destinationFolder.isEmpty())
    {
        showUserInfo("Please select source and destination folders first.");
        return;
    }

    showUserInfo("Processing started.");
    double initialTime = (double)getTickCount();

    readImages();
    buildVideo();

    double finalTime = ((double)getTickCount()-initialTime)/getTickFrequency();
    showUserInfo("Processing completed in " + QString::number(finalTime) + " seconds.");
}

void MainWindow::on_playVideoButton_clicked(){
    if (result.empty()) {
        std::cerr << "No images to write!" << std::endl;
        showUserInfo("No images to transform in a video.");
        return;
    }

    showUserInfo("Playing video.");
    double initialTime = (double)getTickCount();
    for(int i=0; i<result.size(); i++){
        displayImageToImageLabel(result[i]);
        waitKey(3000);
    }
    double finalTime = ((double)getTickCount()-initialTime)/getTickFrequency();
    showUserInfo("Video ended. It lasted "+ QString::number(finalTime) + " seconds.");
}

void MainWindow::on_saveVideoButton_clicked()
{
    if (result.empty()) {
        std::cerr << "No images to write!" << std::endl;
        showUserInfo("No images to transform in a video.");
        return;
    }else{
        std::cerr<<"Input vector has " + std::to_string(result.size()) << " frames" << std::endl;
    }

    string fileName = "T02.Dialog.avi";
    string outputPath = destinationFolder.toStdString() + "/" + fileName;

    int aviCodec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    //int mp4Codec = cv::VideoWriter::fourcc('X', '2', '6', '4');

    cv::Size frameSize(result[0].cols, result[0].rows);
    cv::VideoWriter writer(outputPath, aviCodec, 0.5, frameSize, true);

    if (!writer.isOpened()) {
        std::cerr << "Could not open the video file for writing!" << std::endl;
        return;
    }

    for (const cv::Mat& img : result) {
        writer.write( img);
    }

    writer.release();
    showUserInfo("Video saved successfully to " + destinationFolder + " as " + QString::fromStdString(fileName));
}



void MainWindow::readImages()
{
    int contor = 0;
    for (const auto& entry : fs::directory_iterator(sourceFolder.toStdString()))
    {
        if (entry.is_regular_file())
        {
            string filePath = entry.path().string();
            Mat image = imread(filePath, cv::COLOR_RGB2BGR);

            if (image.empty()) {
                showUserInfo("Error: One image could not be loaded: " + QString::fromStdString(filePath) );
                continue;
            }

            if(contor==0){
                scenery = image;
            }
            else{
                cv::Mat resizedImage;
                cv::resize(image, resizedImage, cv::Size(), 0.80, 0.80, cv::INTER_LINEAR);
                lines.push_back(resizedImage);
            }

            displayImageToImageLabel(image);
        }
        contor++;
    }

    showUserInfo("All images have been read.");
}

void MainWindow::buildVideo()
{
    displayImageToImageLabel(scenery);
    showUserInfo("Building video started.");

    int leftRoiX = 90, leftRoiY = 0;
    int roiWidth = lines[0].cols, roiHeight = lines[0].rows;
    int rightRoiX = scenery.cols - leftRoiX - roiWidth, rightRoiY = 0;
    Mat roiScenery, sceneryTemp;
    result.push_back(scenery.clone());
    for(int i=0; i<lines.size(); i++){
        sceneryTemp = scenery.clone();
        displayImageToImageLabel(sceneryTemp);
        //waitKey(1000);

        if(i%2==0){
            roiScenery = sceneryTemp(cv::Rect(leftRoiX, leftRoiY, roiWidth, roiHeight));
            addWeighted(roiScenery, 0.7, lines[i], 0.9,0., roiScenery);
            roiScenery.copyTo(sceneryTemp(cv::Rect(leftRoiX, leftRoiY, roiWidth, roiHeight)));
        }else{
            roiScenery = sceneryTemp(cv::Rect(rightRoiX, rightRoiY, roiWidth, roiHeight));
            addWeighted(roiScenery, 0.7, lines[i], 0.9,0., roiScenery);
            roiScenery.copyTo(sceneryTemp(cv::Rect(rightRoiX, rightRoiY, roiWidth, roiHeight)));
        }
        result.push_back(sceneryTemp.clone());
    }
    showUserInfo("Building video ended.");
}



void MainWindow::displayImageToImageLabel(Mat tempPhoto){
    QImage img= QImage((uchar*) tempPhoto.data, tempPhoto.cols, tempPhoto.rows, tempPhoto.step, QImage::Format_RGB888);
    ui->photoLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->photoLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::showUserInfo(QString message){
    ui->informationLabel->setText(message);
}
