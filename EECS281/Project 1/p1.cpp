#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;

struct Tile{
    char t; 
    int r;
    int c;
    int f;
    char dirPrev; 
    //type , row , column, direction of previous tile
    Tile(char t1, int f1, int r1, int c1, char d1)
    {
        t = t1;
        r = r1;
        c = c1;
        f = f1;
        dirPrev = d1;
    }
    Tile()
    {
        t = 0;
        r = 0;
        c = 0;
        dirPrev = 'x';
    }
};
struct Square
{
    char t;
    bool visited; 
    Square(char t1, bool v1)
    {
        t = t1;
        visited = v1;
    }
    Square()
    {
        t = '.';
        visited = false;
    }
};

int main(int argc, char **argv)
{
    stack<Tile> sTiles; 
    stack<Tile> svTiles;
    stack<Tile> pathTiles;
    queue<Tile> qTiles;
    char inpMode;
    int i, n, numFarms;
    bool queue = true;
    bool map = true;
    string curLine;
    ifstream file;
    file.open("map.txt");
    file >> inpMode >> n >> numFarms;
    vector<vector<vector<Square>>> fields(numFarms, vector<vector<Square>>(n, vector<Square>(n,Square())));
    int row = 0, col = 0, farm = 0;
    char type = '.';
    int currRow = 0, currCol = 0, currFarm = 0;
    int endRow = 0, endCol = 0, endFarm = 0;
    // code for reading with the map input mode
    if(inpMode == 'M') 
    {
        while(getline(file, curLine))
        {
            if(curLine.size() > 1 && curLine[0] == '/' && curLine[1] == '/')
            {
            }
            else if(curLine.size() > 0)
            {
                for(int i = 0; i < n; i ++)
                {
                    //check valid character
                    if(curLine[i]=='B'||curLine[i]=='S'||curLine[i]=='<'||curLine[i]=='>'||curLine[i]=='#')
                    {
                        fields[farm][row][col].t = curLine[i];
                        if(curLine[i]=='S')
                        {
                            currRow = row;
                            currCol = col;
                            currFarm = farm;
                        }
                        if(curLine[i]=='B')
                        {
                            endRow = row;
                            endCol = col;
                            endFarm = farm;
                        }
                    }
                    else if(curLine[i] == '.')
                    {
                    }
                    else
                    {
                        cout << "invalid character input"; 
                        exit(1);
                    }
                    col ++;
                    if(col == n)
                    {
                        col = 0;
                        row ++; 
                    }
                    if(row == n)
                    {
                        row = 0;
                        farm ++;
                    }
                }


            }
            if(file.fail())
                break;
        }
    }
    // code for reading with the list input mode 
    else if(inpMode == 'L') 
    {
        while(getline(file, curLine))
        {
            if(curLine.size() > 1 && curLine[0] == '/' && curLine[1] == '/')
            {
            }
            else if(curLine.size() > 0)
            {
                stringstream strstr(curLine);
                strstr >> type >> row >> type >> col >> type >> farm >> type >> type; 
                //check valid position
                if(farm>=0 && farm < numFarms && row >= 0 && row < n && col >=0 && col < n)
                {
                    //check valid character
                    if(type =='B'||type=='S'||type=='<'||type=='>'||type=='#'||type=='.')
                    {
                        fields[farm][row][col].t = type;
                        if(type=='S')
                        {
                            currRow = row;
                            currCol = col;
                            currFarm = farm;
                        }
                        if(type=='B')
                        {
                            endRow = row;
                            endCol = col;
                            endFarm = farm;
                        }

                    }
                    else
                    {
                        cout << "invalid character input";
                        exit(1);
                    }
                }
                else
                {
                    cout << "invalid position!";
                    exit(1);
                }
            }
            if(file.fail())
                break;
        }
    }

    if(queue)
    {
        //queues
        qTiles.push(Tile('S',currFarm, currRow, currCol, 'x'));
        fields[currFarm][currRow][currCol].visited = true; 
        while(!qTiles.empty())
        {
            Tile top = qTiles.front();
            svTiles.push(top);
            qTiles.pop();

            if(top.t == '>' && top.f + 1 < numFarms && fields[top.f+1][top.r][top.c].t != '#' && !fields[top.f+1][top.r][top.c].visited)
            {
                qTiles.push(Tile(fields[top.f+1][top.r][top.c].t, top.f+1, top.r, top.c, 'u'));
                fields[top.f+1][top.r][top.c].visited = true;
                if(qTiles.back().t == 'B')
                {
                    svTiles.push(qTiles.back());
                    break;
                }
            }
            else if(top.t == '<' && top.f - 1 >= 0 && fields[top.f-1][top.r][top.c].t != '#'  && !fields[top.f-1][top.r][top.c].visited)
            {
                qTiles.push(Tile(fields[top.f-1][top.r][top.c].t, top.f-1, top.r, top.c, 'd'));
                fields[top.f-1][top.r][top.c].visited = true;
                if(qTiles.back().t == 'B')
                {
                    svTiles.push(qTiles.back());
                    break;
                }
            }
            else if(top.t != '<' && top.t != '>')
            {
                if(top.r-1 >= 0 && fields[top.f][top.r-1][top.c].t != '#' && !fields[top.f][top.r-1][top.c].visited)
                {
                    qTiles.push(Tile(fields[top.f][top.r-1][top.c].t, top.f, top.r-1, top.c, 'n'));
                    fields[top.f][top.r-1][top.c].visited = true;
                    if(qTiles.back().t == 'B')
                {
                    svTiles.push(qTiles.back());
                    break;
                }
                }
                if(top.c+1 < n && fields[top.f][top.r][top.c+1].t != '#' && !fields[top.f][top.r][top.c+1].visited) 
                {
                    qTiles.push(Tile(fields[top.f][top.r][top.c+1].t, top.f, top.r, top.c+1, 'e'));
                    fields[top.f][top.r][top.c+1].visited = true;
                    if(qTiles.back().t == 'B')
                {
                    svTiles.push(qTiles.back());
                    break;
                }
                }
                if(top.r+1 < n && fields[top.f][top.r+1][top.c].t != '#' && !fields[top.f][top.r+1][top.c].visited)
                {
                    qTiles.push(Tile(fields[top.f][top.r+1][top.c].t, top.f, top.r+1, top.c, 's'));
                    fields[top.f][top.r+1][top.c].visited = true;
                    if(qTiles.back().t == 'B')
                {
                    svTiles.push(qTiles.back());
                    break;
                }
                }
                if(top.c-1 >= 0 && fields[top.f][top.r][top.c-1].t != '#' && !fields[top.f][top.r][top.c-1].visited)
                {
                    qTiles.push(Tile(fields[top.f][top.r][top.c-1].t, top.f, top.r, top.c-1, 'w'));
                    fields[top.f][top.r][top.c-1].visited = true;
                    if(qTiles.back().t == 'B')
                {
                    svTiles.push(qTiles.back());
                    break;
                }
                }
            }

        }
    }
    else
    {
        //stacks
        sTiles.push(Tile('S',currFarm, currRow, currCol, 'x'));
        fields[currFarm][currRow][currCol].visited = true; 
        while(!sTiles.empty())
        {
            Tile top = sTiles.top();
            svTiles.push(top);
            sTiles.pop();

            if(top.t == '>' && top.f + 1 < numFarms && fields[top.f+1][top.r][top.c].t != '#' && !fields[top.f+1][top.r][top.c].visited)
            {
                sTiles.push(Tile(fields[top.f+1][top.r][top.c].t, top.f+1, top.r, top.c, 'u'));
                fields[top.f+1][top.r][top.c].visited = true;
                if(sTiles.top().t == 'B')
                {
                    svTiles.push(sTiles.top());
                    break;
                }
            }
            else if(top.t == '<' && top.f - 1 >= 0 && fields[top.f-1][top.r][top.c].t != '#'  && !fields[top.f-1][top.r][top.c].visited)
            {
                sTiles.push(Tile(fields[top.f-1][top.r][top.c].t, top.f-1, top.r, top.c, 'd'));
                fields[top.f-1][top.r][top.c].visited = true;
                if(sTiles.top().t == 'B')
                {
                    svTiles.push(sTiles.top());
                    break;
                }
            }
            else if(top.t != '<' && top.t != '>')
            {
                if(top.r-1 >= 0 && fields[top.f][top.r-1][top.c].t != '#' && !fields[top.f][top.r-1][top.c].visited)
                {
                    sTiles.push(Tile(fields[top.f][top.r-1][top.c].t, top.f, top.r-1, top.c, 'n'));
                    fields[top.f][top.r-1][top.c].visited = true;
                    if(sTiles.top().t == 'B')
                    {
                        svTiles.push(sTiles.top());
                        break;
                    }
                }
                if(top.c+1 < n && fields[top.f][top.r][top.c+1].t != '#' && !fields[top.f][top.r][top.c+1].visited) 
                {
                    sTiles.push(Tile(fields[top.f][top.r][top.c+1].t, top.f, top.r, top.c+1, 'e'));
                    fields[top.f][top.r][top.c+1].visited = true;
                    if(sTiles.top().t == 'B')
                    {
                        svTiles.push(sTiles.top());
                        break;
                    }
                }
                if(top.r+1 < n && fields[top.f][top.r+1][top.c].t != '#' && !fields[top.f][top.r+1][top.c].visited)
                {
                    sTiles.push(Tile(fields[top.f][top.r+1][top.c].t, top.f, top.r+1, top.c, 's'));
                    fields[top.f][top.r+1][top.c].visited = true;
                    if(sTiles.top().t == 'B')
                    {
                        svTiles.push(sTiles.top());
                        break;
                    }
                }
                if(top.c-1 >= 0 && fields[top.f][top.r][top.c-1].t != '#' && !fields[top.f][top.r][top.c-1].visited)
                {
                    sTiles.push(Tile(fields[top.f][top.r][top.c-1].t, top.f, top.r, top.c-1, 'w'));
                    fields[top.f][top.r][top.c-1].visited = true;
                    if(sTiles.top().t == 'B')
                    {
                        svTiles.push(sTiles.top());
                        break;
                    }
                }
            }

        }
    }
    currFarm = endFarm;
    currRow = endRow;
    currCol = endCol;
    while(!svTiles.empty())
    {
        Tile top = svTiles.top();
        svTiles.pop();
        if(currRow == top.r && currCol == top.c && currFarm == top.f)
        {
            if(top.dirPrev == 'n')
            {
                currRow +=1;
                if(map)
                fields[top.f][top.r+1][top.c].t = top.dirPrev;
                else
                pathTiles.push(Tile(top.dirPrev, top.f, top.r+1, top.c, 'x'));
            }
            else if(top.dirPrev == 'e')
            {
                currCol -=1;
                if(map)
                fields[top.f][top.r][top.c-1].t = top.dirPrev;
                else
                pathTiles.push(Tile(top.dirPrev, top.f, top.r, top.c-1, 'x'));
            }
            else if(top.dirPrev == 's')
            {
                currRow -=1;
                if(map)
                fields[top.f][top.r-1][top.c].t = top.dirPrev;
                else
                pathTiles.push(Tile(top.dirPrev, top.f, top.r-1, top.c, 'x'));
            }
            else if(top.dirPrev == 'w')
            {
                currCol +=1;
                if(map)
                fields[top.f][top.r][top.c+1].t = top.dirPrev;
                else
                pathTiles.push(Tile(top.dirPrev, top.f, top.r, top.c+1, 'x'));
            }
            else if(top.dirPrev == 'u')
            {
                currFarm -=1;
                if(map)
                fields[top.f-1][top.r][top.c].t = top.dirPrev;
                else
                pathTiles.push(Tile(top.dirPrev, top.f-1, top.r, top.c, 'x'));
            }
            else if(top.dirPrev == 'd')
            {
                currFarm +=1;
                if(map)
                fields[top.f+1][top.r][top.c].t = top.dirPrev;
                else
                pathTiles.push(Tile(top.dirPrev, top.f+1, top.r, top.c, 'x'));
            }
        }
    }
    ostringstream oss;
    oss << n << "\n" << numFarms << "\n";
    if(map)
    {
        for (i = 0; i < numFarms; i++)
        {
            oss << "//farm " << i << "\n";
            for(int j = 0 ; j < n; j++)
            {
                for(int k = 0; k < n; k++)
                {
                    oss <<  fields[i][j][k].t << " ";
                }
                oss << "\n";
            }
        }
    }
    else
    {
        oss << "//path taken\n";
        while(!pathTiles.empty())
        {
            Tile top = pathTiles.top();
            oss << "(" << top.r << "," << top.c << "," << top.f << "," << top.t << ")\n";
            pathTiles.pop();
        }
    }
    cout << oss.str() << flush; 

    return 0;
}
