#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pb_progress->setText("Кнопка");
    ui->pb_progress->setCheckable(true);
    ui->pb_progress->setChecked(false);

    ui->rb_up->setText("Увеличить");
    ui->rb_down->setText("Уменьшить");
    ui->rb_up->setChecked(true);


    ui->pBar_main->setMaximum(110);
    ui->pBar_main->setMinimum(10);
    ui->pBar_main->setValue(20);

    int delta = ui->pBar_main->maximum() - ui->pBar_main->minimum();

    ui->cb_progress_value->addItems({"1", "10", "50"});
    deltas.append(delta / 100);
    deltas.append(delta / 10);
    deltas.append(delta / 2);
    ui->cb_progress_value->setCurrentIndex(1);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_progress_clicked()
{
    int mul = 0;
    if (ui->rb_up->isChecked()) {
        mul = 1;
    }
    if (ui->rb_down->isChecked()) {
        mul = -1;
    }
    int delta = 0;
    int indx = ui->cb_progress_value->currentIndex();
    if (indx >=0 && indx < deltas.size()) {
        delta = deltas[indx];
    }
    int currentValue = ui->pBar_main->value();
    int newValue = currentValue + mul * delta;
    if (newValue < ui->pBar_main->minimum()) {
        newValue = ui->pBar_main->maximum();
    }
    if (newValue > ui->pBar_main->maximum()) {
        newValue = ui->pBar_main->minimum();
    }
    ui->pBar_main->setValue(newValue);
}
