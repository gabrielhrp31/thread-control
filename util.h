//
// Created by winchester on 22/11/2019.
//

#ifndef TRABALHO_THREAD_UTIL_H
#define TRABALHO_THREAD_UTIL_H

#define MAX_PEOPLES 6

typedef struct {
    int id;
    char *name;
    int times;
    int priority;
    pthread_cond_t sig_consumer;//= PTHREAD_COND_INITIALIZER;
}T_PEOPLE;

int parse_arguments(int argc, char **argv);
unsigned int random_num(unsigned int max);
int next(T_PEOPLE **people_queue, int people_pos);
void save_last(T_PEOPLE last_people);
T_PEOPLE *new_people(int times);

#endif //TRABALHO_THREAD_UTIL_H
