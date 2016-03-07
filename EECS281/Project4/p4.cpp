#include <queue>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <random>
#include <math.h>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

struct edge
{
    double dist;
    int adj;
    edge(double a, int b)
    {
        dist = a;
        adj = b;
    }
};
struct location
{
    int in, x,y,num;

    bool visited;
    vector <edge> edges;
    location(int a, int b, int c, int d)
    {
        x = a;
        y = b;
        in = c;
        num = d;
        visited = false;
    }
};

struct opair
{   
    int node1, node2;
    opair(int x, int y)
    {
        node1 = x;
        node2 = y;
    }

};

struct path
{
    vector<opair> nodes;
    int distance;
    vector<int> visited;
    path()
    {
        distance = 0;

    }
};
double dist(location a, location b);
int fact(int n);
int main(int argc, char **argv)
{
    int lx, ly, rx, ry, mode = 3;
    int m, tx, ty, temp;

    double totalDist = 0;
    vector <location> verts;
    vector <location> visit;
    //vector <location> nvisit;
    vector <opair> mst;
    cin >> lx >> ly >> rx >> ry;
    cin >> m;
    for(int i = 0; i < m; i++)
    {
        cin >> tx >> ty;
        if(tx > lx && tx < rx && ty > ly && ty < ry)
            temp = 0;
        else if((tx == lx || tx == rx) && (ty > ly && ty < ry)
            || (ty == ry || ty == ly) && (tx > lx && tx < rx))
            temp = 1;
        else 
            temp = 2;
        verts.push_back(location(tx, ty, temp, i));
    }
    if(mode == 1)
    {
        for(unsigned int i = 0; i < verts.size() - 1; i++)
        {
            for(unsigned int j = i + 1; j < verts.size(); j++)
            {
                if(verts[i].in == 0 && (verts[j].in == 0 || verts[j].in == 1)
                    ||verts[i].in == 1 ||
                    (verts[i].in == 2 && (verts[j].in == 1 || verts[j].in == 2)))
                {
                    verts[i].edges.push_back(edge(dist(verts[i], verts[j]), j));
                    verts[j].edges.push_back(edge(dist(verts[i], verts[j]), i));
                }
            }
        }
        verts[0].visited = true;
        //nvisit = verts;
        visit.push_back(verts[0]);
        //nvisit.erase(nvisit.begin());
        int verticesLeft = verts.size() - 1;
        while(verticesLeft >= 1)
        {
            double minDist = 999999; 
            int minNode, minNode2;
            for(unsigned int i = 0; i < visit.size(); i++)
            {
                for(unsigned int j = 0; j < visit[i].edges.size(); j++)
                {
                    if(visit[i].edges[j].dist < minDist && !verts[visit[i].edges[j].adj].visited)
                    {
                        minNode2 = verts[visit[i].edges[j].adj].num;
                        minNode = visit[i].num;
                        minDist = visit[i].edges[j].dist;
                    }
                }
            }
            if(minDist != 999999)
            {
                verts[minNode2].visited = true;
                visit.push_back(verts[minNode2]);
                verticesLeft--;
                if(minNode < minNode2)
                    mst.push_back(opair(minNode, minNode2));
                else
                    mst.push_back(opair(minNode2, minNode));
                totalDist += minDist;
            }
            else
            {
                cout << "Cannot construct MST";
                exit(1);
            }
        }
        ostringstream oss;
        oss << setprecision(2) << fixed;
        oss << "" << totalDist << "\n";
        for(unsigned int i = 0; i < mst.size(); i++)
        {
            oss << mst[i].node1 << " " << mst[i].node2 << "\n";
        }
        cout << oss.str() << flush;
    }
    else if(mode == 2)
    {
        for(unsigned int i = 0; i < verts.size() - 1; i++)
        {
            for(unsigned int j = i + 1; j < verts.size(); j++)
            {
               verts[i].edges.push_back(edge(dist(verts[i], verts[j]), j));
               //if(i == 0)
               verts[j].edges.push_back(edge(dist(verts[i], verts[j]), i));
                
            }
        }
    
    
    int verticesLeft = verts.size() - 1;
    vector<int> next;
    vector<path> paths(fact(verticesLeft));
    for(unsigned int i = 0; i < verts[0].edges.size(); i++)
    {
        next.push_back(verts[0].edges[i].adj);
    }
    while(verticesLeft > 0)
    {
        vector<int> temp;
        for(unsigned int i = 0; i < next.size(); i++)
        {
            if(!verts[next[i]].visited)
            {
                
                
                temp.push_back(next[i]);
            }
        }
        verticesLeft--;
        next = temp;
    }
    }
    else if(mode == 3)
    {
        for(unsigned int i = 0; i < verts.size() - 1; i++)
        {
            for(unsigned int j = i + 1; j < verts.size(); j++)
            {
               verts[i].edges.push_back(edge(dist(verts[i], verts[j]), j));
               verts[j].edges.push_back(edge(dist(verts[i], verts[j]), i));
                
            }
        }
        vector<int> route;
        vector<int> route2;
        route.push_back(0);
        verts[0].visited = true;
        int end1 = 0, end2 = 0;
        int verticesLeft = verts.size() - 1;
        while(verticesLeft >= 1)
        {
            double minDist = 999999; 
            int minNode;
            int minEnd;
            for(unsigned int j = 0; j < verts[end1].edges.size(); j++)
            {
                    if(verts[end1].edges[j].dist < minDist && !verts[verts[end1].edges[j].adj].visited)
                    {
                        minNode = verts[verts[end1].edges[j].adj].num;
                        minDist = verts[end1].edges[j].dist;
                        minEnd = end1;
                    }
            }
            for(unsigned int j = 0; j < verts[end2].edges.size(); j++)
            {
                    if(verts[end2].edges[j].dist < minDist && !verts[verts[end2].edges[j].adj].visited)
                    {
                        minNode = verts[verts[end2].edges[j].adj].num;
                        minDist = verts[end2].edges[j].dist;
                        minEnd = end2;
                    }
            }
            if(minDist != 999999)
            {
                verts[minNode].visited = true;
                if(end1 == minEnd)
                {
                    end1 = minNode;
                    route.push_back(minNode);
                }
                else if(end2 == minEnd)
                {
                    end2 = minNode;
                    route2.push_back(minNode);
                }
                verticesLeft--;
         
                
                //totalDist += minDist;
            }
        }
        /*if(dist(verts[end1], verts[0]) < dist(verts[end2], verts[0]))
        {
            totalDist += dist(verts[end1], verts[0]);
        }
        else
        {
            totalDist += dist(verts[end2], verts[0]);
        }*/
        for(int i = route2.size() - 1; i >= 0; i--)
        {
            route.push_back(route2[i]);
        }
        for(int i = 0; i < route.size(); i++)
        {
            if(i < route.size() - 1)
            totalDist += dist(verts[route[i]], verts[route[i+1]]);
            else
                totalDist += dist(verts[route[i]], verts[route[0]]);
        }
        ostringstream oss;
        oss << setprecision(2) << fixed;
        oss << "" << totalDist << "\n";
        for(unsigned int i = 0; i < route.size(); i++)
        {
            oss << route[i] << " ";
        }
        oss << "\n";
        cout << oss.str() << flush;
    }
    
    return 0;

}

int fact(int n)
{
    int f = 1;
    for(int i = 1; i <= n; i++)
        f *= i;
    return f;
}
double dist(location a, location b)
{
    double d;
    d = (double)((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
    d = sqrt(d);
    return d; 
}