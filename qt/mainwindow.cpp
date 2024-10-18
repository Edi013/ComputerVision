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
}


void MainWindow::buildVideo()
{
    for(const auto& entry : lines)
    {
        cout<< "1"<<endl;
    }

    //displayImageToImageLabel(scenery);
    Mat leftPersonRoiScenery = scenery(Range(90, 90+lines[0].cols), Range(90, 90+lines[0].rows));
    Mat rightPersonRoiScenery ;//scenery(Range(90, 90+lines[0].cols), Range(90, 90+lines[0].rows));;
    Mat sceneryTemp = scenery;

    addWeighted(scenery, 0.7, leftPersonRoiScenery, 0.9,0.0, sceneryTemp);
    displayImageToImageLabel(sceneryTemp);
    // take each line image and place it on top of scenery image
    // hold it with blend effect for like 5 seconds
    // go to next one
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
