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



void sit_down(T_PEOPLE *people)
{
    printf("%s sentou\n", people->name);
    sleep(random_num(0,2));
}

void put_up(T_PEOPLE *people){
    pthread_mutex_lock(&lock);

    for (int i = 0; i < peoples_pos-1; ++i) {
        peoples_queue[i] = peoples_queue[i+1];
    }
    peoples_pos--;

    ordenate(peoples_queue);

    printf("%s levantou do assento\n", people->name);

    if(peoples_pos>0){
        pthread_cond_signal(&peoples_queue[0]->sig_consumer);
    }

    pthread_mutex_unlock(&lock);
}

void enter_queue(T_PEOPLE *people){
    peoples_queue[peoples_pos] = people;
    peoples_pos++;
}

void wait(T_PEOPLE *people) {
    printf("%s entrou no onibus\n", people->name);

    pthread_mutex_lock(&lock);

    enter_queue(people);

    if(peoples_pos>1){
        pthread_cond_wait(&peoples_queue[peoples_pos-1]->sig_consumer,&lock);
    }

    pthread_mutex_unlock(&lock);
}

void exit_bus(T_PEOPLE *people){
    printf("%s saiu do ônibus\n", people->name);
    sleep(random_num(0,2));
}

pthread_cond_t sig_producer= PTHREAD_COND_INITIALIZER;

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

int main(int argc, char **argv) {
    pthread_t t_people[MAX_PEOPLES];
    int i,rc;

    int sitting_time = parse_arguments(argc, argv);

    for(i=0;i<MAX_PEOPLES;i++) {
        if ((rc = pthread_create(&t_people[i], NULL,to_sit,(void *)new_people(sitting_time))))
            printf("Error creating the consumer thread..\n");
    }

    for(i=0;i<MAX_PEOPLES;i++) {
        pthread_join(t_people[i], NULL);
    }
}