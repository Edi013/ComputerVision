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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *photoLabel;
    QPushButton *selectInAndOutFoldersButton;
    QPushButton *executeButton;
    QRadioButton *horizontalRadioButton;
    QRadioButton *verticalRadioButton;
    QLabel *label;
    QRadioButton *fullFlipRadioButton;
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
        photoLabel = new QLabel(centralwidget);
        photoLabel->setObjectName("photoLabel");
        photoLabel->setGeometry(QRect(142, 10, 651, 551));
        selectInAndOutFoldersButton = new QPushButton(centralwidget);
        selectInAndOutFoldersButton->setObjectName("selectInAndOutFoldersButton");
        selectInAndOutFoldersButton->setGeometry(QRect(20, 170, 80, 24));
        executeButton = new QPushButton(centralwidget);
        executeButton->setObjectName("executeButton");
        executeButton->setGeometry(QRect(20, 330, 80, 24));
        horizontalRadioButton = new QRadioButton(centralwidget);
        horizontalRadioButton->setObjectName("horizontalRadioButton");
        horizontalRadioButton->setGeometry(QRect(20, 240, 91, 22));
        verticalRadioButton = new QRadioButton(centralwidget);
        verticalRadioButton->setObjectName("verticalRadioButton");
        verticalRadioButton->setGeometry(QRect(20, 270, 91, 22));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 210, 49, 16));
        fullFlipRadioButton = new QRadioButton(centralwidget);
        fullFlipRadioButton->setObjectName("fullFlipRadioButton");
        fullFlipRadioButton->setGeometry(QRect(20, 300, 91, 22));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
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
        photoLabel->setText(QString());
        selectInAndOutFoldersButton->setText(QCoreApplication::translate("MainWindow", "InOut Folders", nullptr));
        executeButton->setText(QCoreApplication::translate("MainWindow", "Execute", nullptr));
        horizontalRadioButton->setText(QCoreApplication::translate("MainWindow", "Horizontal", nullptr));
        verticalRadioButton->setText(QCoreApplication::translate("MainWindow", "Vertical", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Flip types", nullptr));
        fullFlipRadioButton->setText(QCoreApplication::translate("MainWindow", "Both", nullptr));
        menuImgApp->setTitle(QCoreApplication::translate("MainWindow", "ImgApp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
