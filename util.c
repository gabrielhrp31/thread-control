//
// Created by winchester on 22/11/2019.
//

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

//apenas le os argumentos passados e traduz
int parse_arguments(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Arguments invalid\n");
        exit(1);
    }

    return atoi(argv[1]);
}

//cria e retorna uma nova pessoa
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

//gera um numero aleatorio que sera o tempo da pessoa sentada no banco
unsigned int random_num(unsigned int min, unsigned int max) {
    double scaled = (double)rand()/RAND_MAX;
    return (max - min +1) * scaled + min;
}

//ordena a fila de pessoas esperando para usar o assento
void ordenate(T_PEOPLE *peoples_queue[MAX_PEOPLES], int people_pos){
    for (int i = 0; i < people_pos-1; ++i) {
        if((strcmp(peoples_queue[i]->name,"Bruno")==0 || strcmp(peoples_queue[i]->name,"Bruna")==0 ) &&
            (strcmp(peoples_queue[i+1]->name,"Antônio")==0 || strcmp(peoples_queue[i+1]->name,"Antônia")==0))
        {
            T_PEOPLE *aux = peoples_queue[i];
            peoples_queue[i]=peoples_queue[i+1];
            peoples_queue[i+1]=aux;
        }else
        if((strcmp(peoples_queue[i]->name,"Carlos")==0 || strcmp(peoples_queue[i]->name,"Carla")==0 ) &&
           (strcmp(peoples_queue[i+1]->name,"Bruno")==0 || strcmp(peoples_queue[i+1]->name,"Bruna")==0))
        {
            T_PEOPLE *aux = peoples_queue[i];
            peoples_queue[i]=peoples_queue[i+1];
            peoples_queue[i+1]=aux;
        }else
        if((strcmp(peoples_queue[i]->name,"Antônio")==0 || strcmp(peoples_queue[i]->name,"Antônia")==0 ) &&
           (strcmp(peoples_queue[i+1]->name,"Carlos")==0 || strcmp(peoples_queue[i+1]->name,"Carla")==0))
        {
            T_PEOPLE *aux = peoples_queue[i];
            peoples_queue[i]=peoples_queue[i+1];
            peoples_queue[i+1]=aux;
        }
    }
}