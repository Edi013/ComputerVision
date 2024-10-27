#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QSlider>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>

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
    void updateText();
    void saveImage();

    void setTextPosition(int x, int y);
    void updateTextAttributes();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_colorButton_clicked();

    void on_sizeSlider_valueChanged(int value);

    void on_thicknessSlider_valueChanged(int value);

    void on_textEdit_textChanged(const QString &arg1);

    void on_textXSlider_valueChanged(int value);

    void on_textYSlider_valueChanged(int value);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;

    QLabel imageLabel;
    QLabel textSizeLabel;
    QLabel textThicknessLabel;
    cv::Mat originalMat;
    cv::Mat modifiedMat;

    QLineEdit textEdit;
    QSlider sizeSlider;
    QSlider thicknessSlider;
    QPushButton colorButton;
    QPushButton loadButton;
    QPushButton saveButton;

    QColor textColor;
    QString userText;
    int textSize;
    int textThickness;
    int textPosX, textPosY;
    void displayImageToImageLabel(const cv::Mat& matImage);
    void toggleButtonsAvailability(bool value);
    void displayTextSizeToLabel(int value);
    void displayTextThicknessToLabel(int value);
    void changeUserInputValue(std::string value);

    void displayTextToXLabel(int value);
    void displayTextToYLabel(int value);
    cv::Mat QPixmapToCvMat(const QPixmap &pixmap) ;
    QPixmap CvMatToQPixmap(const cv::Mat &mat) ;
    void updateSlidersWithMatSize(const cv::Mat &mat);
};

#endif // MAINWINDOW_H
