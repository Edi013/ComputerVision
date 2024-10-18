#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->executeButton->setDisabled(true);
    on_selectInOutFoldersButton_clicked();
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
                lines.push_back(image);
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

    cv::Mat resizedImage;
    cv::resize(lines[0], resizedImage, cv::Size(), 0.77, 0.77, cv::INTER_LINEAR);  // Scaling by 0.77
    lines[0] = resizedImage;

    int roiX = 90;
    int roiY = 0;
    int leftRoiWidth = lines[0].cols;
    int leftRoiHeight = lines[0].rows;



    Mat leftPersonRoiScenery = scenery(cv::Rect(roiX, roiY, leftRoiWidth, leftRoiHeight));

    //Mat rightPersonRoiScenery ;//scenery(Range(90, 90+lines[0].cols), Range(90, 90+lines[0].rows));;

    addWeighted(leftPersonRoiScenery, 0.7, lines[0], 0.9,0., leftPersonRoiScenery);

    Mat sceneryTemp = scenery.clone();
    leftPersonRoiScenery.copyTo(sceneryTemp(cv::Rect(roiX, roiY, leftRoiWidth, leftRoiHeight)));

    displayImageToImageLabel(sceneryTemp);
    // take each line image and place it on top of scenery image
    // hold it with blend effect for like 5 seconds
    // go to next one

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
