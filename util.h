//
// Created by winchester on 22/11/2019.
//

#ifndef TRABALHO_THREAD_UTIL_H
#define TRABALHO_THREAD_UTIL_H

#define MAX_PEOPLES 6

typedef struct {
    char *name;
    int times;
    pthread_cond_t sig_consumer;//= PTHREAD_COND_INITIALIZER;
}T_PEOPLE;

T_PEOPLE last_sit;
int bruno, bruna, carlos,carla,antonio,antonia;

int parse_arguments(int argc, char **argv);
unsigned int random_num(unsigned int max);
int times_to_sit(int max);
int next(T_PEOPLE **people_queue, int people_pos);
void load_queue(T_PEOPLE **people_queue, int people_pos);
void save_last(T_PEOPLE last_people);
T_PEOPLE *new_people(int times);

#endif //TRABALHO_THREAD_UTIL_H
