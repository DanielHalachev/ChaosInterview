#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filesystem>
#include <QGraphicsScene>
#include <QThread>
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void onComputationFinished(QPixmap pixmap, double duration);
    void closeEvent(QCloseEvent *event);

    ~MainWindow();

private slots:
    void on_selectButton_clicked();

    void on_startStopButton_clicked();

private:
    Ui::MainWindow *ui;
    std::filesystem::path selectedFilePath;
    LightParserWorker* worker;
    QThread* workerThread;
    QGraphicsScene scene = QGraphicsScene(this);
};
#endif // MAINWINDOW_H
