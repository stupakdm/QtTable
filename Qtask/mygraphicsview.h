#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <algorithm>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QObject>
#include <QPen>
#include <QBrush>
#include <QSettings>
#include <QMouseEvent>
#include <QEvent>
#include <QLabel>
#include <QDebug>
#include <QThread>
#include <QTimeLine>
#include "board.h"


class MyGraphicView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicView(QWidget *parent = 0);
    ~MyGraphicView();

    QList<QGraphicsItem*> lines;
    QList<QGraphicsItem*> mouse_lines;
    QList<QGraphicsItem*> path_lines;
    void clearImage(bool not_wheel_delete = true);
    void clearMouseLines();
    void clearGraphLines();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *ev);
    //void wheelEvent(QWheelEvent *event);
    void drawMouseLines();
    void saveSettings();
    void loadSettings();
    void countTable(int x=3, int y=4, bool not_wheel_delete = true, bool not_loading = true);
    //void wheelEvent(QWheelEvent *event);
    void slotDrawBoard(int x=3, int y=4, bool not_wheel_delete = true, bool not_loading = true);

    //int mouse_x, mouse_y;
signals:
    void left_clicked();
    void board_draw();


    void find_path(const int n, int from, int to);

    void count_table(int x=3, int y= 4, bool not_wheel_delete = true, bool not_loading = true);

public slots:
    void wheelEvent(QWheelEvent* event);
    void drawBoard();

private slots:

    void drawPath();

private:
    qreal _numScheduledScalings = 0;
    QPoint wheelEventMousePos;

    QSettings *settings;
    QGraphicsScene      *scene;
    QGraphicsItemGroup  *group_1;


    Board *table;

    QThread *board_thread;

    QThread *table_thread;


private:
    int x_offset, y_offset;
    int mouseLines[2];
    float scaleFactor;
    bool board_created;
    int flag_pressed;
    int load_flag;

private:
    void resizeEvent(QResizeEvent *event);

    void deleteItemsFromGroup(QGraphicsItemGroup *group_1);
    bool check_bad_vertex(int number);
};

#endif
