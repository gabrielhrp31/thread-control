#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "util.h"

pthread_mutex_t seat= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;

T_PEOPLE *peoples_queue[MAX_PEOPLES];
int peoples_pos = 0;

//senta no assento do onibus
void sit_down(T_PEOPLE *people)
{
    printf("%s sentou\n", people->name);
    sleep(1);
}

//levanta do assento do onibus
void put_up(T_PEOPLE *people){


    pthread_mutex_lock(&lock);

    //salva o ultimo a sentar
    save_last(*peoples_queue[0]);
    //remove um da fila
    for (int i = 0; i < peoples_pos-1; ++i) {
        peoples_queue[i] = peoples_queue[i+1];
    }

    peoples_pos--;

    printf("%s levantou do assento\n", people->name);

    //ordena a fila
    int next_pos = next(peoples_queue, peoples_pos-1);

    while (next_pos>0){
        T_PEOPLE *aux;
        aux = peoples_queue[next_pos];
        peoples_queue[next_pos] = peoples_queue[next_pos-1];
        peoples_queue[next_pos-1]=aux;
        next_pos--;
    }

    if(peoples_pos>0){
        pthread_cond_signal(&peoples_queue[0]->sig_consumer);
    }

    pthread_mutex_unlock(&lock);
}

//entra na fila
void enter_queue(T_PEOPLE *people){
    peoples_queue[peoples_pos] = people;
    peoples_pos++;
}

//espera para então sentar
void wait(T_PEOPLE *people) {
    printf("%s entrou no onibus\n", people->name);

    pthread_mutex_lock(&lock);

    enter_queue(people);

    if(peoples_pos>1){
        pthread_cond_wait(&peoples_queue[peoples_pos-1]->sig_consumer,&lock);
    }

    pthread_mutex_unlock(&lock);
}

//sai do onibus
void exit_bus(T_PEOPLE *people){
    printf("%s saiu do ônibus\n", people->name);
    sleep(random_num(5));
}

//realiza todo o proceeso da thread função principal da mesma
void *to_sit(void *arg_people) {

    T_PEOPLE *people = (T_PEOPLE *) arg_people;

    while (people->times){
        wait(people);
        pthread_mutex_lock(&seat);
        sit_down(people);
        put_up(people);
        pthread_mutex_unlock(&seat);
        exit_bus(people);
        people->times--;
    }

    return NULL;
}

//cria as threads de pessoas e assentos
int main(int argc, char **argv) {
    pthread_t t_people[MAX_PEOPLES];
    int i,rc;
    srand(time(NULL)); // randomize seed

    int sitting_time = parse_arguments(argc, argv);

    for(i=0;i<MAX_PEOPLES;i++) {
        if ((rc = pthread_create(&t_people[i], NULL,to_sit,(void *)new_people(sitting_time))))
            printf("Error creating the consumer thread..\n");
    }

    for(i=0;i<MAX_PEOPLES;i++) {
        pthread_join(t_people[i], NULL);
    }
}