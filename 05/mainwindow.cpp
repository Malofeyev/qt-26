#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QElapsedTimer>
#include <QTime>

constexpr int WINDOW_UPDATE_T = 100;

QString msecToString(qint64 msec) {
    QTime time = QTime::fromMSecsSinceStartOfDay(msec);
    const QString format = "HH:mm:ss:zzz";
    return time.toString(format);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , stopwatch(/*this, */WINDOW_UPDATE_T)
{
    ui->setupUi(this);

    ui->pb_status->setText("Старт");
    ui->pb_round->setText("Круг");
    ui->pb_round->setEnabled(false);
    ui->pb_reset->setText("Очистить");
    ui->l_time->setStyleSheet(
        "QLabel {"
        "   font: 23pt 'Arial';"
        "   qproperty-alignment: 'AlignCenter';"
        "}"
    );
    ui->l_time->setText(msecToString(stopwatch.elapsed()));

    connect(&stopwatch, &Stopwatch::timeout, this, [&]() {
        ui->l_time->setText(msecToString(stopwatch.elapsed()));
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_status_clicked()
{
    if (stopwatch.isActive()) {
        ui->pb_status->setText("Старт");
        ui->pb_round->setEnabled(false);
        stopwatch.stop();
    } else {
        ui->pb_status->setText("Стоп");
        ui->pb_round->setEnabled(true);
        stopwatch.start();
    }
}

void MainWindow::on_pb_round_clicked()
{
    stopwatch.setRound();
    auto round = stopwatch.getRound();
    ui->tb_round->append(QString("Круг %1, время: %2 сек")
                         .arg(round.num)
                         .arg(round.interval / 1000.0));
}

void MainWindow::on_pb_reset_clicked()
{
    stopwatch.reset();
    ui->l_time->setText(msecToString(stopwatch.elapsed()));
    ui->tb_round->clear();
}
