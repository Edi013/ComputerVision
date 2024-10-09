/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *openButton;
    QLabel *photoLabel;
    QPushButton *verticalFlipButton;
    QPushButton *horizontalFlipButton;
    QPushButton *fullFlipButton;
    QLabel *label;
    QPushButton *saveImgButton;
    QMenuBar *menubar;
    QMenu *menuImgApp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        openButton = new QPushButton(centralwidget);
        openButton->setObjectName("openButton");
        openButton->setGeometry(QRect(30, 50, 83, 29));
        photoLabel = new QLabel(centralwidget);
        photoLabel->setObjectName("photoLabel");
        photoLabel->setGeometry(QRect(142, 10, 651, 551));
        verticalFlipButton = new QPushButton(centralwidget);
        verticalFlipButton->setObjectName("verticalFlipButton");
        verticalFlipButton->setGeometry(QRect(30, 270, 83, 29));
        horizontalFlipButton = new QPushButton(centralwidget);
        horizontalFlipButton->setObjectName("horizontalFlipButton");
        horizontalFlipButton->setGeometry(QRect(30, 220, 83, 29));
        fullFlipButton = new QPushButton(centralwidget);
        fullFlipButton->setObjectName("fullFlipButton");
        fullFlipButton->setGeometry(QRect(30, 320, 83, 29));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 180, 131, 21));
        saveImgButton = new QPushButton(centralwidget);
        saveImgButton->setObjectName("saveImgButton");
        saveImgButton->setGeometry(QRect(30, 370, 83, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuImgApp = new QMenu(menubar);
        menuImgApp->setObjectName("menuImgApp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuImgApp->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        openButton->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        photoLabel->setText(QString());
        verticalFlipButton->setText(QCoreApplication::translate("MainWindow", "Vertical", nullptr));
        horizontalFlipButton->setText(QCoreApplication::translate("MainWindow", "Horizontal", nullptr));
        fullFlipButton->setText(QCoreApplication::translate("MainWindow", "Both", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Mirroring", nullptr));
        saveImgButton->setText(QCoreApplication::translate("MainWindow", "Save Img", nullptr));
        menuImgApp->setTitle(QCoreApplication::translate("MainWindow", "ImgApp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
