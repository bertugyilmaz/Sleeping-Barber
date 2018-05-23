#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define SEAT_COUNT  5
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

int freeWaitingSeatCount = WAITING_SEATS_COUNT;
int freeBarberSeatCount = BARBERS_COUNT;
int clientCount = CLIENT_COUNT;

int main()
{
    openingShop();
    
    return 0;
}

void openingShop(){
    printf("Barber is opening ...\n");
    
    initialize_semaphores(&lock,&barber,&client);
    
    createThreads();
}

void *initialize_semaphores(sem_t *l, sem_t *b, sem_t *c){
    sem_init(l,0,1);
    sem_init(c,0,CLIENT_COUNT);
    sem_init(b,0,BARBERS_COUNT);
}

void createThreads(){
    
    for(int i = 0; i < BARBERS_COUNT; i++){
        pthread_create(&barberThreads[i],NULL,barbers,i);
    }
    
    for(int j = 0; j < CLIENT_COUNT; j++){
        pthread_create(&customerThreads[j],NULL,clients,j);
    }
    for(int j = 0; j < CLIENT_COUNT; j++){
        pthread_join(customerThreads[j],NULL);
    }
    for(int i = 0; i < BARBERS_COUNT; i++){
        pthread_join(barberThreads[i],NULL);
    }
}

void *barbers(void *params){
    int id = (int*)params;
    do{
        clientCount = clientCount - 1;
        if(clientCount == 0 || clientCount < 0){
            sleeps(id);
            break;
        }
        
        sem_wait(&client);
        shave(id);
        printf("%d adet müşteri kalmıştır.\n",clientCount);
        sleep(3);
        sem_post(&barber);
        
    }while(1);
}

void *clients(void *params){
    int id = (int *)params;
    sem_wait(&lock);
    if(freeWaitingSeatCount==0){
        printf("%d numaralı musteri ayriliyor ... Bekleme salonu Dolu\n",id);
        pthread_exit(NULL);
    }
    freeWaitingSeatCount = freeWaitingSeatCount - 1;
    printf("free waiting seat %d",freeWaitingSeatCount);
    sem_post(&lock);
    wait(id);
    sleep(3);
    sem_wait(&barber);
    sem_post(&client);
}

void *wait(int position){
    printf("Customer %d is waiting...\n", position);
}

void *shave(int position){
    printf("Barber %d is shaving...\n", position);
}

void *sleeps(int position){
    printf("Barber %d is sleeping...\n", position);
}
