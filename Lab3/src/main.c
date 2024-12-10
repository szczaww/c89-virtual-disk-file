#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>

#include <string.h>
#include <time.h>

#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>


// <================================================>
//      Semaphore adn P&V method implementation
// <================================================>

void P(sem_t* sem){
  if (sem_wait(sem) !=0)
    __THROW "sem_wait: failed";
}

void V(sem_t* sem){
  if (sem_post(sem) !=0)
    __THROW "sem_post: failed";
}

int random(int x, int y) {
  srand(time(NULL));
  return x + rand() % (y - x + 1);
}


// <================================================>
//                      Warehouse
// <================================================>

typedef struct{
  int k;
  char file_path;
  char log_path;
  sem_t mutex;
  sem_t producer_in;
  sem_t consumer_in;
} warehouse_t;

warehouse_t* construct_warehouse(int k, int share) {
  warehouse_t* instance;

  instance->k = k;
  instance->file_path = "warehouse.txt";
  instance->log_path = "warehouse_log.txt";
  sem_init(&(instance->mutex), share, 1);
  sem_init(&(instance->producer_in), share, 0);
  sem_init(&(instance->consumer_in), share, 0);

  return instance;
}


int see_content(warehouse_t* warehouse) {
  int amount;
  FILE *file = fopen(warehouse->file_path, "r");
  fscanf(file, "%d", &amount);
  fclose(file);
  return amount;
}

void add_amount(warehouse_t* warehouse, int amount) {
  int content = see_content(warehouse);
  content += amount;
  FILE *file = fopen(warehouse->file_path, "w");
  fprintf(file, "%d", content);
  fclose(file);
}

// <================================================>
//                      Producer
// <================================================>

typedef struct{
  int id;
  int a;
  int b;
  char log_path;
} producer_t;

producer_t* construct_producer(int id, int a, int b) {
  producer_t* instance;

  instance->id = id;
  instance->a = a;
  instance->b = b;
  sprintf(instance->log_path, "producer_logs/producer%d", id);

  return instance;
}

void run_producer(producer_t* producer, warehouse_t* warehouse) {
  while (1) {
    int produced = random(producer->a, producer->b);
    int stored = 0;

    while(stored=0) {
      P(&(warehouse->producer_in));
      P(&(warehouse->mutex));

      int content = see_content(warehouse);
      if (warehouse->k - content > produced) {
        add_amount(warehouse, produced);
        stored = 1;
      }
      
      V(&(warehouse->mutex));
      V(&(warehouse->producer_in));
    }
  }
}

// <================================================>
//                      Consumer
// <================================================>

typedef struct{
  int id;
  int c;
  int d;
  char log_path;
} consumer_t;

consumer_t* construct_consumer(int id, int c, int d) {
  consumer_t* instance;

  instance->id = id;
  instance->c = c;
  instance->d = d;
  sprintf(instance->log_path, "consumer_logs/consumer%d", id);

  return instance;
}

void run_consumer(consumer_t* consumer, warehouse_t* warehouse) {
  while (1) {
    int to_be_taken = random(consumer->c, consumer->d);
    int taken = 0;

    while(taken=0) {
      P(&(warehouse->consumer_in));
      P(&(warehouse->mutex));

      int content = see_content(warehouse);
      if (warehouse->k - content < to_be_taken) {
        add_amount(warehouse, to_be_taken);
        taken = 1;
      }
      
      V(&(warehouse->mutex));
      V(&(warehouse->consumer_in));
    }
  }
}

// <================================================>
//                  Main Program
// <================================================>

int main(int argc, char *argv[]) {
        int k = 20;
        int n = 1;
        int a = 1;
        int b = 10;
        int m = 1;
        int c = 1;
        int d = 10;

    if (argc == 9) {
        k = argv[1];
        n = argv[3];
        a = argv[4];
        b = argv[5];
        m = argv[6];
        c = argv[7];
        d = argv[8];
    }
    
    producer_t* producers[n];
    for (int i=0; i<n; i++) {
      producers[i] = construct_producer(i, a, b);
    }

    consumer_t* consumers[m];
    for (int i=0; i<m; i++) {
      producers[i] = construct_consumer(i, c, d);
    }

    return 0;
}