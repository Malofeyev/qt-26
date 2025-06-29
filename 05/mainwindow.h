#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stopwatch.h>

#include <QMainWindow>

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
    void on_pb_status_clicked();

    void on_pb_round_clicked();

    void on_pb_reset_clicked();

private:
    Ui::MainWindow *ui;
    Stopwatch stopwatch;
};
#endif // MAINWINDOW_H
