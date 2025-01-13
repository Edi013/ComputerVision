#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadVideoButton_clicked();
    void on_processVideoButton_clicked();
    void on_saveVideoButton_clicked();
private:
    Ui::MainWindow *ui;
    cv::Mat frame;  // Cadru video curent
    std::string videoFilePath;  // Calea fișierului video original
    std::string outputFilePath; // Calea fișierului video procesat

    void processFrame(cv::Mat &frame);  // Funcție pentru procesarea fiecărui cadru cu YOLOv11
    void drawSegmentation(cv::Mat &frame, const std::vector<cv::Mat> &masks);  // Desenează contururi pe cadre
};

#endif // MAINWINDOW_H
