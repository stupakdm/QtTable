#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <mygraphicsview.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

public:
    void saveSettings();
    void loadSettings();

private:
    bool first_time;
    QSettings* settings;
    Ui::MainWindow  *ui;
    MyGraphicView   *myPicture;
};

#endif // MAINWINDOW_H
