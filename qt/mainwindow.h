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

    void on_hueLow_valueChanged(int value);

    void on_hueHigh_valueChanged(int value);

    void on_saturationLow_valueChanged(int value);

    void on_saturationHigh_valueChanged(int value);

    void on_valueLow_valueChanged(int value);

    void on_valueHigh_valueChanged(int value);
    void on_xSliderROI_valueChanged(int value);

    void on_ySliderROI_valueChanged(int value);

    void on_roiSizeSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    cv::Mat originalMat;
    cv::Mat modifiedMat;
    int hueLow = 0;
    int hueHigh = 180;
    int saturationLow = 0;
    int saturationHigh = 40;
    int valueLow = 200;
    int valueHigh = 255;
    int roiSize=400;

    std::string noneIdentifier = "none";
    std::string colorMethodIdentifier = "color";
    std::string imageMethodIdentifier = "image";
    std::string lastUsed = noneIdentifier;
    cv::Scalar lastBackgroundColorUsed;
    cv::Mat lastBackgroundImageUsed;
    cv::Mat roi;


    void replaceBackgroundWithColor(const cv::Scalar &color);
    void replaceBackgroundWithImage(const cv::Mat &bgImage);
    void displayImageToImageLabel(const cv::Mat &matImage);
    void toggleButtonsAvailability(bool value);
    int findBackgroundThreshold(const cv::Mat& hist);
    void displayMask(const cv::Mat &mask);
    void updateBackgroundReplacement();
    void lastMethodUsed();
    void drawSquare();
    void onSliderChange();
};

#endif // MAINWINDOW_H
