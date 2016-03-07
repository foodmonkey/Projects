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
//#include <getopt.h>
using namespace std;

struct Order
{
    long long timestamp, quantity;
    string client_name, equitySym;
    Order(long long t, string c, string e, long long q)
    {
        timestamp = t;
        quantity = q;
        client_name = c;
        equitySym = e;
    }
};
struct key
{
    long long price, ID;
    key(long long p, long long i)
    {
        price = p;
        ID = i;
    }
};
struct bcomp
{
    bool operator()(key x, key y) 
    {
        if(x.price == y.price)
            return  x.ID < y.ID;
        else
            return x.price > y.price;
    }
};
struct scomp
{
    bool operator()(key x, key y) 
    {
        if(x.price == y.price)
            return  x.ID < y.ID;
        else
            return x.price < y.price;
    }
};
struct trades
{
    string equity;
    vector<long long> tradePrices; 
    trades(string e)
    {
        equity = e;
    }
};
bool stringComp(trades x, trades y) 
{
    return x.equity < y.equity;
};
struct client
{
    string name;
    long long bought, sold, net;
    client(string n, long long b, long long s, long long ne)
    {
        name = n;
        bought = b;
        sold = s;
        net = ne;
    }
};
bool clientComp(client x, client y) 
{
    return x.name < y.name;
};

bool isal(string clientname);

