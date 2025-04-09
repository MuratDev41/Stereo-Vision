#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadLeftButton_clicked();
    void on_loadRightButton_clicked();
    void on_computeButton_clicked();
    void on_methodComboBox_currentIndexChanged(int index);
    void on_windowSizeEdit_textChanged(const QString &text);
    void on_maxDisparityEdit_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    QImage leftImage;
    QImage rightImage;
    QImage disparityImage;
    QGraphicsScene *leftScene;
    QGraphicsScene *rightScene;
    QGraphicsScene *disparityScene;

    int windowSize;
    int maxDisparity;

    typedef double (MainWindow::*CostFunction)(const QImage&, const QImage&, int, int, int);
    CostFunction currentCostFunction;

    double computeSAD(const QImage& left, const QImage& right, int x, int y, int d);
    double computeSSD(const QImage& left, const QImage& right, int x, int y, int d);
    double computeZNCC(const QImage& left, const QImage& right, int x, int y, int d);

    void updateDisparityMap();
    bool validateWindowSize(int size);
    bool validateMaxDisparity(int disparity);
};

#endif // MAINWINDOW_H
