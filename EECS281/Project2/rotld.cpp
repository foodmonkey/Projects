#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list> 
#include <deque>
#include <algorithm>
#include <sstream>
#include "poorman_priority_queue.h"
#include "pairing_heap.h"
#include "binary_heap.h"
#include "sorted_priority_queue.h"
#include "eecs281priority_queue.h"
//#include <getopt.h>
using namespace std;

struct zombie
{
    string name;
    int roundAct, dist, speed, health, rdsActive;
    bool active; 
    zombie(int c, int d, int s, int h, string n)
    {
        roundAct = c; 
        dist = d;
        speed = s;
        health = h;
        name = n;
        rdsActive = 0;
    }
    zombie()
    {
        roundAct = 5; 
        dist = 1;
        speed = 1;
        health = 1;
    }
};
struct comp
{
    bool operator()(zombie * x, zombie* y) const
    {
        if((*x).dist / (*x).speed == (*y).dist / (*y).speed)
            return  (*x).name > (*y).name;
        else
            return (*x).dist / (*x).speed > (*y).dist / (*y).speed ;
    }
};
 bool active(zombie * x, zombie* y) 
    {
        if((*x).rdsActive == (*y).rdsActive)
            return  (*x).name < (*y).name;
        else
        return (*x).rdsActive > (*y).rdsActive;
    }
 bool active2(zombie * x, zombie* y) 
    {
        if((*x).rdsActive == (*y).rdsActive)
            return  (*x).name > (*y).name;
        else
        return (*x).rdsActive > (*y).rdsActive;
    }

