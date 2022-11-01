#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <queue>
#include <algorithm>
#include <stdlib.h> // íóæåí äëÿ âûçîâà ôóíêöèé rand(), srand()
#include <time.h> // íóæåí äëÿ âûçîâà ôóíêöèè time()
#include <QDebug>
#include <QThread>
using namespace std;

struct vertex{
    int number;
    int up, down, left, right;
    int x, y;
    int beg_x,beg_y;
    int end_x, end_y;
};



class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = nullptr);

signals:
    void path_finded();

private:
    int** graph;
    int* visited;
    int *dfs_path;

public slots:
    void bfs(const int n, int from, int to);

public:
    vector<int> path;
    int path_size;
    vector<int> bad_vertex;
    vector<struct vertex*> draw_path;
    int gen_bad_vertex(int ammount_bad_vertex, int x, int y);
    int board_x, board_y;
    int ammount_bad_vertex;
    vector<struct vertex*> board;
    int create_board(bool not_wheel_delete = true, bool not_loading=true);
    int dfs(const int n, int from, const int to);

    int create_graph(int num_size, int begin_vertex, int end_vertex);
    void make_path();

    void deletion();

};


class Graph {
  int numVertices;
  list<int> *adjLists;
  bool *visited;

   public:
  Graph(int V);
  void addEdge(int src, int dest);
  void DFS(int vertex);
};


#endif // BOARD_H
