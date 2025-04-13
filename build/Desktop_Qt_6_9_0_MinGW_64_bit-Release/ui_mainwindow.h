/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *leftGraphicsView;
    QGraphicsView *rightGraphicsView;
    QComboBox *methodComboBox;
    QLineEdit *windowSizeEdit;
    QPushButton *loadGTButton;
    QPushButton *computeButton;
    QGraphicsView *disparityGraphicsView;
    QLineEdit *maxDisparityEdit;
    QPushButton *loadLeftButton;
    QPushButton *loadRightButton;
    QLabel *timeLabel;
    QLabel *sadLabel;
    QLabel *ssdLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1019, 555);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        leftGraphicsView = new QGraphicsView(centralwidget);
        leftGraphicsView->setObjectName("leftGraphicsView");
        leftGraphicsView->setGeometry(QRect(0, 21, 281, 211));
        rightGraphicsView = new QGraphicsView(centralwidget);
        rightGraphicsView->setObjectName("rightGraphicsView");
        rightGraphicsView->setGeometry(QRect(280, 20, 281, 211));
        methodComboBox = new QComboBox(centralwidget);
        methodComboBox->setObjectName("methodComboBox");
        methodComboBox->setGeometry(QRect(835, 40, 131, 26));
        windowSizeEdit = new QLineEdit(centralwidget);
        windowSizeEdit->setObjectName("windowSizeEdit");
        windowSizeEdit->setGeometry(QRect(840, 70, 131, 26));
        loadGTButton = new QPushButton(centralwidget);
        loadGTButton->setObjectName("loadGTButton");
        loadGTButton->setGeometry(QRect(620, 170, 93, 29));
        computeButton = new QPushButton(centralwidget);
        computeButton->setObjectName("computeButton");
        computeButton->setGeometry(QRect(830, 170, 93, 29));
        disparityGraphicsView = new QGraphicsView(centralwidget);
        disparityGraphicsView->setObjectName("disparityGraphicsView");
        disparityGraphicsView->setGeometry(QRect(140, 300, 281, 211));
        maxDisparityEdit = new QLineEdit(centralwidget);
        maxDisparityEdit->setObjectName("maxDisparityEdit");
        maxDisparityEdit->setGeometry(QRect(840, 100, 131, 26));
        loadLeftButton = new QPushButton(centralwidget);
        loadLeftButton->setObjectName("loadLeftButton");
        loadLeftButton->setGeometry(QRect(90, 240, 93, 29));
        loadRightButton = new QPushButton(centralwidget);
        loadRightButton->setObjectName("loadRightButton");
        loadRightButton->setGeometry(QRect(380, 240, 93, 29));
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setGeometry(QRect(690, 330, 251, 16));
        sadLabel = new QLabel(centralwidget);
        sadLabel->setObjectName("sadLabel");
        sadLabel->setGeometry(QRect(690, 300, 281, 16));
        ssdLabel = new QLabel(centralwidget);
        ssdLabel->setObjectName("ssdLabel");
        ssdLabel->setGeometry(QRect(690, 360, 291, 16));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        loadGTButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        computeButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        loadLeftButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        loadRightButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        sadLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        ssdLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
