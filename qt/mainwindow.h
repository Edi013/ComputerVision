#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>

using namespace std;
using namespace cv;

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
    void on_openButton_clicked();

    void on_horizontalFlipButton_clicked();

    void on_verticalFlipButton_clicked();

    void on_fullFlipButton_clicked();
    void displayImageToLabel();

    void on_saveImgButton_clicked();

private:
    Ui::MainWindow *ui;
    Mat photo;
    Mat tempPhoto;
};
#endif // MAINWINDOW_H
