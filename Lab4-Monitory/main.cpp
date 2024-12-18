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

class Warehouse: public Monitor{
public:
    Warehouse(int k, std::string file_path) :
        k(k), file_path(file_path), empty(), full() {
    }

    void store(int amount) {
        enter();
        
        leave();
    };

private:
    int k;
    std::string file_path;
    Condition full, empty;

    int read_content() {
        int content;
        std::ifstream file(file_path);
        file >> content;
        file.close();
        return content;
    }

    int write_content() {

    }

};

// <================================================>
//                      Producer
// <================================================>

class Producer {
public:
    Producer(int id, int a, int b) {
        this->id = id;
        this->a = a;
        this->b = b;
        log_path = "producer_logs/p" + std::to_string(id) + ".txt";
    }
    
    void run(Warehouse *wh) {

        // Init random generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(a, b);

        while (true) {
            int produced = dist(gen);
            bool stored = false;

            while (!stored) {

            }
        }
    }
private:
    int id;
    int a;
    int b;
    std::string log_path;
}

// <================================================>
//                  Main Program
// <================================================>

int main(int argc, char *argv[]) {
        int k = 20;
        int n = 9;
        int a = 10;
        int b = 20;
        int m = 1;
        int c = 17;
        int d = 20;

    if (argc == 8) {
        k = atoi(argv[1]);
        n = atoi(argv[2]);
        a = atoi(argv[3]);
        b = atoi(argv[4]);
        m = atoi(argv[5]);
        c = atoi(argv[6]);
        d = atoi(argv[7]);
    }
    
    Warehouse wh = Warehouse(k, "warehouse.txt");

    for (int i=0; i<n; i++) {
      Producer pd = Producer(i, a, b);
       
    }

    // Keep program alive
    while (1) {}

    return 0;
}
