#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>
#include <functional>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadLeftButton_clicked();
    void on_loadRightButton_clicked();
    void on_loadGTButton_clicked(); // Yeni slot
    void on_computeButton_clicked();
    void on_methodComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QImage leftImage, rightImage, disparityImage, gtImage;
    QGraphicsScene *leftScene, *rightScene, *disparityScene;
    int windowSize = 5;
    int maxDisparity = 30;

    using CostFunction = std::unique_ptr<std::function<double(const QImage&, const QImage&, int, int, int)>>;
    CostFunction currentCostFunction;

    double computeSAD(const QImage &left, const QImage &right, int x, int y, int d);
    double computeSSD(const QImage &left, const QImage &right, int x, int y, int d);
    double computeZNCC(const QImage &left, const QImage &right, int x, int y, int d);

    double evaluateWithSAD(const QImage &gt, const QImage &pred);
    double evaluateWithSSD(const QImage &gt, const QImage &pred);

    void updateDisparityMap();
};

#endif // MAINWINDOW_H
