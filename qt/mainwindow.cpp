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

// Select destination folder
void MainWindow::on_selectInAndOutFoldersButton_clicked()
{
    sourceFolder = QFileDialog::getExistingDirectory(this, tr("Select Source Folder"), "");
    if (sourceFolder.isEmpty())
    {
        ui->photoLabel->setText("Source: " + sourceFolder + " was NOT selected.");
    }else
        ui->photoLabel->setText("Source: " + sourceFolder + " was selected successfully.");

    destinationFolder = QFileDialog::getExistingDirectory(this, tr("Select Destination Folder"), "");
    if (destinationFolder.isEmpty())
    {
        ui->photoLabel->setText("Destination: " + destinationFolder + " was NOT selected.");
    }else
        ui->photoLabel->setText("Destination: " + destinationFolder + " was selected successfully.");


    if(sourceFolder.isEmpty() || destinationFolder.isEmpty()){
        ui->photoLabel->setText("Operation will be repeted. Select the source and destination again. For EXIT select any folders, than press x in the main window.");
        on_selectInAndOutFoldersButton_clicked();
        return;
    }

    ui->executeButton->setDisabled(false);
}

// Execute the flipping process
void MainWindow::on_executeButton_clicked()
{
    if (sourceFolder.isEmpty() || destinationFolder.isEmpty())
    {
        ui->photoLabel->setText("Please select both source and destination folders.");
        return;
    }

    int flipCode = 2;

    if (ui->fullFlipRadioButton->isChecked())
        flipCode = -1; // Both flips
    else if (ui->horizontalRadioButton->isChecked())
        flipCode = 1; // Horizontal flip
    else if (ui->verticalRadioButton->isChecked())
        flipCode = 0; // Vertical flip

    if(flipCode == 2)
    {
        ui->photoLabel->setText("Please select a flip type before trying to initiate the process.");
        return;
    }

    processImages(flipCode);
    ui->photoLabel->setText("Processing complete.");
}

void MainWindow::processImages(int flipCode)
{
    for (const auto& entry : fs::directory_iterator(sourceFolder.toStdString()))
    {
        if (entry.is_regular_file())
        {
            string filePath = entry.path().string();
            Mat image = imread(filePath);
            if (!image.empty())
            {
                Mat flippedImage;
                flip(image, flippedImage, flipCode);

                string fileName = entry.path().filename().string();
                string outputFilePath = destinationFolder.toStdString() + "/" + fileName;
                imwrite(outputFilePath, flippedImage);
            }
        }
    }
}
