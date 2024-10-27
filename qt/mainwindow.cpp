#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textSize = 32;
    ui->sizeSlider->setRange(0, 200);
    ui->sizeSlider->setValue(textSize);
    displayTextSizeToLabel(textSize);

    textThickness = 20;
    ui->thicknessSlider->setRange(0, 25);
    ui->thicknessSlider->setValue(textThickness);
    displayTextThicknessToLabel(textThickness);


    textPosX=imageLabel.size().width()/2;
    ui->textXSlider->setRange(10, imageLabel.size().width()*2);
    ui->textXSlider->setValue(textPosX);
    displayTextToXLabel(textPosX);

    textPosY=imageLabel.size().height()/2;
    ui->textYSlider->setRange(10, imageLabel.size().height()*2);
    ui->textYSlider->setValue(textPosY);
    displayTextToYLabel(textPosY);

    textColor = Qt::black;
    changeUserInputValue("Sample Text");
    toggleButtonsAvailability(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeUserInputValue(std::string value){
    userText = QString::fromStdString(value);
    ui->textEdit->setText(userText);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint imagePos = event->pos() - ui->imageLabel->pos();
    if (ui->imageLabel->geometry().contains(imagePos)) {
        setTextPosition(imagePos.x(), imagePos.y());
        updateText();
    }
}


void MainWindow::setTextPosition(int x, int y) {
    textPosX = x;
    textPosY = y;
}

void MainWindow::updateText()
{
    userText = ui->textEdit->text();
    updateTextAttributes();
}

// void MainWindow::updateTextAttributes()
// {
//     textSize = ui->sizeSlider->value();
//     textThickness = ui->thicknessSlider->value();

//     modifiedPixmap = originalPixmap;
//     QPainter painter(&modifiedPixmap);
//     QPen pen(textColor, textThickness);
//     QFont font("Arial", textSize);

//     painter.setPen(pen);
//     painter.setFont(font);
//     painter.drawText(textPosX, textPosY, userText);


//     displayImageToImageLabel(modifiedPixmap);
// }
void MainWindow::updateTextAttributes()
{
    textSize = ui->sizeSlider->value();
    textThickness = ui->thicknessSlider->value();

    modifiedPixmap = originalPixmap;

    // Convert QPixmap to OpenCV Mat
    cv::Mat matImage = QPixmapToCvMat(modifiedPixmap);

    // Set color and font parameters
    cv::Scalar cvTextColor(textColor.red(), textColor.green(), textColor.blue());
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;

    // Use putText to add text to the image
    cv::putText(matImage, userText.toStdString(), cv::Point(textPosX, textPosY),
                fontFace, textSize / 20.0, cvTextColor, textThickness);

    // Convert modified Mat back to QPixmap
    modifiedPixmap = CvMatToQPixmap(matImage);

    displayImageToImageLabel(modifiedPixmap);
}


void MainWindow::saveImage()
{
    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!savePath.isEmpty()) {
        modifiedPixmap.save(savePath);
    }
}

void MainWindow::loadImage()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open image"), ".",
                                                    tr("Image FIles ((*.png *.jpg *.jpeg *.bmp)"));
    if (!filePath.isEmpty()) {
        originalPixmap.load(filePath);
        modifiedPixmap = originalPixmap;
        displayImageToImageLabel(modifiedPixmap);
    }

    toggleButtonsAvailability(true);
}

void MainWindow::displayImageToImageLabel(QPixmap image) {
    ui->imageLabel->setPixmap(image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::toggleButtonsAvailability(bool value) {
    ui->saveButton->setDisabled(!value);
    ui->colorButton->setDisabled(!value);
    ui->textEdit->setDisabled(!value);
}

void MainWindow::displayTextSizeToLabel(int value){
    ui->textSizeLabel->setText(QString("Text Size: %1").arg(value));
}

void MainWindow::displayTextThicknessToLabel(int value){
    ui->textThicknessLabel->setText(QString("Text Size: %1").arg(value));
}

void MainWindow::on_saveButton_clicked()
{
    saveImage();
}

void MainWindow::on_loadButton_clicked()
{
    loadImage();
}

void MainWindow::on_colorButton_clicked()
{
    QColor selectedColor = QColorDialog::getColor(textColor, this, tr("Select Text Color"));

    if (selectedColor.isValid()) {
        textColor = selectedColor;
        updateText();
    }
}

void MainWindow::on_sizeSlider_valueChanged(int value)
{
    textSize = value;
    updateText();
    displayTextSizeToLabel(value);
}

void MainWindow::on_thicknessSlider_valueChanged(int value)
{
    textThickness = value;
    updateText();
    displayTextThicknessToLabel(value);
}

void MainWindow::on_textEdit_textChanged(const QString &value)
{
    userText = value;
    updateText();
}

void MainWindow::on_textXSlider_valueChanged(int value)
{
    textPosX = value;
    updateText();
    displayTextToXLabel(value);
}

void MainWindow::on_textYSlider_valueChanged(int value)
{
    textPosY = value;
    updateText();
    displayTextToYLabel(value);
}

void MainWindow::displayTextToXLabel(int value){
    ui->textXLabel->setText("Text X: " + QString::number(value));

}
void MainWindow::displayTextToYLabel(int value){
    ui->textYLabel->setText("Text Y: " + QString::number(value));
}

cv::Mat MainWindow::QPixmapToCvMat(const QPixmap &pixmap) {
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    return cv::Mat(img.height(), img.width(), CV_8UC3, const_cast<uchar*>(img.bits()), img.bytesPerLine()).clone();
}

QPixmap MainWindow::CvMatToQPixmap(const cv::Mat &mat) {
    QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return QPixmap::fromImage(img.rgbSwapped());
}
