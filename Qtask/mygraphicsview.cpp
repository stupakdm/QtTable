#include "mygraphicsview.h"
#include <algorithm>



MyGraphicView::MyGraphicView(QWidget *parent)
    : QGraphicsView(parent)
{

    qDebug() << QThread::currentThreadId();


    table = new Board(nullptr);
    board_thread = new QThread(this);
    table->moveToThread(board_thread);
    connect(this, &MyGraphicView::find_path, table, &Board::bfs);
    connect(table, &Board::path_finded, this, &MyGraphicView::drawPath);
    board_thread->start();

    table_thread = new QThread(this);
    table->moveToThread(table_thread);
    connect(this, &MyGraphicView::count_table, this, &MyGraphicView::countTable);
    connect(this, &MyGraphicView::board_draw, this, &MyGraphicView::drawBoard);
    table_thread->start();



    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   //Отключим скроллбар по вертикали
    //this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

    settings = new QSettings("MyTest_2", "MygraphicsView", this);
    load_flag = 0;
    this->setMinimumHeight(100);
    this->setMinimumWidth(100);
    setMouseTracking(true);


    scene = new QGraphicsScene();   // Инициализируем сцену для отрисовки
    this->setScene(scene);          // Устанавливаем сцену в виджет

    group_1 = new QGraphicsItemGroup(); // Инициализируем первую группу элементов

    scene->addItem(group_1);            // Добавляем первую группу в сцену
    x_offset = 50;
    y_offset = 50;
    loadSettings();
    if (load_flag == 0)
    {

        scaleFactor = 0.75f;
        board_created = false;
        mouseLines[0] = -1;
        mouseLines[1] = -1;
        flag_pressed = 0;

        table->path_size = -1;

    }



}

MyGraphicView::~MyGraphicView()
{
    board_thread->quit();
    table_thread->quit();
    board_thread->wait();
    table_thread->wait();
    saveSettings();
}

void MyGraphicView::clearMouseLines()
{
    for (int i =0;i<mouse_lines.size();i++)
    {
        delete mouse_lines[i];
    }
    mouse_lines.clear();
}



void MyGraphicView::clearGraphLines()
{
    for (int i=0;i<path_lines.size();i++)
    {
        delete path_lines[i];
    }
    path_lines.clear();
    for (int i =0;i<(int)table->draw_path.size();i++)
         delete table->draw_path[i];
    table->draw_path.clear();

}

void MyGraphicView::clearImage(bool not_wheel_delete)
{
    if (not_wheel_delete)
    {
        table->deletion();
        table->bad_vertex.clear();
        mouseLines[0] = -1;
        mouseLines[1] = -1;
        flag_pressed = 0;
    }
    for (int i =0;i<(int)table->board.size();i++)
         delete table->board[i];
    table->board.clear();

    for (int i =0;i<lines.size();i++)
    {
        delete lines[i];

    }
    lines.clear();


    clearMouseLines();
    clearGraphLines();

}

bool MyGraphicView::check_bad_vertex(int number)
{
    for (int z : table->bad_vertex)
    {
        if (z == number)
            return true;
    }
    return false;

}

void MyGraphicView::drawMouseLines()
{
    int beg_x, beg_y, end_x, end_y;
    int mid_x, mid_y;
    int delta_x, delta_y;
    QPen penGreen(Qt::green);
    if (mouseLines[0] != -1)
    {
        QPen penBlue(Qt::blue);
        struct vertex vert = *table->board[mouseLines[0]-1];
        beg_x = vert.beg_x;
        beg_y = vert.beg_y;
        end_x = vert.end_x;
        end_y = vert.end_y;

        mid_x = beg_x+(end_x-beg_x)/2;
        mid_y = beg_y+(end_y-beg_y)/2;

        delta_x = (end_x-beg_x)/4;
        delta_y = (end_y-beg_y)/4;
        mouse_lines << scene->addEllipse(mid_x-delta_x/2, mid_y-delta_y/2, delta_x, delta_y, penGreen);

    }
    if (mouseLines[1] != -1)
    {
        QPen penGreen(Qt::green);
        struct vertex vert = *table->board[mouseLines[1]-1];
        beg_x = vert.beg_x;
        beg_y = vert.beg_y;
        end_x = vert.end_x;
        end_y = vert.end_y;

        mid_x = beg_x+(end_x-beg_x)/2;
        mid_y = beg_y+(end_y-beg_y)/2;

        delta_x = (end_x-beg_x)/4;
        delta_y = (end_y-beg_y)/4;
        mouse_lines << scene->addEllipse(mid_x-delta_x/2, mid_y-delta_y/2, delta_x, delta_y, penGreen);

    }
}

