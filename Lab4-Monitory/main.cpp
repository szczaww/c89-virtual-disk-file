#include "monitor.h"
#include <thread>
#include <random>
#include <string>
#include <iostream>
#include <fstream>

// <================================================>
//               Warehouse as monitor
// <================================================>

class Warehouse : public Monitor
{
public:
    Warehouse(int k, std::string file_path) : k(k), file_path(file_path), not_empty(), not_full()
    {
    }

    void store(int amount, std::string pd_log)
    {
        enter();
        int content = read_content();

        std::ofstream file(pd_log, std::ios::app);
        file << "Wants to store " + std::to_string(amount) + "\n";
        file.close();

        while (content + amount > k) {
            wait(not_full);
            content = read_content();
        }
        write_content(content + amount);

        file.open(pd_log, std::ios::app);
        file << "Stored " + std::to_string(amount) + "\n";
        file.close();


        signal(not_empty);
        leave();
    };

    void take(int amount, std::string cn_log)
    {
        enter();
        int content = read_content();

        std::ofstream file(cn_log, std::ios::app);
        file << "Wants to take " + std::to_string(amount) + "\n";
        file.close();

        while (content < amount) {
            wait(not_empty);
            content = read_content();
        }
        write_content(content - amount);
        
        file.open(cn_log, std::ios::app);
        file << "Took " + std::to_string(amount) + "\n";
        file.close();

        signal(not_full);
        leave();
    };

private:
    int k;
    std::string file_path;
    Condition not_full, not_empty;

    int read_content()
    {
        int content;
        std::ifstream file(file_path);
        file >> content;
        file.close();
        return content;
    }

    void write_content(int content)
    {
        std::ofstream file(file_path);
        file << std::to_string(content) << std::endl;
        file.close();
    }

};

// <================================================>
//                      Producer
// <================================================>

class Producer
{
public:
    Producer(int id, int a, int b)
    {
        this->id = id;
        this->a = a;
        this->b = b;
        log_path = "producer_logs/p" + std::to_string(id) + ".txt";
    }

    void run(Warehouse *wh)
    {
        while (true)
        {
            int produced = rand() % b + a;
            sleep(1);
            wh->store(produced, log_path);
        }
    }

private:
    int id;
    int a;
    int b;
    std::string log_path;
};

// <================================================>
//                      Consumer
// <================================================>

class Consumer
{
public:
    Consumer(int id, int c, int d)
    {
        this->id = id;
        this->c = c;
        this->d = d;
        log_path = "consumer_logs/c" + std::to_string(id) + ".txt";
    }

    void run(Warehouse *wh)
    {

        while (true)
        {
            int consumed = rand() % d + c;
            sleep(1);
            wh->take(consumed, log_path);
        }
    }

private:
    int id;
    int c;
    int d;
    std::string log_path;
};

// <================================================>
//                  Main Program
// <================================================>

int main(int argc, char *argv[])
{
    int k = 20;
    int n = 9;
    int a = 1;
    int b = 5;
    int m = 1;
    int c = 1;
    int d = 5;

    if (argc == 8)
    {
        k = atoi(argv[1]);
        n = atoi(argv[2]);
        a = atoi(argv[3]);
        b = atoi(argv[4]);
        m = atoi(argv[5]);
        c = atoi(argv[6]);
        d = atoi(argv[7]);
    }

    Warehouse wh = Warehouse(k, "warehouse.txt");
    
    for (int i = 0; i < n; i++)
    {
        Producer pd = Producer(i, a, b);
        std::thread(&Producer::run, pd, &wh).detach();
    }

    for (int i = 0; i < m; i++)
    {
        Consumer cn = Consumer(i, c, d);
        std::thread(&Consumer::run, cn, &wh).detach();
    }

    while (1) {}

    return 0;
}