int main(int argc, char **argv)
{
    
    map<key, Order*, bcomp> buyers, tbuyers;
    map<key, Order*, scomp> sellers, tsellers;
    vector <trades> medians;
    vector <client> clients; 
    vector <string> equit;
    vector <string> insiders;
    long long bought = 0;
    ostringstream oss;
    bool summary = true, verbose = true, median = true;
    bool  transfers = true, insider = true, ttt = true;/*
    bool summary = false, verbose = false, median = false;
    bool  transfers = false, insider = false, ttt = false;/*
    
    /*
    struct option longOpts[] = 
    {
        {"verbose", required_argument, NULL, 'v'},
        {"v", required_argument, NULL, 'v'},
        {"summary", no_argument, NULL, 's'},
        {"s", no_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {"h", no_argument, NULL, 'h'},
        {"median", no_argument, NULL, 'm'},
        {"m", no_argument, NULL, 'm'},
        {"transfers", no_argument, NULL, 't'},
        {"t", no_argument, NULL, 't'},
        {"ttt", required_argument, NULL, 'g'},
        {"g", required_argument, NULL, 'g'},
        {"insider", required_argument, NULL, 'i'},
        {"i", required_argument, NULL, 'i'},
    };
    int opt = 0, index = 0;
    while((opt = getopt_long (argc, argv, "svmti:g:h", longOpts, &index)) != -1)
    {
        switch(opt) 
        {
            case 'h': 
                cout << "HELP: -s --summary \n" <<
                    "-v --verbose\n" <<
                    "-m --median\n" << "-t --transfers\n" << "-i --insider EQUIY_SYMBOL\n" << 
                    "-g --ttt EQUITY_SYMBOL\n" << "-h --help\n";
                exit(0);
                break;
            case 'v': 
                verbose = true;
                break;
            case 't': 
                transfers = true;
                break;
            case 's': 
                summary = true;
                break;
            case 'm': 
                median = true;
                break;
            case 'g': 
                ttt = true;
                equit.push_back(optarg);
                break;
            case 'i': 
                insider =true;
                insiders.push_back(optarg);
                break;
        }
    }*/
    string type, client_name, buyorsell, equitySym, curLine;
    long long timestamp, price, quantity, min = 0;
    long long tradePrice, commission, num_shares, total_com = 0,
        comp_trades = 0, shares_traded = 0, money_transferred = 0;
    long long  numOrd, count = 0, count2 = 0;
    long long currTime = 0;
    unsigned long random_seed = 1;
    double arrival_rate = 1;
    char last_client = 'b', last_equity = 'B', junk = ' ';
    cin >> type;
    if (!type.compare("PR"))
    {
        string junks;
        cin >> junks >> random_seed >> junks >> numOrd >> junks >> last_client
            >> junks >> last_equity >> junks >> arrival_rate;
    }
    long long generator_timestamp = 0;
    mt19937 gen(random_seed);    uniform_int_distribution<char> clientz('a', last_client);    uniform_int_distribution<char> equities('A', last_equity);
    exponential_distribution<> arrivals(arrival_rate);
    bernoulli_distribution buy_or_sell;
    uniform_int_distribution<> prices(2, 11);
    uniform_int_distribution<> quantities(1, 30);
    if(insider)
    for(int i = 0; i < insiders.size(); i++)
    {
        clients.push_back(client("INSIDER_"+insiders.at(i), 0, 0, 0));
    }
    while((!type.compare("TL") && cin.good()) || (!type.compare("PR") && count2 < numOrd))
    {
        if(!type.compare("TL"))
        {
            if(cin >> timestamp >> client_name >> buyorsell >> equitySym)
            {
                cin >> junk; 
                if (junk != '$') 
                    exit(1);
                cin >> price; 
                cin >> junk;
                if(junk != '#')
                    exit(1);
                cin >> quantity;
                if(timestamp < 0 || equitySym.length() > 5 || equitySym.length() < 1 ||
                    (buyorsell.compare("BUY") && buyorsell.compare("SELL")) || price <= 0 
                    || !isal(client_name) || min > timestamp)
                    exit(1);
                min = timestamp;

            }
            else
                break;
        }
        else if(!type.compare("PR"))
        {
            timestamp = generator_timestamp;
            generator_timestamp = generator_timestamp + floor(arrivals(gen) + .5);
            client_name = string("C_") + clientz(gen);
            buyorsell = (buy_or_sell(gen) ? "BUY" : "SELL");
            equitySym = string("E_") + equities(gen);
            price = 5 * prices(gen);
            quantity = quantities(gen);
            // oss << timestamp <<" " << client_name << " " << buyorsell <<  " " << equitySym << " " <<  price << " " << quantity << " " << count << "\n";
            count2++;
        }   
        if(timestamp != currTime)
        {
            //printmedian
            if(median && medians.size() > 0 )
                        {
                            sort(medians.begin(), medians.end(), stringComp);
                            for(int i = 0; i < medians.size(); i++)
                            {
                                if(medians.at(i).tradePrices.size() % 2 == 1) 
                                {
                                    oss << "Median match price of " << medians.at(i).equity << " at time " << currTime
                                        << " is $" << medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2)) << "\n";
                                }
                                else if(medians.at(i).tradePrices.size() % 2 == 0)
                                {
                                    long long med = (long long)(medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2 - 1)) + 
                                        medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2)))/2;
                                    oss << "Median match price of " << medians.at(i).equity << " at time " << currTime
                                        << " is $" << med << "\n";
                                }

                            }
                        }
            currTime = timestamp;
        }

        if(!buyorsell.compare("BUY"))
        {
            buyers[key(price, count)] = new Order(timestamp, client_name, equitySym, quantity);
            if(ttt)
                tbuyers[key(price, count)] = new Order(timestamp, client_name, equitySym, quantity);
        }
        else if(!buyorsell.compare("SELL"))
        {
            sellers[key(price, count)] = new Order(timestamp, client_name, equitySym, quantity);
            if(ttt)
                tsellers[key(price, count)] = new Order(timestamp, client_name, equitySym, quantity);
        }
        //insider
        bool valid = false;
        bought = 0;
        if(insider)
        {
            for(int i = 0; i < insiders.size(); i ++)
            {
                if(!equitySym.compare(insiders.at(i)))
                {
                    valid = true;
                }
            }
        }
        if(transfers)
        {
            bool f = false;
            for(int i = 0; i < clients.size(); i ++)
            {
                if(!client_name.compare(clients.at(i).name))
                {
                    f = true;
                }
            }
            if (!f)
            clients.push_back(client(client_name, 0, 0, 0));
                
        }
        count ++;
        bool brk = false;
        bool process = true;

        while(process)
        {
           process = false;
            //  if(buyers.size() > 0 && sellers.size() > 0)
            
            for(auto it = buyers.begin(); it != buyers.end(); it ++)
            {
                //if(!it->second->equitySym.compare(equitySym))
                for(auto it2 = sellers.begin(); it2 != sellers.end(); it2++)
                {
                     if(!it2->second->equitySym.compare(it->second->equitySym))
                     {
                    if(it->first.price < it2->first.price)// && !it->second->equitySym.compare(it2->second->equitySym)
                       // && !it->second->equitySym.compare(equitySym))
                    {
                        // brk = true;
                        break;                    
                    }
                    if(it->second->quantity > 0 && it2->second->quantity > 0)
                    {
                        if(it->second->quantity <= it2->second->quantity)
                            num_shares = it->second->quantity;
                        else
                            num_shares = it2->second->quantity;
                        if(it->first.ID < it2->first.ID)
                            tradePrice = it->first.price;
                        else
                            tradePrice = it2->first.price;
                        commission = (tradePrice * num_shares) / 100;
                        it->second->quantity -= num_shares;
                        it2->second->quantity -= num_shares;
                        if(summary)
                        {
                            total_com += commission * 2; 
                            money_transferred += tradePrice * num_shares;
                            comp_trades++; 
                            shares_traded += num_shares;
                        }
                        if(verbose)
                        {
                            oss << it->second->client_name << " purchased " << num_shares << " shares of " <<
                                it->second->equitySym << " from " << it2->second->client_name << " for $"
                                << tradePrice << "/share\n";
                        }
                        if(median || insider)
                        {
                            bool found = false;
                            for(unsigned int i = 0; i < medians.size(); i++)
                            {
                                if(!medians.at(i).equity.compare(it->second->equitySym))
                                {
                                    medians.at(i).tradePrices.push_back(tradePrice);
                                    sort(medians.at(i).tradePrices.begin(),medians.at(i).tradePrices.end());
                                    found = true;
                                    break;
                                }
                            }
                            if(medians.size() <= 0 || !found)
                            {
                                medians.push_back(trades(it->second->equitySym));
                                medians.at(medians.size()-1).tradePrices.push_back(tradePrice);
                            }
                        }

                        if(transfers)
                        {
                            bool found1 = false, found2 = false;
                            for(unsigned int i = 0; i < clients.size(); i++)
                            {
                                if(!clients.at(i).name.compare(it->second->client_name))
                                {
                                    clients.at(i).bought += num_shares;
                                    clients.at(i).net -= tradePrice * num_shares;
                                    found1 = true;
                                }
                                if(!clients.at(i).name.compare(it2->second->client_name))
                                {
                                    clients.at(i).sold += num_shares;
                                    clients.at(i).net += tradePrice * num_shares;
                                    found2 = true;
                                }
                                if(found1 && found2)
                                    break;
                            }
                            if(!found1)
                            {
                                if(!it->second->client_name.compare(it2->second->client_name))
                                    clients.push_back(client(it->second->client_name, num_shares, num_shares, 0));
                                else
                                clients.push_back(client(it->second->client_name, num_shares, 0, -tradePrice * num_shares));

                            }
                            if(!found2 && it->second->client_name.compare(it2->second->client_name))
                            {
                                clients.push_back(client(it2->second->client_name, 0, num_shares, tradePrice * num_shares));
                            }
                        }
                        if(it->second->quantity <= 0)
                        {
                            buyers.erase(it);
                            if(buyers.size() <= 0)
                            {
                                brk = true;
                                break;
                            }
                            it = buyers.begin();
                        }
                        if(it2->second->quantity <= 0)
                        {
                            
                            sellers.erase(it2);
                            if(sellers.size() <= 0)
                            {
                                brk = true;
                                break;
                            }
                            it2 = sellers.begin();
                            
                        }
                        process = true;
                    }
                        }
                }
                if(brk)
                    break;
            }
            
            
            if(insider && valid && bought == 0)
            {
                for(int i = 0; i < medians.size(); i++)
                {
                    if(!medians.at(i).equity.compare(equitySym))
                    {
                        long long med;
                        if(medians.at(i).tradePrices.size() % 2 == 1) 
                        {
                            med = medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2));
                        }
                        else if(medians.at(i).tradePrices.size() % 2 == 0)
                        {
                            med = (long long)(medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2 - 1)) + 
                                medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2)))/2;
                        }
                        for(auto it3 = sellers.begin(); it3 != sellers.end(); it3++)
                        {
                            if(!it3->second->equitySym.compare(equitySym))
                            {
                                if(10 * (med - it3->first.price) > med)
                                {
                                    buyers[key(it3->first.price, it3->first.ID)] =
                                        new Order(currTime, "INSIDER_"+equitySym, equitySym, it3->second->quantity);
                                    if(ttt)
                                        tbuyers[key(it3->first.price, it3->first.ID)] =
                                        new Order(currTime, "INSIDER_"+equitySym, equitySym, it3->second->quantity);
                                    
                                    count++;
                                }
                                break;
                            }
                        }
                    }

                }
                bought = 1;
                process = true;
            }
            else if(insider && valid && bought == 1)
            {
                for(int i = 0; i < medians.size(); i++)
                {
                    if(!medians.at(i).equity.compare(equitySym))
                    {
                        long long med;
                        if(medians.at(i).tradePrices.size() % 2 == 1) 
                        {
                            med = medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2));
                        }
                        else if(medians.at(i).tradePrices.size() % 2 == 0)
                        {
                            med = (long long)(medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2 - 1)) + 
                                medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2)))/2;
                        }
                        for(auto it3 = buyers.begin(); it3 != buyers.end(); it3++)
                        {
                            if(!it3->second->equitySym.compare(equitySym))
                            {
                                if(10 * (it3->first.price - med) > med)
                                {
                                    sellers[key(it3->first.price, count)] =
                                        new Order(currTime, "INSIDER_"+equitySym, equitySym, it3->second->quantity);
                                    if(ttt)
                                        tsellers[key(it3->first.price, count)] =
                                        new Order(currTime, "INSIDER_"+equitySym, equitySym, it3->second->quantity);
                                    process = true;
                                    count++;
                                }
                                else
                                {
                                    process = false;
                                }
                                break;
                            }
                        }
                    }
                }
                bought = 2;
            }
        }
    }
    if(median && medians.size() > 0 )
                        {
                            sort(medians.begin(), medians.end(), stringComp);
                            for(int i = 0; i < medians.size(); i++)
                            {
                                if(medians.at(i).tradePrices.size() % 2 == 1) 
                                {
                                    oss << "Median match price of " << medians.at(i).equity << " at time " << currTime
                                        << " is $" << medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2)) << "\n";
                                }
                                else if(medians.at(i).tradePrices.size() % 2 == 0)
                                {
                                    long long med = (long long)(medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2 - 1)) + 
                                        medians.at(i).tradePrices.at((long long)(medians.at(i).tradePrices.size()/2)))/2;
                                    oss << "Median match price of " << medians.at(i).equity << " at time " << currTime
                                        << " is $" << med << "\n";
                                }

                            }
                        }
    oss << "---End of Day---\n";
    if(summary)
    {
        oss << "Commission Earnings: $" << total_com << "\n";
        oss << "Total Amount of Money Transferred: $" << money_transferred << "\n";
        oss << "Number of Completed Trades: " << comp_trades << "\n";
        oss << "Number of Shares Traded: " << shares_traded << "\n";
    }
    if(transfers && clients.size() > 0)
    {
        sort(clients.begin(), clients.end(), clientComp);
        for(int i = 0; i < clients.size(); i++)
        {
            oss << clients.at(i).name << " bought " << clients.at(i).bought << " and sold " <<
                clients.at(i).sold << " for a net transfer of $" << clients.at(i).net << "\n";
        }
    }
    if(ttt)
    {
        for(int i = 0; i < equit.size(); i++)
        {
            long long max = -999999;
            long long timestamp1 = -1, timestamp2 = -1;
            for(auto it = tbuyers.begin(); it != tbuyers.end(); it ++)
            {
                if(!equit.at(i).compare(it->second->equitySym))    
                    for(auto it2 = tsellers.begin(); it2 != tsellers.end(); it2++)
                    {
                        if(!equit.at(i).compare(it2->second->equitySym))
                            if(it2->first.ID < it->first.ID)// && it->first.price - it2->first.price >= max)
                            {
                                if(it->first.price - it2->first.price > max ||
                                    (it->first.price - it2->first.price == max
                                    && it2->second->timestamp < timestamp1 && it->second->timestamp < timestamp2))
                                {
                                    timestamp1 = it2->second->timestamp;
                                    timestamp2 = it->second->timestamp;
                                    max = it->first.price - it2->first.price;
                                }
                            }
                    }
            }
            oss << "Time travelers would buy " << equit.at(i) << " at time: " << timestamp1 <<
                " and sell it at time: " << timestamp2 << "\n";
        }
    }
    cout << oss.str() << flush;
    return 0;
}

bool isal(string clientName)
{
    for (unsigned int i=0; i<clientName.size(); ++i) 
    {
        if( !( isalnum(clientName[i]) || (clientName[i]=='_') ) )
            return false;
    }
    return true;
}
