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
//#include <getopt.h>
using namespace std;

struct edge
{ 
    location a, b;
    edge(location x, location y)
    {
        a = x;
        b = y;
    }
};
struct location
{
    int in, x,y,num;

    bool visited;
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
struct MSTCoord
{
        MSTCoord(location *a)
        {
                c = a;
                d = -1;
                x = NULL;
        }
        location *c;
        int d;
        location *x;
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
double findDist(vector<location> &verts, vector<int> &path, int a);
bool findOPT(vector<location> &verts, vector<int> &remain, vector<int> &path, double &best, vector<int> &bestPath);
double MST(vector<location> &verts, vector<int> &path);
int main(int argc, char **argv)
{
    int lx, ly, rx, ry, mode = 1;
    int m, tx, ty, temp;/*
    string opts;
    struct option longOpts[] = 
    {
        {"mode", required_argument, NULL, 'm'},
        {"m", required_argument, NULL, 'm'},
        {"help", no_argument, NULL, 'h'},
        {"h", no_argument, NULL, 'h'},
    };
    int opt = 0, index = 0;
    while((opt = getopt_long (argc, argv, "m:h", longOpts, &index)) != -1)
    {
        switch(opt) 
        {
        case 'h': 
            cout << "HELP: -m --mode" <<
                "MST is mst\n" <<
                "OPTTSP is optimum TSP\n" <<
                "FASTTSP is fast calculation of TSP\n";
            exit(0);
            break;
        case 'm': 
            opts.assign(optarg);
            if(!opts.compare("MST"))
            {
                mode = 1;
            }
            else if(!opts.compare("OPTTSP"))
            {
                mode = 2;
            }
            else if(!opts.compare("FASTTSP"))
            {
                mode = 3;
            }
            break;
        }
    }*/
    double totalDist = 0;
    vector <location> verts;
    vector <int> visit;
    
    vector <opair> mst;
    cin >> lx >> ly >> rx >> ry;
    cin >> m;
    for(int i = 0; i < m; i++)
    {
        cin >> tx >> ty;
        if(mode == 1)
        {
        if(tx > lx && tx < rx && ty > ly && ty < ry)
            temp = 0;
        else if((tx == lx || tx == rx) && (ty > ly && ty < ry)
            || (ty == ry || ty == ly) && (tx > lx && tx < rx))
            temp = 1;
        else 
            temp = 2;
        
            verts.push_back(location(tx, ty, temp,i));
        }
        else
        {
            verts.push_back(location(tx, ty, 0,i));
        }
    }
    if(mode == 1)
    {
       vector<MSTCoord*> mstlocs;
       vector<edge*> edges;
                mstlocs[0]->d = 0;
                int total = 0;
                while (mstlocs.size() > 0)
                {
                        MSTCoord *a = mstlocs[0];
                        int index = 0;
                        for (int i = 0; i < mstlocs.size(); i++)
                        {
                                if (a->d == -1 || (a->d > mstlocs[i]->d && mstlocs[i]->d != -1))
                                {
                                        a = mstlocs[i];
                                        index = i;
                                }
                        }
                        total += a->d;
                        location *first = a->c, *second = a->x;
                        if (second != NULL)
                        {
                                if (first > second)
                                {
                                        first = second;
                                        second = a->c;
                                }
                                edges.push_back(new edge(first, second));
                        }
                        MSTCoord *z = mstlocs[index];
                        mstlocs[index] = mstlocs[mstlocs.size() - 1];
                        mstlocs.pop_back();
                        for (int i = 0; i < mstlocs.size(); i++)
                        {
                                if (mstlocs[i]->d == -1 || mstlocs[i]->c->distTo(*(a->c)) < mstlocs[i]->d)
                                {
                                        mstlocs[i]->d = mstlocs[i]->c->distTo(*(a->c));
                                        mstlocs[i]->x = a->c;
                                }
                        }
                        delete z;
                }
                cout << total << "\n";
                for (int i = 0; i < edges.size(); i++)
                {
                        cout << edges[i]->a->getIndex() << " " << edges[i]->b->getIndex() << endl;
                }
                return 0;
        }
        /*
        //vector<vector<int>> adjacent;
        vector <int> unvisit0, unvisit1, unvisit2;
        vector<int> visit;
        visit.push_back(0);
        for(unsigned int i = 1; i < verts.size(); i ++)
        {
            if(verts[i].in == 0)
                unvisit0.push_back(i);
            else if(verts[i].in == 1)
                unvisit1.push_back(i);
            else if(verts[i].in == 2)
                unvisit2.push_back(i);
        }
        int verticesLeft = verts.size() - 1;
        while(verticesLeft >= 1)
        {
            double minDist = 1.79769e+308; 
            int minNode, minNode2, index, loc;
            for(unsigned int i = 0; i <visit.size(); i++)
            {
                int node = visit[i];
                if(verts[node].in == 0||verts[node].in == 1)
                {
                    for(unsigned int j = 0; j < unvisit0.size(); j++)
                    {
                        int unode = unvisit0[j];
                        if(dist(verts[node], verts[unode]) < minDist)
                        {
                            minNode2 = unode;
                            minNode = node;
                            minDist = dist(verts[node], verts[unode]);
                            index = j;
                            loc = 0;
                        }
                    }
                }
                if(verts[node].in == 0||verts[node].in == 1||verts[node].in == 2)
                {
                    for(unsigned int j = 0; j < unvisit1.size(); j++)
                    {
                        int unode = unvisit1[j];
                        if(dist(verts[node], verts[unode]) < minDist)
                        {
                            minNode2 = unode;
                            minNode = node;
                            minDist = dist(verts[node], verts[unode]);
                            index = j;
                            loc = 1;
                        }
                    }
                }
                if(verts[node].in == 1||verts[node].in == 2)
                {
                    for(unsigned int j = 0; j < unvisit2.size(); j++)
                    {
                        int unode = unvisit2[j];
                        if(dist(verts[node], verts[unode]) < minDist)
                        {
                            minNode2 = unode;
                            minNode = node;
                            minDist = dist(verts[node], verts[unode]);
                            index = j;
                            loc = 2;
                        }
                    }
                }
            }
            if(minDist != 1.79769e+308)
            {
                visit.push_back (minNode2);
                if(loc == 0)
                unvisit0.erase(unvisit0.begin() + index);
                else if(loc == 1)
                   unvisit1.erase(unvisit1.begin() + index);   
                else if(loc == 2)
                   unvisit2.erase(unvisit2.begin() + index);   
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
        cout << setprecision(2) << fixed;
        cout << "" << totalDist << "\n";
        for(unsigned int i = 0; i < mst.size(); i++)
        {
            cout << mst[i].node1 << " " << mst[i].node2 << "\n";
        }*/
    }
    else if(mode == 2)
    {
        vector<int> remain;
        vector<int> path;
        vector<int> best;
        
        for(unsigned int i = 1; i < verts.size(); i++)
        {
            remain.push_back(i);
            
        }
        double minDist = 1.79769e+308;
        path.push_back(0);
        findOPT(verts, remain, path, minDist, best);
        while(best[0] != 0)
        {
            best.push_back(best[0]);
            best.erase(best.begin());
        }
        cout << setprecision(2) << fixed;
        cout << "" << minDist << "\n";
        for(unsigned int i = 0; i < best.size(); i++)
        {
            cout << best[i] << " ";
        }
        cout << "\n";
        
    }
    else if(mode == 3)
    {
        double minimum = 1.79769e+308;
        int end1 = 0, end2 = 0;
        vector<int> route;
        vector<int> route2;
        for(unsigned int i = 0; i < verts.size(); i++)
        {
            for(unsigned int j = i + 1; j < verts.size(); j++)
            {
                if(dist(verts[i], verts[j]) < minimum)
                {
                    minimum = dist(verts[i], verts[j]);
                    end1 = i;
                    end2 = i;
                }
            }
        }
        route.push_back(end1);
        verts[end1].visited = true;
        int verticesLeft = verts.size() - 1;
        while(verticesLeft >= 1)
        {
            double minDist = 1.79769e+308; 
            int minNode;
            int minEnd;
            for(unsigned int j = 0; j < verts.size(); j++)
            {
                if(!verts[j].visited && dist(verts[end1], verts[j]) < minDist )
                {
                    minNode = j;
                    minDist = dist(verts[end1], verts[j]);
                    minEnd = end1;
                }
            }
            for(unsigned int j = 0; j < verts.size(); j++)
            {
                if(!verts[j].visited && dist(verts[end2], verts[j]) < minDist )
                {
                    minNode = j;
                    minDist = dist(verts[end2], verts[j]);
                    minEnd = end2;
                }
            }
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


        }
        for(int i = route2.size() - 1; i >= 0; i--)
        {
            route.push_back(route2[i]);
        }
        while(route[0] != 0)
        {
            route.push_back(route[0]);
            route.erase(route.begin());
        }
        for(int i = 0; i < route.size(); i++)
        {
            if(i < route.size() - 1)
                totalDist += dist(verts[route[i]], verts[route[i+1]]);
            else
                totalDist += dist(verts[route[i]], verts[route[0]]);
        }
        cout << setprecision(2) << fixed;
        cout << "" << totalDist << "\n";
        for(unsigned int i = 0; i < route.size(); i++)
        {
            cout << route[i] << " ";
        }
        cout << "\n";
        //cout << oss.str() << flush;
    }

