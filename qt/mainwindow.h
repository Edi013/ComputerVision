#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;

    QLabel imageLabel;
    QLabel textSizeLabel;
    QLabel textThicknessLabel;
    QPixmap originalPixmap;
    QPixmap modifiedPixmap;

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
    void displayImageToImageLabel(QPixmap image);
    void toggleButtonsAvailability(bool value);
    void displayTextSizeToLabel(int value);
    void displayTextThicknessToLabel(int value);
    void changeUserInputValue(std::string value);
};

#endif // MAINWINDOW_H