void MyGraphicView::drawPath()
{
    table->make_path();
    QPen penBlue(Qt::blue);
    int mid_x, mid_y;
    int beg_x, beg_y, end_x, end_y;
    int next_x, next_y;
    int prev_x, prev_y;
    for (int i=0;i<(int)table->draw_path.size();i++)
    {
        struct vertex vert = *table->draw_path[i];
        beg_x = vert.beg_x;
        beg_y = vert.beg_y;
        end_x = vert.end_x;
        end_y = vert.end_y;
        mid_x = beg_x+(end_x-beg_x)/2;
        mid_y = beg_y+(end_y-beg_y)/2;
        if (vert.up != -1)
        {
            next_x = mid_x;
            next_y = end_y;
        }
        else if (vert.down != -1)
        {
            next_x = mid_x;
            next_y = beg_y;
        }
        else if (vert.left != -1)
        {
            next_x = beg_x;
            next_y = mid_y;
        }
        else if (vert.right != -1)
        {
            next_x = end_x;
            next_y = mid_y;
        }

        if (i == (int)table->draw_path.size()-1)
        {
           next_x = mid_x;
           next_y = mid_y;
        }
        if (i == 0)
        {
            prev_x = mid_x;
            prev_y = mid_y;
        }
        path_lines << scene->addLine(prev_x, prev_y, mid_x, mid_y, penBlue);
        path_lines << scene->addLine(mid_x, mid_y, next_x, next_y, penBlue);
        prev_x = next_x;
        prev_y = next_y;
    }
}

void MyGraphicView::saveSettings()
{
    settings->beginWriteArray("path");
    for (size_t i=0;i<table->path.size(); i++)
    {
        //cout<< ' ' << table->path[i] << endl;
        settings->setArrayIndex(i);
        settings->setValue("vertex", table->path[i]);
    }
    settings->endArray();

    settings->beginWriteArray("mouseLines");
    for (size_t i=0;i<2; i++)
    {
        //cout<< ' ' << mouseLines[i] << endl;
        settings->setArrayIndex(i);
        settings->setValue("line", mouseLines[i]);
    }
    settings->endArray();

    settings->beginWriteArray("badVertex");
    for (size_t i=0;i<table->bad_vertex.size(); i++)
    {
        //cout<< ' ' << table->bad_vertex[i] << endl;
        settings->setArrayIndex(i);
        settings->setValue("vertex", table->bad_vertex[i]);
    }
    settings->endArray();
    //cout << "scale: " << scaleFactor << endl;
    settings->setValue("scaleFactor", scaleFactor);
    //cout << "board_created: " << board_created << endl;
    settings->setValue("board_created", board_created);
    //cout << "flag_pressed: " << flag_pressed << endl;
    settings->setValue("flag_pressed", flag_pressed);
    settings->setValue("width", this->width());
    settings->setValue("height", this->height());
    settings->setValue("board_x", table->board_x);
    settings->setValue("board_y", table->board_y);
    load_flag = 1;
    settings->setValue("load_flag", load_flag);
    settings->sync();

}


