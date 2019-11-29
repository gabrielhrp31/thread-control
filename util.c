//
// Created by winchester on 22/11/2019.
//

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

int parse_arguments(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Arguments invalid\n");
        exit(1);
    }

    return atoi(argv[1]);
}

T_PEOPLE *new_people(int times){
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
    people->times=times;
    people->sig_consumer = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    people_num++;

    return people;
}


unsigned int random_num(unsigned int min, unsigned int max) {
    double scaled = (double)rand()/RAND_MAX;
    return (max - min +1) * scaled + min;
}

void ordenate(T_PEOPLE *peoples_queue[MAX_PEOPLES]){
    for (int i = 0; i < MAX_PEOPLES; ++i) {
        printf("%s - ",peoples_queue[i]->name);
        if(strcmp(peoples_queue[i]->name,"Bruno")==0 && strcmp(peoples_queue[i+1]->name,"Antônio")==0){
        }
    }
    printf("\n");
}