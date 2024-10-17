#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QFileDialog>
#include <QRadioButton>
#include <filesystem>

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
    void on_selectInAndOutFoldersButton_clicked();
    void on_executeButton_clicked();

private:
    Ui::MainWindow *ui;
    QString sourceFolder;
    QString destinationFolder;

    Mat photo;
    Mat photoAdapted;

    void processImages();
};
#endif // MAINWINDOW_H
