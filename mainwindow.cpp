#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , leftScene(new QGraphicsScene(this))
    , rightScene(new QGraphicsScene(this))
    , disparityScene(new QGraphicsScene(this))
    , windowSize(3)
    , maxDisparity(64)
    , currentCostFunction(&MainWindow::computeSAD)
{
    ui->setupUi(this);

    ui->leftGraphicsView->setScene(leftScene);
    ui->rightGraphicsView->setScene(rightScene);
    ui->disparityGraphicsView->setScene(disparityScene);

    ui->methodComboBox->addItems({"SAD", "SSD", "ZNCC"});
    ui->windowSizeEdit->setText(QString::number(windowSize));
    ui->maxDisparityEdit->setText(QString::number(maxDisparity));

    connect(ui->methodComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_methodComboBox_currentIndexChanged);
    connect(ui->windowSizeEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_windowSizeEdit_textChanged);
    connect(ui->maxDisparityEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_maxDisparityEdit_textChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_loadLeftButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Left Image");
    if (leftImage.load(fileName)) {
        leftScene->clear();
        leftScene->addPixmap(QPixmap::fromImage(leftImage));
        ui->leftGraphicsView->fitInView(leftScene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::on_loadRightButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Right Image");
    if (rightImage.load(fileName)) {
        rightScene->clear();
        rightScene->addPixmap(QPixmap::fromImage(rightImage));
        ui->rightGraphicsView->fitInView(rightScene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::on_computeButton_clicked() {
    if (leftImage.isNull() || rightImage.isNull()) {
        QMessageBox::warning(this, "Error", "Load both images.");
        return;
    }
    if (leftImage.size() != rightImage.size()) {
        QMessageBox::warning(this, "Error", "Images must be the same size.");
        return;
    }
    if (!validateWindowSize(windowSize) || !validateMaxDisparity(maxDisparity)) {
        return;
    }
    updateDisparityMap();
}

void MainWindow::on_methodComboBox_currentIndexChanged(int index) {
    switch (index) {
    case 0: currentCostFunction = &MainWindow::computeSAD; break;
    case 1: currentCostFunction = &MainWindow::computeSSD; break;
    case 2: currentCostFunction = &MainWindow::computeZNCC; break;
    }
}

void MainWindow::on_windowSizeEdit_textChanged(const QString &text) {
    int size = text.toInt();
    if (validateWindowSize(size)) windowSize = size;
}

void MainWindow::on_maxDisparityEdit_textChanged(const QString &text) {
    int disparity = text.toInt();
    if (validateMaxDisparity(disparity)) maxDisparity = disparity;
}

double MainWindow::computeSAD(const QImage& left, const QImage& right, int x, int y, int d) {
    int radius = windowSize / 2;
    double sum = 0;

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int lx = x + dx, ly = y + dy;
            int rx = lx - d, ry = ly;

            if (lx < 0 || lx >= left.width() || ly < 0 || ly >= left.height() ||
                rx < 0 || rx >= right.width() || ry < 0 || ry >= right.height()) {
                sum += 255 * 3;
                continue;
            }

            QRgb l = left.pixel(lx, ly);
            QRgb r = right.pixel(rx, ry);

            sum += abs(qRed(l) - qRed(r)) +
                   abs(qGreen(l) - qGreen(r)) +
                   abs(qBlue(l) - qBlue(r));
        }
    }
    return sum;
}

double MainWindow::computeSSD(const QImage& left, const QImage& right, int x, int y, int d) {
    int radius = windowSize / 2;
    double sum = 0;

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int lx = x + dx, ly = y + dy;
            int rx = lx - d, ry = ly;

            if (lx < 0 || lx >= left.width() || ly < 0 || ly >= left.height() ||
                rx < 0 || rx >= right.width() || ry < 0 || ry >= right.height()) {
                sum += (255 * 255) * 3;
                continue;
            }

            QRgb l = left.pixel(lx, ly);
            QRgb r = right.pixel(rx, ry);

            int dr = qRed(l) - qRed(r);
            int dg = qGreen(l) - qGreen(r);
            int db = qBlue(l) - qBlue(r);

            sum += dr * dr + dg * dg + db * db;
        }
    }
    return sum;
}

double MainWindow::computeZNCC(const QImage& left, const QImage& right, int x, int y, int d) {
    int radius = windowSize / 2;
    double sumL = 0, sumR = 0;
    double sumL2 = 0, sumR2 = 0, sumLR = 0;
    int count = 0;

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int lx = x + dx, ly = y + dy;
            int rx = lx - d, ry = ly;

            if (lx < 0 || lx >= left.width() || ly < 0 || ly >= left.height() ||
                rx < 0 || rx >= right.width() || ry < 0 || ry >= right.height()) {
                continue;
            }

            int lGray = qGray(left.pixel(lx, ly));
            int rGray = qGray(right.pixel(rx, ry));

            sumL += lGray;
            sumR += rGray;
            sumL2 += lGray * lGray;
            sumR2 += rGray * rGray;
            sumLR += lGray * rGray;
            count++;
        }
    }

    if (count == 0) return 0;

    double meanL = sumL / count;
    double meanR = sumR / count;
    double numerator = sumLR - (sumL * sumR / count);
    double denomL = sumL2 - (sumL * sumL / count);
    double denomR = sumR2 - (sumR * sumR / count);
    double denominator = sqrt(denomL * denomR);

    return (denominator == 0) ? 0 : numerator / denominator;
}


void MainWindow::updateDisparityMap() {
    disparityImage = QImage(leftImage.size(), QImage::Format_Grayscale8);
    int radius = windowSize / 2;
    bool isZNCC = (currentCostFunction == &MainWindow::computeZNCC);

    for (int y = radius; y < leftImage.height() - radius; ++y) {
        for (int x = radius; x < leftImage.width() - radius; ++x) {
            double bestVal = isZNCC ? -INFINITY : INFINITY;
            int bestD = 0;

            for (int d = 0; d <= maxDisparity; ++d) {
                if (x - d < radius) continue;
                double cost = (this->*currentCostFunction)(leftImage, rightImage, x, y, d);
                if ((isZNCC && cost > bestVal) || (!isZNCC && cost < bestVal)) {
                    bestVal = cost;
                    bestD = d;
                }
            }
            disparityImage.setPixel(x, y, qRgb(bestD * 255 / maxDisparity, bestD * 255 / maxDisparity, bestD * 255 / maxDisparity));
        }
    }

    disparityScene->clear();
    disparityScene->addPixmap(QPixmap::fromImage(disparityImage));
    disparityImage.save("disparity.png");
}

bool MainWindow::validateWindowSize(int size) {
    return (size >= 3 && size <= 21 && size % 2 == 1);
}

bool MainWindow::validateMaxDisparity(int disparity) {
    return (disparity > 0);
}
