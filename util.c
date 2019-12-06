//
// Created by winchester on 22/11/2019.
//

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

T_PEOPLE **peoples_in_queue;
int qtd_peoples;
T_PEOPLE last_sit;

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
            "Antonio",
            "Antonia",
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
    people->priority=0;
    people->sig_consumer = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    people_num++;

    return people;
}

//gera um numero aleatorio que sera o tempo da pessoa sentada no banco
unsigned int random_num(unsigned int max) {
    return  rand()%max;
}

//verifica se a pessoa está na fila
int is_in_queue(char *nome){
    for (int j = 0; j < qtd_peoples; ++j) {
        if(peoples_in_queue[j])
            if(strcmp(peoples_in_queue[j]->name,nome)==0)
                return j;
    }
    return -1;
}

//verifica a primeira posicao com incial passada por parametro
int get_first_with(char inicial){
    for (int j = 0; j < qtd_peoples; ++j) {
        if(peoples_in_queue[j])
            if(peoples_in_queue[j]->name[0]==inicial)
                return j;
    }
    return -1;
}

//vezes ainda restando para sentar
int times_to_sit(int pos){
    if(pos>=0)
        return peoples_in_queue[pos]->times;
    return -1;
}

void save_last(T_PEOPLE last_people){
    last_sit = last_people;
}

//ordena a fila de pessoas esperando para usar o assento
int next(T_PEOPLE **peoples_queue, int people_qtd) {
    peoples_in_queue = peoples_queue;
    qtd_peoples = people_qtd;

    int bruno = is_in_queue("Bruno");
    int bruna = is_in_queue("Bruna");
    int antonio = is_in_queue("Antonio");
    int antonia = is_in_queue("Antonia");
    int carlos = is_in_queue("Carlos");
    int carla = is_in_queue("Carla");

    if(((antonio>=0 || antonia>=0) && (times_to_sit(antonio)==last_sit.times || times_to_sit(antonia)==last_sit.times)) &&
        ((bruno>=0 || bruna>=0) && (times_to_sit(bruno)==last_sit.times || times_to_sit(bruna)==last_sit.times))&&
        ((carlos>=0 || carla>=0) && (times_to_sit(carlos)==last_sit.times || times_to_sit(bruna)==last_sit.times))
    ){
        unsigned int rand_next = random_num(people_qtd);
        printf("Cobrador Encontrou um Deadlock\n");

        printf("Cobrador Mandou %s Sentar\n",peoples_queue[rand_next]->name);
        return rand_next;

    }else{
        //se o bruno sentou anteriormente então seria necessário vere=ificar a necessidade de passar alguem na frente da bruna
        if(strcmp(last_sit.name,"Bruno")==0){
            if(bruna>=0){
                //verifica se o bruno sentou e a proxima seria a bruna
                //e passa alguem com menor prioridade entre os dois
                if(times_to_sit(bruna)==last_sit.times) {
                    if((antonio>=0 || antonia>=0) && (times_to_sit(antonio)==last_sit.times || times_to_sit(antonia)==last_sit.times)){
                        return get_first_with('A');
                    }else{
                        return  bruna;
                    }
                }else{
                    if(carlos>=0 || carla>=0){
                        return get_first_with('C');
                    }else{
                        return get_first_with('A');

                    }
                }
            }else{
                if(carlos>=0 || carla>=0){

                    return get_first_with('C');
                }else{
                    return get_first_with('A');

                }
            }
        }

        if(strcmp(last_sit.name,"Bruna")==0){
            if(bruno>=0){
                //verifica se a bruna vai sentar seguida do bruno
                if(times_to_sit(bruno)==last_sit.times) {
                    if((antonio>=0 || antonia>=0) && (times_to_sit(antonio)==last_sit.times || times_to_sit(antonia)==last_sit.times)){
                        return get_first_with('A');
                    }else{
                        return bruno;
                    }
                }else{
                    if(carlos>=0 || carla>=0){
                        return get_first_with('C');
                    }else{
                        return get_first_with('A');

                    }
                }
            }else{
                if(carlos>=0 || carla>=0){

                    return get_first_with('C');
                }else{
                    return get_first_with('A');

                }
            }
        }

        if(strcmp(last_sit.name,"Antonio")==0){
            if(antonia>=0){
                if(times_to_sit(antonia)==last_sit.times) {
                    if((carlos>=0 || carla>=0) && (times_to_sit(carlos)==last_sit.times || times_to_sit(carla)==last_sit.times)){
                        return get_first_with('C');
                    }else{
                        return antonia;
                    }
                }else{
                    if(bruno>=0 || bruna>=0){
                        return get_first_with('B');
                    }else{
                        return get_first_with('C');

                    }
                }
            }else{
                if(bruno>=0 || bruna>=0){
                    return get_first_with('B');
                }else{
                    return get_first_with('C');

                }
            }
        }

        if(strcmp(last_sit.name,"Antonia")==0){
            if(antonio>=0){
                if(times_to_sit(antonio)==last_sit.times) {
                    if((carlos>=0 || carla>=0) && (times_to_sit(carlos)==last_sit.times || times_to_sit(carla)==last_sit.times)){
                        return get_first_with('C');
                    }else{
                        return antonio;
                    }
                }else{
                    if(bruno>=0 || bruna>=0){
                        return get_first_with('B');
                    }else{
                        return get_first_with('C');

                    }
                }
            }else{
                if(bruno>=0 || bruna>=0){
                    return get_first_with('B');
                }else{
                    return get_first_with('C');

                }
            }
        }

        if(strcmp(last_sit.name,"Carlos")==0){
            if(carla>=0){
                if(times_to_sit(carla)==last_sit.times) {
                    if((bruno>=0 || bruna>=0) && (times_to_sit(bruno)==last_sit.times || times_to_sit(bruna)==last_sit.times)){
                        return get_first_with('B');
                    }else{
                        return carla;
                    }
                }else{
                    if(antonia>=0 || antonio>=0){
                        return get_first_with('A');
                    }else{
                        return get_first_with('B');

                    }
                }
            }else{
                if(bruno>=0 || bruna>=0){
                    return get_first_with('A');
                }else{
                    return get_first_with('B');

                }
            }
        }

        if(strcmp(last_sit.name,"Carla")==0){
            if(carlos>=0){
                if(times_to_sit(carlos)==last_sit.times) {
                    if((bruno>=0 || bruna>=0) && (times_to_sit(bruno)==last_sit.times || times_to_sit(bruna)==last_sit.times)){
                        return get_first_with('B');
                    }else{
                        return carla;
                    }
                }else{
                    if(antonia>=0 || antonio>=0){
                        return get_first_with('A');
                    }else{
                        return get_first_with('B');

                    }
                }
            }else{
                if(bruno>=0 || bruna>=0){
                    return get_first_with('A');
                }else{
                    return get_first_with('B');

                }
            }
        }
    }
}