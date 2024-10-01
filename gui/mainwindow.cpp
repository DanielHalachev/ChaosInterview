#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QString"
#include "QMessageBox"
#include <QtConcurrent/QtConcurrent>
#include "lighting/LightingParser.h"
#include "progressnotifierbase.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Scene", QDir::homePath());
    this->selectedFilePath = std::filesystem::path(fileName.toStdString());
    this->ui->statusbar->showMessage("Selected file " + QString::fromStdString(this->selectedFilePath.filename().string()), 10000);
    this->ui->startStopButton->setEnabled(true);
    this->ui->progressBar->setValue(0);
    this->ui->progressBar->setDisabled(true);
}


void MainWindow::on_startStopButton_clicked()
{
    if(selectedFilePath.empty()){
        QMessageBox::warning(this, "No File Selected", "Please select a file before starting.");
    }

    this->ui->statusbar->showMessage("Working...");
    this->ui->startStopButton->setDisabled(true);
    this->ui->progressBar->setEnabled(true);
    this->ui->selectButton->setDisabled(true);

    QCoreApplication::processEvents();
    QtConcurrent::run([this]() {
        auto startTime = std::chrono::high_resolution_clock::now();
        ProgressNotifierBase notifier(this->ui->progressBar);
        LightingParser<double> parser;
        std::vector<std::vector<double>> array = parser.parse(this->selectedFilePath, &notifier);
        QImage image(array[0].size(), array.size(), QImage::Format_Grayscale8);

        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                float value = array[y][x]/10.0;
                uint8_t grayscale = static_cast<uint8_t>(std::clamp(value * 255.0f, 0.0f, 255.0f));
                image.setPixel(x, y, qRgb(grayscale, grayscale, grayscale));
            }
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        QPixmap pixmap = QPixmap::fromImage(image);
        QMetaObject::invokeMethod(this, [this, pixmap, duration]() {
            this->scene.addPixmap(pixmap);
            this->ui->graphicsView->setScene(&scene);

            this->ui->startStopButton->setText("Start");
            this->ui->selectButton->setEnabled(true);
            this->ui->statusbar->showMessage("Image rendered in " + QString::fromStdString(std::to_string(duration.count())) + "s");
        });
    });

    // below is an attempt to make a worker class
    // in order to force the termination of the execution,
    // if the window is closed using the X button
    // but it didn't make a difference

    // this->worker = new LightParserWorker(this->selectedFilePath, this->ui->progressBar);
    // this->workerThread = new QThread();

    // this->worker->moveToThread(workerThread);

    // connect(workerThread, &QThread::started, worker, &LightParserWorker::doWork);
    // // connect(worker, &LightParserWorker::progressMade, this, &MainWindow::updateScene);
    // connect(worker, &LightParserWorker::finished, this, &MainWindow::onComputationFinished);
    // connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);

    // workerThread->start();
}

void MainWindow::onComputationFinished(QPixmap pixmap, double duration) {
    this->scene.addPixmap(pixmap);
    this->ui->graphicsView->setScene(&scene);

    this->ui->startStopButton->setText("Start");
    this->ui->selectButton->setEnabled(true);
    this->ui->statusbar->showMessage("Image rendered in " + QString::number(duration) + "s");

    this->workerThread->quit();
    this->workerThread->wait();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (workerThread && workerThread->isRunning()) {
        worker->stop();
        workerThread->quit();
        workerThread->wait();
    }
    QMainWindow::closeEvent(event);
}


