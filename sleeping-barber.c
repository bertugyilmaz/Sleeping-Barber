#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define  SEAT_COUNT  5
#define WAITING_SEATS_COUNT 10
#define CLIENT_COUNT 20
#define BARBERS_COUNT 5

pthread_t barberThreads[BARBERS_COUNT];
pthread_t customerThreads[CLIENT_COUNT];

sem_t barber;
sem_t client;
sem_t lock;

void *initialize_semaphores(sem_t *l, sem_t *b, sem_t *c);
void *barbers(void *params);
void *clients(void *params);
void *shave(int position);
void *sleeps(int position);
void *wait(int position);
void openingShop();
void createThreads();

int main()
{
    openingShop();
    
    return 0;
}

void openingShop(){
    printf("Barber is opening ...");
    
    initialize_semaphores(&lock,&barber,&client);
    
    createThreads();
}

void *initialize_semaphores(sem_t *l, sem_t *b, sem_t *c){
    sem_init(l,0,1);
    sem_init(c,0,0);
    sem_init(b,0,BARBERS_COUNT);
}

void createThreads(){
    
    for(int i = 0; i < BARBERS_COUNT; i++){
        pthread_create(&barberThreads[i],NULL,barbers,&i);
    }
    
    for(int j = 0; j < CLIENT_COUNT; j++){
        pthread_create(&customerThreads[j],NULL,clients,&j);
    }
}

void *barbers(void *params){
    int id = (int*)params;
    
    sem_wait(&client);
    
    shave(id);
    sleep(3);
    
    sem_post(&barber);
    sem_post(&lock);
    
}

void *clients(void *params){
    int id = (int *)params;
    
    sem_wait(&lock);
    sem_wait(&barber);
    
    wait(id);
    sleep(3);
    
    sem_post(&client);
    
}

void *wait(int position){
    printf("Customer %d is waiting...", position);
}

void *shave(int position){
    printf("Barber %d is shaving...", position);
}

void *sleeps(int position){
    printf("Barber %d is sleeping...", position);
}
