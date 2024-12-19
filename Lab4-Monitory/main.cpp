#include "monitor.h"
#include <thread>
#include <iomanip>

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

        while (content + amount > k) {
            wait(not_full);
        }
        write_content(content + amount);

        signal(not_empty);
        leave;
    };

    void take(int amount, std::string cn_log)
    {
        enter();
        int content = read_content();

        while (content < amount) {
            wait(not_empty);
        }
        write_content(content + amount);

        signal(not_full);
        leave;
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

        // Init random generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(a, b);

        while (true)
        {
            int produced = dist(gen);
            sleep(1);
            wh->store(produced);
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

        // Init random generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(c, d);

        while (true)
        {
            int consumed = dist(gen);
            sleep(1);
            wh->store(-consumed);
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
    int a = 10;
    int b = 20;
    int m = 1;
    int c = 17;
    int d = 20;

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
    
    std::vector<std::thread> threads;

    for (int i = 0; i < n; i++)
    {
        Producer pd = Producer(i, a, b);
        std::thread(&Producer::run, pd, &wh).detach();
    }

    for (int i = 0; i < n; i++)
    {
        Consumer cn = Consumer(i, c, d);
        std::thread(&Consumer::run, cn, &wh).detach();
    }

    while (1) {}

    return 0;
}
