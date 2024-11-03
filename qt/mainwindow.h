#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void saveImage();
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_colorButton_clicked();
    void on_imageButton_clicked();


private:
    Ui::MainWindow *ui;
    cv::Mat originalMat;
    cv::Mat modifiedMat;

    void replaceBackgroundWithColor(const cv::Scalar &color);
    void replaceBackgroundWithImage(const cv::Mat &bgImage);
    void displayImageToImageLabel(const cv::Mat &matImage);
    void toggleButtonsAvailability(bool value);
};

#endif // MAINWINDOW_H
