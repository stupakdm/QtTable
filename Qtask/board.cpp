#include "board.h"
using namespace std;

Board::Board(QObject *parent) : QObject(parent)
{

}



int Board::create_graph(int num_size, int begin_vertex, int end_vertex) {
    graph = new int* [num_size];
    visited = new int[num_size];
    dfs_path = new int[num_size];
    for (int i = 0; i < num_size; i++) {
        graph[i] = new int[num_size];
    }
    for (int i = 0; i < num_size; i++)
        for (int j = 0; j < num_size; j++)
            graph[i][j] = 0;
    for (int i=0;i<(int)board.size(); i++)
    {
        int a,up,down, left, right;
        a = board[i]->number;
        if (board[i]->up != -1)
        {
            up = board[i]->up;
            graph[a-1][up-1] = graph[up-1][a-1] = 1;
        }
        if (board[i]->down != -1)
        {
            down = board[i]->down;
            graph[a-1][down-1] = graph[down-1][a-1] = 1;
        }
        if (board[i]->left != -1)
        {
            left = board[i]->left;
            graph[a-1][left-1] = graph[left-1][a-1] = 1;
        }
        if (board[i]->right != -1)
        {
            right = board[i]->right;
            graph[a-1][right-1] = graph[right-1][a-1] = 1;
        }

    }
    /*for (int i = 0; i < num_size; i++)
    {
        for (int j = 0; j < num_size; j++)
            cout << graph[i][j];
        cout << endl;
    }*/

    for (int i = 0; i < num_size; i++)
        visited[i] = 0;
    path_size = dfs(num_size, begin_vertex, end_vertex);

    for (int i = 0; i < num_size; i++)
        delete[] graph[i];
    delete[] visited;
    //if (path_size < 0) {
    //    cout << "path does not exist" << endl;
    //}

    return 0;


}
int Board::dfs(const int n, int from, const int to) {
    if (visited[from] == true)
        return -1;
    visited[from] = true;
    if (from == to) {
        path[0] = to;
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        if (graph[from][i] == 0)
            continue;
        path_size = dfs(n, i, to);
        if (path_size < 0)
            continue;
        path[path_size + 1] = from;
        return path_size + 1;
    }
    return -1;
}

void Board::bfs(const int n, int from, int to_where)
{


    vector <vector<int> > g; // граф

    for (int i=0;i<(int)board.size();i++)
    {
        vector<int> edges;
        if (board[i]->up != -1)
            edges.push_back(board[i]->up-1);
        if (board[i]->down != -1)
            edges.push_back(board[i]->down-1);
        if (board[i]->left != -1)
            edges.push_back(board[i]->left-1);
        if (board[i]->right != -1)
            edges.push_back(board[i]->right-1);

        g.push_back(edges);
    }

    queue<int> q;
    q.push (from);
    vector<bool> used (n);
    vector<int> d (n), p (n);
    used[from] = true;
    p[from] = -1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (size_t i=0; i<g[v].size(); ++i) {
            int to = g[v][i];
            if (!used[to]) {
                used[to] = true;
                q.push (to);
                d[to] = d[v] + 1;
                p[to] = v;
            }
        }
    }

    if (!used[to_where])
    {
        //cout << "No path!";
        path_size = -1;
    }
    else {
        for (int v=to_where; v!=-1; v=p[v])
            path.push_back (v+1);
        reverse (path.begin(), path.end());
        //cout << "Path: ";
        //for (size_t i=0; i<path.size(); ++i)
        //    cout << path[i] << " ";
        path_size = path.size();
    }

    emit path_finded();
}

int GetRandomNumber(int min, int max)
{
  srand(time(NULL));

  int num = min + rand() % (max - min + 1);

  return num;
}


void Board::make_path()
{
    for (int i =0;i<path_size;i++)
    {
        struct vertex *vert = new struct vertex;
        vert->number = path[i];
        vert->up = vert->down = vert->left = vert->right = -1;
        //cout << "number: " << board[path[i]-1]->number << endl;
        if (i!=path_size-1)
        {
            if (board[path[i]-1]->up == path[i+1])
                vert->up = path[i+1];
            else if (board[path[i]-1]->down == path[i+1])
                vert->down = path[i+1];
            else if (board[path[i]-1]->left == path[i+1])
                vert->left = path[i+1];
            else if (board[path[i]-1]->right == path[i+1])
                vert->right = path[i+1];
        }
        vert->beg_x = board[path[i]-1]->beg_x;
        vert->beg_y = board[path[i]-1]->beg_y;
        vert->end_x = board[path[i]-1]->end_x;
        vert->end_y = board[path[i]-1]->end_y;
        draw_path.push_back(vert);
    }
}

void Board::deletion()
{
    path.clear();
    path_size = -1;
}

int Board::gen_bad_vertex(int ammount_bad_vertex, int x, int y)
{
    for(int i=0;i<ammount_bad_vertex; i++)
    {
        while (true)
        {
            int z = GetRandomNumber(1, x*y);
            //cout << "random: " << z << endl;
            if (find(bad_vertex.begin(), bad_vertex.end(), z) == bad_vertex.end() )
            {
                bad_vertex.push_back(z);
                break;
            }

        }

    }
    return 0;
}

int Board::create_board(bool not_wheel_delete, bool not_loading)
{
    int full_board = board_x*board_y;
    int err=0;
    if ((not_wheel_delete) && (not_loading))
    {
        //cout << "Do not gen" << endl;
        err = gen_bad_vertex(ammount_bad_vertex, board_x, board_y);
    }
    if (err == -1)
        return -1;
    for (int i=1;i<=full_board;i++)
    {
        struct vertex *new_vertex = new struct vertex;
        if (find(bad_vertex.begin(), bad_vertex.end(), i) != bad_vertex.end() )
        {
            new_vertex->number = i;
            new_vertex->up = -1;
            new_vertex->down = -1;
            new_vertex->left = -1;
            new_vertex->right = -1;
            board.push_back(new_vertex);
            continue;
        }
        new_vertex->number = i;
        //left
        if (i == 1)
            new_vertex->left = -1;
        else if ((i - 1) % board_y == 0)
            new_vertex->left = -1;
        else
        {
            if (find(bad_vertex.begin(), bad_vertex.end(), i-1) != bad_vertex.end() )
                new_vertex->left = -1;
            else
                new_vertex->left = i-1;
        }

        //right
        if (i == full_board)
            new_vertex->right = -1;
        else if (i%board_y==0)
            new_vertex->right = -1;
        else
        {
            if (find(bad_vertex.begin(), bad_vertex.end(), i+1) != bad_vertex.end() )
                new_vertex->right = -1;
            else
                new_vertex->right = i+1;
        }

        //up
        if (i+board_y>full_board)
            new_vertex->up = -1;
        else
        {
            if (find(bad_vertex.begin(), bad_vertex.end(), i+board_y) != bad_vertex.end() )
                new_vertex->up = -1;
            else
                new_vertex->up = i+board_y;
        }

        //down
        if (i-board_y<1)
            new_vertex->down = -1;
        else
        {
            if (find(bad_vertex.begin(), bad_vertex.end(), i-board_y) != bad_vertex.end() )
                new_vertex->down = -1;
            else
                new_vertex->down = i-board_y;
        }
        board.push_back(new_vertex);
    }
    return 0;
}
