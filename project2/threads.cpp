//Dalton Meny
//CS 470
//Thread Synchronization Project

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

using namespace std;

//Buffer type and size
typedef int buffer_item;
const int BUFFER_SIZE = 5;

//mutex and semaphores
pthread_mutex_t mutex;
pthread_mutex_t print_mutex;
sem_t sem1;
sem_t sem2;

//Global buffer
int buffer[BUFFER_SIZE];

//index counters
int in = 0;
int out = 0;

//Functions to insert and remove buffer items
void  insert_item(buffer_item item, int index);
void  remove_item(buffer_item *item, int index);

//Functions that producers and consumers execute
void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[]) {
  if (argc !=4) {
    cout << "Need to enter sleep timer, # producers, # comsumers | ex: 10 5 3"
	 << endl;
    return 0;
  }

  //Converting input to int
  int sleep_timer = atoi(argv[1]);
  int num_pro = atoi(argv[2]);
  int num_con = atoi(argv[3]);

  //Initializing semaphores and mutexes
  sem_init(&sem1, 0 , BUFFER_SIZE);
  sem_init(&sem2, 0 , 0);
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&print_mutex, NULL);

  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  //Creating producer threads
  for (int i=0; i<num_pro; i++) {
    pthread_create(&tid, &attr, producer, (void *)(long)i);
    //used long to get rid of warning
  }

  //Creating consumer threads
  for (int i=0; i<num_con; i++) {
    pthread_create(&tid, &attr, consumer, (void *)(long)i);
  }
  
  sleep(sleep_timer);

  cout << "Exiting after " << sleep_timer << " seconds" << endl;
  cout << "Goodbye!" << endl;
  
  return 0;
  
}

void insert_item(buffer_item item, int index) {
   pthread_mutex_lock(&print_mutex);
   cout << "Producer " << index << " - Attempting to gain mutex" << endl;
   pthread_mutex_unlock(&print_mutex);
   
  pthread_mutex_lock(&mutex); //Lock for buffer items
  buffer[in] = item; //putting item in buffer
  in = (in+1)%BUFFER_SIZE; //incrementing in
  pthread_mutex_unlock(&mutex);

  pthread_mutex_lock(&print_mutex);
  cout << "Producer " << index << " - Released mutex" << endl;
   pthread_mutex_unlock(&print_mutex);
  
}

void remove_item(buffer_item &item, int index) {
  pthread_mutex_lock(&print_mutex);
  cout << "Consumer " << index << " - Attempting to gain mutex" << endl;
  pthread_mutex_unlock(&print_mutex);
  
  pthread_mutex_lock(&mutex);
  item = buffer[out]; //getting item
  out = (out+1)%BUFFER_SIZE; //incrementing out
  pthread_mutex_unlock(&mutex);

  pthread_mutex_lock(&print_mutex);
  cout << "Consumer " << index << " - Released mutex" << endl;
  pthread_mutex_unlock(&print_mutex);
}

void *producer(void *param) {
  buffer_item item;
  int index = (long)param; //converting index back to int
  
  while(true) {
    pthread_mutex_lock(&print_mutex);
    cout << "Producer " << index << " - Attempting to gain semaphore" << endl;
    pthread_mutex_unlock(&print_mutex);
    
    sem_wait(&sem1); //waits if buffer is full
    
    sleep(rand()%5); //sleeping a rand amount of time under 4s
    item = rand()%10000; //making the number smaller
    
    insert_item(item,index); //producing item
      
    pthread_mutex_lock(&print_mutex);
    cout << "Producer "<< index << " produced " << item << endl;
    pthread_mutex_unlock(&print_mutex);
    
    sem_post(&sem2);

    pthread_mutex_lock(&print_mutex);
    cout << "Producer " << index << " - Released semaphore" << endl;
    pthread_mutex_unlock(&print_mutex);
    
  }
}

void *consumer(void *param) {
  buffer_item item;
  int index = (long)param;
  
  while(true) {
    pthread_mutex_lock(&print_mutex);
    cout << "Consumer " << index << " - Attempting to gain semaphore" << endl;
    pthread_mutex_unlock(&print_mutex);
    
    sem_wait(&sem2); //waits if buffer is empty
    sleep(rand()%5);
    
    remove_item(item, index); //consuming item
   
    pthread_mutex_lock(&print_mutex);
    cout << "Consumer " << (long)param << " consumed " << item << endl;
    pthread_mutex_unlock(&print_mutex);
      
    sem_post(&sem1);

    pthread_mutex_lock(&print_mutex);
    cout << "Consumer " << index << " - Released semaphore" << endl;
    pthread_mutex_unlock(&print_mutex);
  }
}
