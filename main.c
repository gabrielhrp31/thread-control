#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "util.h"

#define MAX_PEOPLES 6

typedef struct {
    char *name;
    pthread_cond_t sig_consumer;//= PTHREAD_COND_INITIALIZER;
}T_PEOPLE;

pthread_mutex_t seat= PTHREAD_MUTEX_INITIALIZER;

T_PEOPLE *new_people(int time){
    static int people_num=0;

    char *name[] = {
            "Antônio",
            "Antônia",
            "Bruno",
            "Bruna",
            "Carlos",
            "Carla"
    };

    T_PEOPLE *people =  (T_PEOPLE *)malloc(sizeof(T_PEOPLE));

    if (people_num >= MAX_PEOPLES) {
        people = 0;
    }

    people->name=name[people_num];
    people->sig_consumer = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    people_num++;

    return people;
}

void *to_sit(void *arg_people)
{

    T_PEOPLE *people = (T_PEOPLE *)arg_people;

    printf("%s entrou no onibus\n", people->name);

    pthread_mutex_lock(&seat);
    /* Signal the producer that the consumer is ready. */
    pthread_cond_signal(&people->sig_consumer);
    /* Wait for a new number. */
    pthread_cond_wait(&people->sig_consumer, &seat);

    /* Consume (print) the number. */
    printf("%s sentou\n", people->name);
    /* Unlock the mutex. */
    pthread_mutex_unlock(&seat);

    /*
      If the MAX number was the last consumed number, the consumer should
      stop.
    */
}


//pthread_cond_t sig_producer= PTHREAD_COND_INITIALIZER;
//
//void *sit_down(void *dummy)
//{
//    printf("Producer : \"Hello I am producer #%ld. Ready to produce numbers"
//           " now\"\n", pthread_self());
//
//    while (1)
//    {
//        pthread_mutex_lock(&seat);
//        number ++;
//        printf("Producer : %d\n", number);
//        /*
//          Signal the consumer that a new number has been generated for its
//          consumption.
//        */
//        pthread_cond_signal(&sig_consumer);
//        /*
//          Now wait for consumer to confirm. Note, expect no confirmation for
//          consumption of MAX from consumer.
//        */
//        if (number != MAX)
//            pthread_cond_wait(&sig_producer, &mu);
//
//        /* Unlock the mutex. */
//        pthread_mutex_unlock(&seat);
//
//        /* Stop if MAX has been produced. */
//        if (number == MAX)
//        {
//            printf("Producer done.. !!\n");
//            break;
//        }
//    }
//}

int main(int argc, char **argv) {
    pthread_t t_people[MAX_PEOPLES];
    int peoples_queue[MAX_PEOPLES];
    int thread_args[MAX_PEOPLES];
    int i,rc;

    int sitting_time = parse_arguments(argc, argv);

    for(i=0;i<MAX_PEOPLES;i++) {
        if ((rc = pthread_create(&t_people[0], NULL,to_sit,(void *)new_people(sitting_time))))
            printf("Error creating the consumer thread..\n");
    }

    for(i=0;i<MAX_PEOPLES;i++) {
        pthread_join(t_people[i], NULL);
    }
}

