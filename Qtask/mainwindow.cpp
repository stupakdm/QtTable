#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //first_time = true;
    ui->setupUi(this);

    settings = new QSettings("MyTest_2", "MainWindow", this);
    loadSettings();

}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    int size_x = ui->Box_X->value();
    int size_y = ui->Box_Y->value();
    if ((size_x <=0) || (size_y <=0))
    {
        QMessageBox::about(this, "Ошибка", "Размеры поля должны быть больше 0");
    }
    else
    {
        ui->mygraphicsView->clearImage();

        ui->mygraphicsView->slotDrawBoard(size_x, size_y);
    }


}

void MainWindow::saveSettings()
{
    //cout << "save" << endl;
    settings->setValue("Box_X", ui->Box_X->value());
    settings->setValue("Box_Y", ui->Box_Y->value());
    //settings->setValue("first_time", first_time);
    ui->mygraphicsView->saveSettings();
}


void MainWindow::loadSettings()
{
    //if (!first_time)
    //{
    uint size_x = settings->value("Box_X").toInt();
    uint size_y = settings->value("Box_Y").toInt();
    //cout << size_x << ' ' << size_y << endl;
    ui->Box_X->setValue(size_x);
    ui->Box_Y->setValue(size_y);

}


