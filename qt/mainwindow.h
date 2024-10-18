#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include <QRadioButton>
#include <filesystem>
#include <vector>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectInOutFoldersButton_clicked();
    void on_executeButton_clicked();
    void on_playVideoButton_clicked();
    void on_saveVideoButton_clicked();

private:
    Ui::MainWindow *ui;
    QString sourceFolder;
    QString destinationFolder;

    Mat scenery;
    std::vector<cv::Mat> lines;
    std::vector<cv::Mat> result;

    void readImages();
    void buildVideo();
    void showUserInfo(QString message);
    void displayImageToImageLabel(Mat tempPhoto);
    void writeFrameToVideo(VideoWriter& videoWriter, Mat& sceneryTemp, Mat& tempRoiScenery, Mat& roiScenery, int i, double j, int roiX, int roiY, int roiWidth, int roiHeight);
};
#endif // MAINWINDOW_H