void MyGraphicView::loadSettings()
{
    //settings->clear();
    load_flag = settings->value("load_flag").toInt();
    if (load_flag != 0)
    {
        board_created = settings->value("board_created").toBool();
        int size_x = settings->value("board_x").toInt();
        int size_y = settings->value("board_y").toInt();
        table->board_x = size_x;
        table->board_y = size_y;

        scaleFactor = (float)settings->value("scaleFactor").toFloat();
        int width = settings->value("width").toInt();
        int height = settings->value("height").toInt();

        //cout << "scaleFactor: " << scaleFactor << endl;
        //cout << width << ' ' << "height: " << height << endl;

        if (board_created)
        {
            int size = settings->beginReadArray("path");
            for (int i=0;i<size; i++)
            {
                settings->setArrayIndex(i);
                table->path.push_back(settings->value("vertex").toInt());
                //cout<< ' ' << table->path[i] << endl;
            }
            settings->endArray();

            size = settings->beginReadArray("mouseLines");
            for (int i=0;i<size; i++)
            {
                settings->setArrayIndex(i);
                mouseLines[i] = settings->value("line").toInt();
                //cout<< ' ' << mouseLines[i] << endl;
            }
            settings->endArray();

            size = settings->beginReadArray("badVertex");
            for (int i=0;i<size; i++)
            {
                settings->setArrayIndex(i);
                table->bad_vertex.push_back(settings->value("vertex").toInt());
                //cout<< ' ' << table->bad_vertex[i] << endl;

            }
            settings->endArray();

            table->ammount_bad_vertex = table->bad_vertex.size();
            flag_pressed = settings->value("flag_pressed").toInt();

            table->create_board(true, false);
            table->path_size = table->path.size();
            slotDrawBoard(table->board_x, table->board_y, true, false);
            drawMouseLines();
            drawPath();
        }
    }

    //settings->clear();

}


void MyGraphicView::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    int beg_x, beg_y, end_x, end_y;
    QPen penGreen(Qt::green);
    for (int i=0;i<(int)table->board.size();i++)
    {
        struct vertex vert = *table->board[i];
        if (check_bad_vertex(vert.number))
            continue;
        beg_x = vert.beg_x;
        beg_y = vert.beg_y;
        end_x = vert.end_x;
        end_y = vert.end_y;

        if ((x>=beg_x) && (x<=end_x) && (y>=beg_y) && (y<=end_y))
        {

            if (vert.number == mouseLines[0] || vert.number == mouseLines[1])
                break;
            if (flag_pressed == 1)
            {
                int mid_x = beg_x+(end_x-beg_x)/2;
                int mid_y = beg_y+(end_y-beg_y)/2;
                int delta_x = (end_x-beg_x)/4;
                int delta_y = (end_y-beg_y)/4;
                //delete mouse_lines[1];
                clearMouseLines();
                mouseLines[1] = vert.number;

                clearGraphLines();
                table->deletion();
                emit find_path(table->board.size(), mouseLines[0]-1, mouseLines[1]-1);
                drawMouseLines();



                break;
            }
        }
    }

}

void MyGraphicView::mousePressEvent(QMouseEvent *event)
{
   int x = event->pos().x();
   int y = event->pos().y();

   int beg_x, beg_y, end_x, end_y;
   QPen penGreen(Qt::green);
   for (int i=0;i<(int)table->board.size();i++)
   {
       struct vertex vert = *table->board[i];
       if (check_bad_vertex(vert.number))
           continue;
       beg_x = vert.beg_x;
       beg_y = vert.beg_y;
       end_x = vert.end_x;
       end_y = vert.end_y;

       if ((x>=beg_x) && (x<=end_x) && (y>=beg_y) && (y<=end_y))
       {
           if (flag_pressed==1)
           {
               clearMouseLines();
               clearGraphLines();
               table->deletion();
               mouseLines[1] = -1;
               flag_pressed = 0;
           }
           if (flag_pressed == 0)
           {
               int mid_x = beg_x+(end_x-beg_x)/2;
               int mid_y = beg_y+(end_y-beg_y)/2;
               int delta_x = (end_x-beg_x)/4;
               int delta_y = (end_y-beg_y)/4;
               mouse_lines << scene->addEllipse(mid_x-delta_x/2, mid_y-delta_y/2, delta_x, delta_y, penGreen);
               mouseLines[0] = vert.number;
               flag_pressed++;
               break;
           }
       }
   }
}


