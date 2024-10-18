#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->executeButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectInOutFoldersButton_clicked()
{
    sourceFolder = QFileDialog::getExistingDirectory(this, tr("Select Source Folder"), "");
    if (sourceFolder.isEmpty())
    {
        showUserInfo("Source: " + sourceFolder + " was NOT selected.");
    }else
        showUserInfo("Source: " + sourceFolder + " was selected successfully.");

    destinationFolder = QFileDialog::getExistingDirectory(this, tr("Select Destination Folder"), "");
    if (destinationFolder.isEmpty())
    {
        showUserInfo("Destination: " + destinationFolder + " was NOT selected.");
    }else
        showUserInfo("Destination: " + destinationFolder + " was selected successfully.");
    // sourceFolder =QStringLiteral("E:\\Projects\\qt_proj\\qt\\images");
    // destinationFolder = sourceFolder;

    if(sourceFolder.isEmpty() || destinationFolder.isEmpty()){
        showUserInfo("Operation will be repeted. Select the source and destination again. For EXIT select any folders, than press x in the main window.");
        on_selectInOutFoldersButton_clicked();
        return;
    }

    ui->executeButton->setDisabled(false);
}

void MainWindow::on_executeButton_clicked()
{
    if (sourceFolder.isEmpty() || destinationFolder.isEmpty())
    {
        showUserInfo("Please select source and destination folders first.");
        return;
    }

    readImages();
    buildVideo();
    saveVideo();

    showUserInfo("Processing complete.");
}

void MainWindow::readImages()
{
    showUserInfo("Reading images from in folder.");
    waitKey(700);
    int contor = 0;
    for (const auto& entry : fs::directory_iterator(sourceFolder.toStdString()))
    {
        if (entry.is_regular_file())
        {
            string filePath = entry.path().string();
            Mat image = imread(filePath, cv::COLOR_RGB2BGR);

            if(contor==0){
                scenery = image;
            }
            else{
                cv::Mat resizedImage;
                cv::resize(image, resizedImage, cv::Size(), 0.80, 0.80, cv::INTER_LINEAR);
                lines.push_back(resizedImage);
            }

            displayImageToImageLabel(image);
            showUserInfo("Selected images are displayed.");
            waitKey(300);
        }
        contor++;
    }

    showUserInfo("All images have been read.");
    waitKey(700);
}

void MainWindow::buildVideo()
{
    displayImageToImageLabel(scenery);
    showUserInfo("Building video started.");
    waitKey(700);

    int leftRoiX = 90, leftRoiY = 0;
    int roiWidth = lines[0].cols, roiHeight = lines[0].rows;
    int rightRoiX = scenery.cols - leftRoiX - roiWidth, rightRoiY = 0;
    Mat roiScenery, sceneryTemp;
    for(int i=0; i<=lines.size(); i++){
        sceneryTemp = scenery.clone();
        displayImageToImageLabel(sceneryTemp);
        waitKey(1000);

        if(i%2==0){
            roiScenery = sceneryTemp(cv::Rect(leftRoiX, leftRoiY, roiWidth, roiHeight));
            addWeighted(roiScenery, 0.7, lines[i], 0.9,0., roiScenery);
            roiScenery.copyTo(sceneryTemp(cv::Rect(leftRoiX, leftRoiY, roiWidth, roiHeight)));
        }else{
            roiScenery = sceneryTemp(cv::Rect(rightRoiX, rightRoiY, roiWidth, roiHeight));
            addWeighted(roiScenery, 0.7, lines[i], 0.9,0., roiScenery);
            roiScenery.copyTo(sceneryTemp(cv::Rect(rightRoiX, rightRoiY, roiWidth, roiHeight)));
        }
        displayImageToImageLabel(sceneryTemp);
        waitKey(3500);
    }
    // hold it with blend effect for like 5 seconds

    showUserInfo("Building video ended.");
    waitKey(700);
}

void MainWindow::saveVideo()
{
    //string fileName = entry.path().filename().string();
    //string outputFilePath = destinationFolder.toStdString() + "/" + fileName;
    //imwrite(outputFilePath, scenery, cv::COLOR_BGR2RGB);
}

void MainWindow::displayImageToImageLabel(Mat tempPhoto){
    QImage img= QImage((uchar*) tempPhoto.data, tempPhoto.cols, tempPhoto.rows, tempPhoto.step, QImage::Format_RGB888);
    ui->photoLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->photoLabel->size(), Qt::KeepAspectRatio));
}

void MainWindow::showUserInfo(QString message){
    ui->informationLabel->setText(message);
}