    return 0;

}
double MST(vector<location> &verts, vector<int> &path)
{
    if(path.size() < 3)
        return 0;
    vector <int> unvisit0, visit;
    double totalDist = 0;
    visit.push_back(path[0]);
    for(unsigned int i = 1; i < path.size(); i ++)
        {
            unvisit0.push_back(path[i]);
        }
        int verticesLeft = path.size() - 1;
        while(verticesLeft >= 1)
        {
            double minDist = 1.79769e+308; 
            int minNode, minNode2, index;
            for(unsigned int i = 0; i < visit.size(); i++)
            {
                int node = visit[i];
                    for(unsigned int j = 0; j < unvisit0.size(); j++)
                    {
                        int unode = unvisit0[j];
                        if(dist(verts[node], verts[unode]) < minDist)
                        {
                            minNode2 = unode;
                            minNode = node;
                            minDist = dist(verts[node], verts[unode]);
                            index = j;
                        }
                    }
                
            }
             visit.push_back(minNode2);
                unvisit0.erase(unvisit0.begin() + index);
                verticesLeft--;
                totalDist += dist(verts[minNode], verts[minNode2]);
        }
        return totalDist;
}
bool findOPT(vector<location> &verts, vector<int> &remain, vector<int> &path, double &best, vector<int> &bestPath)
{
    if(remain.size() == 0)
    {
        if(findDist(verts, path,0) < best)
        {
            bestPath = path;
            best = findDist(verts, path,0);
            
        }
        return true;
    }
    if(MST(verts, remain) + findDist(verts, path,1) >= best)
    {
        return true;
    }
    
    for(unsigned int i = 0; i < remain.size(); i++)
    {
        path.push_back(remain[i]);
        remain.erase(remain.begin()+i);
        if(findOPT(verts, remain, path, best, bestPath))
        {
            remain.push_back(path.back());
            path.erase(path.end()-1);
           // break;
        }
    }
}
double findDist(vector<location> &verts, vector<int> &path, int a)
{
    double total = 0;
    for(int i = 0; i < path.size(); i++)
        {
            if(i < path.size() - 1)
                total += dist(verts[path[i]], verts[path[i+1]]);
            else if(a == 0)
                total += dist(verts[path[i]], verts[path[0]]);

        }
    return total;
        
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
    double ax = a.x, bx = b.x, ay = a.y, by = b.y;
    return sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by));
    
}