void MyGraphicView::drawBoard()
{
    QPen penBlack(Qt::black);
    QPen penRed(Qt::red);

    int beg_x, beg_y, end_x, end_y;
    for (int i=1;i<=table->board_x;i++)
    {
        for (int j=1;j<=table->board_y;j++)
        {
            beg_x = table->board[(i-1)*table->board_y+j-1]->beg_x;
            end_x = table->board[(i-1)*table->board_y+j-1]->end_x;
            beg_y = table->board[(i-1)*table->board_y+j-1]->beg_y;
            end_y = table->board[(i-1)*table->board_y+j-1]->end_y;

            int num_vertex = (i-1)*table->board_y+j;
            bool flag = check_bad_vertex(num_vertex);
            if (!flag)
            {
                lines << scene->addLine(beg_x,beg_y, end_x, beg_y, penBlack);
                lines << scene->addLine(beg_x, beg_y, beg_x, end_y, penBlack);
                lines << scene->addLine(beg_x, end_y, end_x, end_y, penBlack);
                lines << scene->addLine(end_x, beg_y, end_x, end_y, penBlack);

            }
            else
            {

                lines << scene->addLine(beg_x,beg_y, end_x, beg_y, penBlack);
                lines << scene->addLine(beg_x, beg_y, beg_x, end_y, penBlack);
                lines << scene->addLine(beg_x, end_y, end_x, end_y, penBlack);
                lines << scene->addLine(end_x, beg_y, end_x, end_y, penBlack);
                lines << scene->addLine(beg_x,beg_y, end_x, end_y, penRed);
                lines << scene->addLine(beg_x,end_y, end_x, beg_y, penRed);

            }
        }
    }
}

void MyGraphicView::wheelEvent(QWheelEvent *event)
{
    if (!board_created)
        return;
    setTransformationAnchor(this->AnchorUnderMouse);
    if ((event->delta() > 0) && (scaleFactor < 1.5))
    {
        scaleFactor+=0.05;
    }
    else if ((event-> delta() <=0) && (scaleFactor > 0.1))
    {
        scaleFactor -= 0.05f;
    }
    clearImage(false);
    slotDrawBoard(table->board_x, table->board_y, false, true);
    drawMouseLines();


}

void MyGraphicView::countTable(int size_x, int size_y,bool not_wheel_delete, bool not_loading)
{
    int width = this->width();      // определяем ширину нашего виджета
    int height = this->height();    // определяем высоту нашего виджета

    if (not_wheel_delete)
    {
        table->board_x = size_x;
        table->board_y = size_y;
    }


    if (not_loading)
        table->create_board(not_wheel_delete);

    int width_max = width*scaleFactor;//(width*scale_x)/scale_y;
    int height_max = height*scaleFactor;//(height*scale_x)/scale_y;
    //cout << "width_max" << width_max << " height_max" << height_max << endl;
    int delta_y = width_max/(table->board_x+1);
    int delta_x = height_max/(table->board_y+1);
    //cout << "delta_x" << delta_x << " delta_y" << delta_y << endl;

    int begin_x = x_offset-delta_x/2;
    int begin_y = y_offset-delta_y/2;
    int beg_x, beg_y, end_x, end_y;
    for (int i=1;i<=table->board_x;i++)
    {
        for (int j=1;j<=table->board_y;j++)
        {
            beg_x= begin_x+j*delta_x;
            end_x = begin_x+(j+1)*delta_x;
            beg_y = begin_y+i*delta_y;
            end_y = begin_y+(i+1)*delta_y;
            table->board[(i-1)*table->board_y+j-1]->beg_x = beg_x;
            table->board[(i-1)*table->board_y+j-1]->end_x = end_x;
            table->board[(i-1)*table->board_y+j-1]->beg_y = beg_y;
            table->board[(i-1)*table->board_y+j-1]->end_y = end_y;
        }
    }

}


void MyGraphicView::slotDrawBoard(int size_x, int size_y, bool not_wheel_delete, bool not_loading)
{

    board_created = true;
    this->deleteItemsFromGroup(group_1);
    table->ammount_bad_vertex = (size_x + size_y)/2;
    int width = this->width();      // определяем ширину нашего виджета
    int height = this->height();    // определяем высоту нашего виджета
    scene->setSceneRect(0,0,width,height);
    emit count_table(size_x, size_y, not_wheel_delete, not_loading);

    drawBoard();

}


void MyGraphicView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
}



void MyGraphicView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{

    foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
    }
}
