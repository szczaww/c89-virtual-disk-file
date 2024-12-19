#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


// <================================================>
//      Semaphore adn P&V method implementation
// <================================================>

void P(sem_t* sem){
  if (sem_wait(sem) !=0)
    __THROW "semaphore P failed";
}

void V(sem_t* sem){
  if (sem_post(sem) !=0)
    __THROW "semaphore V failed";
}

int random_number(int x, int y) {
  return x + rand() % (y - x + 1);
}


// <================================================>
//                      Warehouse
// <================================================>

typedef struct{
  int k;
  char file_path[100];
  char log_path[100];
  sem_t mutex;
  sem_t producer_in;
  sem_t consumer_in;
} warehouse_t;

warehouse_t* construct_warehouse(int k, int share) {
  warehouse_t* instance = mmap(
    NULL, sizeof(warehouse_t),
    PROT_READ | PROT_WRITE,
    MAP_SHARED | MAP_ANONYMOUS,
    -1, 0
  );

  instance->k = k;
  strcpy(instance->file_path, "warehouse.txt");
  strcpy(instance->log_path, "warehouse_log.txt");
  sem_init(&(instance->mutex), share, 1);
  sem_init(&(instance->producer_in), share, 1);
  sem_init(&(instance->consumer_in), share, 1);

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
  char log_path[100];
} producer_t;

producer_t* construct_producer(int id, int a, int b) {
  producer_t* instance = malloc(sizeof(producer_t));

  instance->id = id;
  instance->a = a;
  instance->b = b;
  sprintf(instance->log_path, "producer_logs/p%d_log.txt", id);

  return instance;
}

void run_producer(producer_t* producer, warehouse_t* warehouse) {
  srand(getpid());

  while (1) {
    int produced = random_number(producer->a, producer->b);
    FILE *file = fopen(producer->log_path, "a");
    fprintf(file, "Wants to store %d\n", produced);
    fclose(file);
    sleep(1);

    int stored = 0;
    while (stored == 0) {
      P(&(warehouse->producer_in));
      P(&(warehouse->mutex));

      int content = see_content(warehouse);
      if (warehouse->k - content >= produced) {
        add_amount(warehouse, produced);
        stored = 1;

        FILE *file = fopen(producer->log_path, "a");
        fprintf(file, "Successfuly stored %d\n", produced);
        fclose(file);
        file = fopen(warehouse->log_path, "a");
        fprintf(file, "Producer %d stored %d\n", producer->id, produced);
        fclose(file);
      }
      
      V(&(warehouse->mutex));
      V(&(warehouse->producer_in));
    }
  }
}

// <================================================>
//                      Consumer
// <================================================>

typedef struct {
    int id;
    int c;
    int d;
    char log_path[100];
} consumer_t;

consumer_t *construct_consumer(int id, int c, int d) {
    consumer_t *instance = malloc(sizeof(consumer_t));
    instance->id = id;
    instance->c = c;
    instance->d = d;
    sprintf(instance->log_path, "consumer_logs/c%d_log.txt", id);
    return instance;
}

void run_consumer(consumer_t* consumer, warehouse_t* warehouse) {
  srand(getpid());
  while (1) {

    int to_be_taken = random_number(consumer->c, consumer->d);
    FILE *file = fopen(consumer->log_path, "a");
    fprintf(file, "Wants to take %d\n", to_be_taken);
    fclose(file);
    sleep(1);

    int taken = 0;
    while(taken == 0) {
      P(&(warehouse->consumer_in));
      P(&(warehouse->mutex));

      int content = see_content(warehouse);
      if (content >= to_be_taken) {
        add_amount(warehouse, -to_be_taken);
        taken = 1;

        FILE *file = fopen(consumer->log_path, "a");
        fprintf(file, "Wants to take %d\n", to_be_taken);
        fclose(file);
        file = fopen(warehouse->log_path, "a");
        fprintf(file, "Consumer %d took %d\n", consumer->id, to_be_taken);
        fclose(file);
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
        int k = 10;
        int n = 5;
        int a = 1;
        int b = 10;
        int m = 1;
        int c = 1;
        int d = 10;

    if (argc == 9) {
        k = atoi(argv[1]);
        n = atoi(argv[3]);
        a = atoi(argv[4]);
        b = atoi(argv[5]);
        m = atoi(argv[6]);
        c = atoi(argv[7]);
        d = atoi(argv[8]);
    }
    
    warehouse_t* warehouse = construct_warehouse(k, 1);

    for (int i=0; i<n; i++) {
      pid_t pid = fork();
      if (pid == 0) {
        producer_t* producer = construct_producer(i, a, b);
        run_producer(producer, warehouse);
        exit(0);
      }
    }

    for (int i=0; i<m; i++) {
      pid_t pid = fork();
      if (pid == 0) {
        consumer_t* consumer = construct_consumer(i, c, d);
        run_consumer(consumer, warehouse);
        exit(0);
      }
    }

    while (wait(NULL) > 0);

    sem_destroy(&(warehouse->mutex));
    sem_destroy(&(warehouse->producer_in));
    sem_destroy(&(warehouse->consumer_in));
    munmap(warehouse, sizeof(warehouse_t));

    return 0;
}
