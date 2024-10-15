#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open image"), ".",
                                                    tr("Image FIles ((*.png *.jpg *.jpeg *.bmp)"));
    photo = imread(fileName.toLatin1().data());
    if(photo.data)
    {
        ui->openButton->setEnabled(true);
        cvtColor(photo, tempPhoto, COLOR_BGR2RGB);
        displayImageToLabel();
    }
    else{
        ui->openButton->setEnabled(false);
        ui->horizontalFlipButton->setEnabled(false);
        ui->verticalFlipButton->setEnabled(false);
        ui->fullFlipButton->setEnabled(false);
    }
}


void MainWindow::on_horizontalFlipButton_clicked()
{
        flip(tempPhoto, tempPhoto, 1);
        displayImageToLabel();
}


void MainWindow::on_verticalFlipButton_clicked()
{
    flip(tempPhoto, tempPhoto, 0);
    displayImageToLabel();
}


void MainWindow::on_fullFlipButton_clicked()
{
    flip(tempPhoto, tempPhoto, -1);
    displayImageToLabel();
}

void MainWindow::displayImageToLabel(){
    QImage img= QImage((uchar*) tempPhoto.data, tempPhoto.cols, tempPhoto.rows, tempPhoto.step, QImage::Format_RGB888);
    ui->photoLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->photoLabel->size(), Qt::KeepAspectRatio));
}


void MainWindow::on_saveImgButton_clicked()
{
     imwrite("E:\\output_image.png", tempPhoto);
}