int main()//int argc, char **argv)
{
    int quiv_capac, rand_seed, max_rand_dist, max_rand_speed, max_rand_health, pla_health;
    int round = 0, num_zom = 0, start_dist, speed, health, arrows, counter = 0;
    string killer, opts;
    ostringstream oss;
    int currRound = 1, verbosen = 9;
    vector<string> killed;
    bool read = false, eof = false;
    bool debug = true, verbose = true;
    bool normal = true, light = false;
    //char q = 'p';
    list<zombie*> masterList;
    eecs281priority_queue<zombie *, comp> *pq;
    pq = new pairing_heap<zombie*, comp>;
    /*
     //get opts
    struct option longOpts[] = 
    {
        {"verbose", required_argument, NULL, 'v'},
        {"v", required_argument, NULL, 'v'},
        {"arrow", required_argument, NULL, 'a'},
        {"a", required_argument, NULL, 'a'},
        {"help", no_argument, NULL, 'h'},
        {"h", no_argument, NULL, 'h'},
        {"container", required_argument, NULL, 'c'},
        {"c", required_argument, NULL, 'c'},
        {"debug", no_argument, NULL, 'd'},
        {"d", no_argument, NULL, 'd'}
    };

    int opt = 0, index = 0;
    while((opt = getopt_long (argc, argv, "v:a:c:hd", longOpts, &index)) != -1)
    {
        switch(opt) 
        {
            case 'h':
                cout << "NAME\n\trotld ­ a command line ‘Revenge of the Living Deadline’ game simulation program.";                cout << "\nSYNOPSIS\n\trotld (­c | ­­container) CONTAINER (­a | ­­arrow ) ARROW [­v | ­­verbose N] GAMEFILE";                cout << "\n\trotld (­h | ­­help)";                cout << "\nDESCRIPTION\n\tROTLD (Revenge of the Living Deadline) game simulator with both zombie and player";                cout << "\n\tAI. This program acts as the player in a ROTLD game and shoots zombies in the";                cout << "\n\toptimal ordering to achieve the best possible result (stay alive for as long as";                cout << "\n\tpossible).\nOPTIONS\n­-h, --help";                cout << "\n\tPrint this help screen and exit.\n­-c, ­­--container";                cout << "\n\tRequired option that states the type of priority queue to use. Must be one of";                cout << "\n\tBINARY, POOR_MAN, SORTED, or PAIRING.";                cout << "\n-a, --arrow\n\tRequired option that states the type of arrows to use. Must be one of NORMAL, LIGHT.";                cout << "\n­-v, --­­verbose N\n\tAn optional option that indicates the program should print additional output";                cout << "\n\tstatistics.";                cout << "\n­-d, --­­debug";                cout << "\n\tAn optional option that indicates the program should print some debugging output.";                exit(1);
                break;
            case 'd':
                debug = true;
                break;
            case 'v':
                verbose = true;
                
                verbosen = atoi(optarg);
                break;
            case 'c':
                opts.assign(optarg);
                if(!opts.compare("BINARY"))
                    pq = new binary_heap<zombie*, comp>;
                else if(!opts.compare("POOR_MAN"))
                    pq = new poorman_priority_queue<zombie*, comp>;
                else if(!opts.compare("SORTED"))
                pq = new sorted_priority_queue<zombie*, comp>;
                else if(!opts.compare("PAIRING"))
                pq = new pairing_heap<zombie*, comp>;
                break;
            case 'a':
                opts.assign(optarg);
                if(!opts.compare("NORMAL"))
                    normal = true;
                else if(!opts.compare("LIGHT"))
                    light = true;
                break;
        }
    }
           string filename = "";
           while (optind < argc)
           {
               filename += argv[optind++];
           }
    if(!filename.compare(""))
        exit(1);*/
    string junk, name; 
    ifstream file;
    file.open("sample.txt");
    file >> junk >> quiv_capac >> junk >> rand_seed >> junk >> max_rand_dist >> junk;
    file >> max_rand_speed >> junk >> max_rand_health >> junk >> pla_health;
    file >> junk;
    if(!file.good())
        eof = true;
    srand(rand_seed);
    while(1)
    {
        if(debug)
            oss << "Round: " << currRound << "\n";
        arrows = quiv_capac;
        for(auto it = masterList.begin(); it != masterList.end(); ++it)
        {
            if((*it) -> health > 0)
            {
                int random_offset = (rand() % (*it) -> speed) / 2;
                (*it) -> dist = (*it) -> dist - (*it) -> speed - random_offset;
                if((*it) -> dist < 0)
                {
                    (*it) -> dist = 0;
                }
                if(debug)
                    oss << "Moved: " << (*it)->name << " " << (*it)->dist << "\n"; 
                if((*it)->dist == 0 && pla_health > 0)
                {
                    if(normal)
                    {
                        pla_health -= (*it)->health;
                    }
                    else if(light)
                    {
                        pla_health = 0;
                    }
                    if(pla_health <= 0)
                    {
                        killer = (*it)->name;
                    }

                }
            }
        }
        
        if(!read && !eof)
        {
            file >> junk >> round >> junk >> num_zom;
            read = true;
                           
        }
        if(round == currRound && read)
        {
            for(int i = 0; i < num_zom; i++)
            {
                start_dist = rand() % max_rand_dist + 1;
                speed = rand() % max_rand_speed + 1;
                health = rand() % max_rand_health + 1;
                name = "AI";
                name += to_string(static_cast<long long>(counter++));
                masterList.push_back(new zombie(round, start_dist, speed, health, name));
                if(debug)
                {
                    oss << "Created: " << masterList.back()->name << " " << masterList.back()->dist << "\n";
                }
                pq->push(masterList.back());
            }
            while(file.good())
            {
                file >> start_dist >> speed >> health >> name;
                if(file.good())
                {
                    masterList.push_back(new zombie(round, start_dist, speed, health, name));
                    if(debug)
                    {
                        oss << "Created: " << masterList.back()->name << " " << masterList.back()->dist << "\n";
                    }
                    pq->push(masterList.back());
                }

            }
            
            file.clear();
            file >> junk;
            if(!file.good())
            eof = true;
            read = false;
        }
        if(pla_health <= 0)
        {
            oss << "DEFEAT IN ROUND " << currRound << "! ";
            oss << killer << " ate your brains!" << "\n";    
            break;  
        }
        
        pq->make_heap();
        //delete pq;
        while(arrows > 0 && !pq->empty())
        {
            if(normal)
                pq->top()->health -= 1;
            else if(light)
                pq->top()->health = 0;
            //oss << "you shot " << pq->top()->name << "\n";
            if(pq->top()->health <= 0)
            {
                //oss << pq->top()->name << " was killed" << "\n";
                pq->top()->rdsActive = currRound - pq->top()->roundAct + 1;
                killed.push_back(pq->top()->name);
                pq->pop();
            }
            arrows--;
        }
        if(pq->empty() && eof)
            {
                oss << "VICTORY IN ROUND " << currRound << "! " << killed.back() << " was the last ";
                oss << "zombie. You survived with " << pla_health << " health left.\n";
                break;
            }
       // oss << "health: " << pla_health << "\n";
        currRound++;

    }
    if(verbose)
    {
        
        oss << "Zombies still active: " << pq->size() << "\n";
        oss << "First zombies killed: " << "\n";
        int mi = min(verbosen, (int)killed.size());
        for(int i = 1; i <= mi; i++)
        {
            oss << killed.at(i-1) << " " << i << "\n";
        }
        oss << "Last zombies killed: " << "\n";
        for(int i = mi; i >= 1; i--)
        {
            oss << killed.at((int)killed.size() - (mi - i) - 1) << " " << i << "\n";
        }
        for(auto it = masterList.begin(); it != masterList.end(); ++it)
        {
            if((*it)->rdsActive == 0 && currRound - (*it)->roundAct > 0)
            (*it)->rdsActive = currRound - (*it)->roundAct + 1;

        }
        masterList.sort(active);
        mi = min(verbosen, (int)masterList.size());
        oss << "Most active zombies: " << "\n";
        auto it = masterList.begin();
        int count = 0;
        while(count < mi)
        {
            oss << (*it)->name << " " << (*it)->rdsActive << "\n";
            it++;
            count ++;
        }

        masterList.sort(active2);
        oss << "Least active zombies: " << "\n";
        it = prev(masterList.end());
        count = 0;
        while(count < mi)
        {
            oss << (*it)->name << " " << (*it)->rdsActive << "\n";
            it--;
            count ++;
        }
    }
    cout << oss.str() << flush;
    return 0; 
    } 

