#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QtMath>
#include <chrono>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , leftScene(new QGraphicsScene(this))
    , rightScene(new QGraphicsScene(this))
    , disparityScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->leftGraphicsView->setScene(leftScene);
    ui->rightGraphicsView->setScene(rightScene);
    ui->disparityGraphicsView->setScene(disparityScene);
    currentCostFunction = std::make_unique<std::function<double(const QImage&, const QImage&, int, int, int)>>(
        [this](const QImage &l, const QImage &r, int x, int y, int d) {
            return computeSAD(l, r, x, y, d);
        });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_loadLeftButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Left Image");
    if (!fileName.isEmpty()) {
        leftImage.load(fileName);
        leftScene->clear();
        leftScene->addPixmap(QPixmap::fromImage(leftImage));
    }
}

void MainWindow::on_loadRightButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Right Image");
    if (!fileName.isEmpty()) {
        rightImage.load(fileName);
        rightScene->clear();
        rightScene->addPixmap(QPixmap::fromImage(rightImage));
    }
}

void MainWindow::on_loadGTButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Ground Truth Image");
    if (!fileName.isEmpty()) {
        gtImage.load(fileName);
    }
}

void MainWindow::on_computeButton_clicked() {
    updateDisparityMap();
}

void MainWindow::on_methodComboBox_currentIndexChanged(int index) {
    switch (index) {
    case 0:
        currentCostFunction.reset(new std::function<double(const QImage&, const QImage&, int, int, int)>(
            [this](const QImage &l, const QImage &r, int x, int y, int d) {
                return computeSAD(l, r, x, y, d);
            }));
        break;
    case 1:
        currentCostFunction.reset(new std::function<double(const QImage&, const QImage&, int, int, int)>(
            [this](const QImage &l, const QImage &r, int x, int y, int d) {
                return computeSSD(l, r, x, y, d);
            }));
        break;
    case 2:
        currentCostFunction.reset(new std::function<double(const QImage&, const QImage&, int, int, int)>(
            [this](const QImage &l, const QImage &r, int x, int y, int d) {
                return computeZNCC(l, r, x, y, d);
            }));
        break;
    }
}

double MainWindow::computeSAD(const QImage &left, const QImage &right, int x, int y, int d) {
    double sum = 0.0;
    for (int dy = -windowSize; dy <= windowSize; ++dy) {
        for (int dx = -windowSize; dx <= windowSize; ++dx) {
            int xl = x + dx;
            int yl = y + dy;
            int xr = xl - d;
            if (xr >= 0 && xr < right.width() && yl >= 0 && yl < left.height()) {
                int leftVal = qGray(left.pixel(xl, yl));
                int rightVal = qGray(right.pixel(xr, yl));
                sum += qAbs(leftVal - rightVal);
            }
        }
    }
    return sum;
}

double MainWindow::computeSSD(const QImage &left, const QImage &right, int x, int y, int d) {
    double sum = 0.0;
    for (int dy = -windowSize; dy <= windowSize; ++dy) {
        for (int dx = -windowSize; dx <= windowSize; ++dx) {
            int xl = x + dx;
            int yl = y + dy;
            int xr = xl - d;
            if (xr >= 0 && xr < right.width() && yl >= 0 && yl < left.height()) {
                int leftVal = qGray(left.pixel(xl, yl));
                int rightVal = qGray(right.pixel(xr, yl));
                double diff = leftVal - rightVal;
                sum += diff * diff;
            }
        }
    }
    return sum;
}

double MainWindow::computeZNCC(const QImage &left, const QImage &right, int x, int y, int d) {
    double meanL = 0, meanR = 0, numerator = 0, denomL = 0, denomR = 0;
    int count = 0;
    for (int dy = -windowSize; dy <= windowSize; ++dy) {
        for (int dx = -windowSize; dx <= windowSize; ++dx) {
            int xl = x + dx;
            int yl = y + dy;
            int xr = xl - d;
            if (xr >= 0 && xr < right.width() && yl >= 0 && yl < left.height()) {
                int l = qGray(left.pixel(xl, yl));
                int r = qGray(right.pixel(xr, yl));
                meanL += l;
                meanR += r;
                count++;
            }
        }
    }
    if (count == 0) return 0;
    meanL /= count;
    meanR /= count;

    for (int dy = -windowSize; dy <= windowSize; ++dy) {
        for (int dx = -windowSize; dx <= windowSize; ++dx) {
            int xl = x + dx;
            int yl = y + dy;
            int xr = xl - d;
            if (xr >= 0 && xr < right.width() && yl >= 0 && yl < left.height()) {
                double l = qGray(left.pixel(xl, yl)) - meanL;
                double r = qGray(right.pixel(xr, yl)) - meanR;
                numerator += l * r;
                denomL += l * l;
                denomR += r * r;
            }
        }
    }
    if (denomL == 0 || denomR == 0) return 0;
    return -numerator / (sqrt(denomL) * sqrt(denomR));
}

void MainWindow::updateDisparityMap() {
    if (leftImage.isNull() || rightImage.isNull()) return;

    auto start = std::chrono::high_resolution_clock::now();

    disparityImage = QImage(leftImage.size(), QImage::Format_Grayscale8);

    for (int y = windowSize; y < leftImage.height() - windowSize; ++y) {
        for (int x = windowSize + maxDisparity; x < leftImage.width() - windowSize; ++x) {
            double minCost = 1e10;
            int bestDisparity = 0;
            for (int d = 0; d <= maxDisparity; ++d) {
                double cost = (*currentCostFunction)(leftImage, rightImage, x, y, d);
                if (cost < minCost) {
                    minCost = cost;
                    bestDisparity = d;
                }
            }
            int gray = static_cast<int>(255.0 * bestDisparity / maxDisparity);
            disparityImage.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }
    disparityScene->clear();
    disparityScene->addPixmap(QPixmap::fromImage(disparityImage));

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    ui->timeLabel->setText(QString("Computation Time: %1 ms").arg(duration.count()));

    // Evaluate accuracy if GT is loaded
    if (!gtImage.isNull()) {
        double sadError = evaluateWithSAD(gtImage, disparityImage);
        double ssdError = evaluateWithSSD(gtImage, disparityImage);
        ui->sadLabel->setText(QString("SAD Error: %1").arg(sadError));
        ui->ssdLabel->setText(QString("SSD Error: %1").arg(ssdError));
    }
}

double MainWindow::evaluateWithSAD(const QImage &gt, const QImage &pred) {
    double sum = 0.0;
    for (int y = 0; y < gt.height(); ++y) {
        for (int x = 0; x < gt.width(); ++x) {
            int gtVal = qGray(gt.pixel(x, y));
            int predVal = qGray(pred.pixel(x, y));
            sum += qAbs(gtVal - predVal);
        }
    }
    return sum;
}

double MainWindow::evaluateWithSSD(const QImage &gt, const QImage &pred) {
    double sum = 0.0;
    for (int y = 0; y < gt.height(); ++y) {
        for (int x = 0; x < gt.width(); ++x) {
            int gtVal = qGray(gt.pixel(x, y));
            int predVal = qGray(pred.pixel(x, y));
            double diff = gtVal - predVal;
            sum += diff * diff;
        }
    }
    return sum;
}
