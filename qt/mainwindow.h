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
    void loadImage();                // Slot to load an image from file
    void updateText();               // Slot to update text display on image
    void saveImage();                // Slot to save the modified image

    void setTextPosition(int x, int y);   // Function to set text position
    void updateTextAttributes();          // Update font size, thickness, and color

protected:
    void mousePressEvent(QMouseEvent *event) override; // Capture mouse position for text placement

private:
    Ui::MainWindow *ui;

    QLabel *imageLabel;              // Label to display the image
    QPixmap originalPixmap;          // Original image
    QPixmap modifiedPixmap;          // Image with text overlay

    QLineEdit *textEdit;             // Input for user text
    QSlider *sizeSlider;             // Slider for text size
    QSlider *thicknessSlider;        // Slider for text thickness
    QPushButton *colorButton;        // Button to open color dialog
    QPushButton *loadButton;         // Button to load an image
    QPushButton *saveButton;         // Button to save the image

    QColor textColor;                // Color for text
    QString userText;                // Text to be drawn
    int textSize;                    // Font size
    int textThickness;               // Text thickness
    int textPosX, textPosY;          // Position for text on the image
};

#endif // MAINWINDOW_H
