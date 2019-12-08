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

/**
 * apenas le os argumentos passados e traduz
 * @return o valor do argumento
 */
int parse_arguments(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Arguments invalid\n");
        exit(1);
    }

    return atoi(argv[1]);
}
/**
 * cria  uma nova pessoa
 * @param int times quantidade de vezes a se sentar
 * @return um ponteiro para a nova pessoa
 */
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
    people->sig_consumer = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    people_num++;

    return people;
}

/**
* @return numero aleatorio
*/
unsigned int random_num(unsigned int max) {
    return  rand()%max;
}
/**
* @return a posicao de um nomew passado por parametro -1 se n encontrar
 */
int is_in_queue(char *name){
    for (int j = 0; j < qtd_peoples; ++j) {
        if(peoples_in_queue[j])
            if(strcmp(peoples_in_queue[j]->name,name)==0)
                return j;
    }
    return -1;
}

/**
* @return a primeira posicao com o caracter passado por parametro -1 se n encontrar
*/
int get_first_with(char inicial){
    for (int j = 0; j < qtd_peoples; ++j) {
        if(peoples_in_queue[j])
            if(peoples_in_queue[j]->name[0]==inicial)
                return j;
    }
    return -1;
}

/**
 * vezes ainda restando para sentar
 * @param int pos a posicao de quem se deseja verificar quantas vezes sentou
 * @return int quantas vezes a pessoa da posição sentou
 */
int times_to_sit(int pos){
    if(pos>=0)
        return peoples_in_queue[pos]->times;
    return -1;
}
/**
 * salva a ultima pessoa a sentar
 * @param T_PEOPLE last_people recebe como parametro a ultima pessoa a sentar
 */
void save_last(T_PEOPLE last_people){
    last_sit = last_people;
}

/**
 * @param T_PEOPLE **peoples_queue salva um ponteiro para a fila como auxiliar para algumas funções deste arquivo para se evitar parametros repetitivos
 * @param int people_qtd salva qtd de pessoas na fila para se evitar parametros repetitivos
 */
void load_queue(T_PEOPLE **peoples_queue, int people_qtd){
    peoples_in_queue = peoples_queue;
    qtd_peoples = people_qtd;

    bruno = is_in_queue("Bruno");
    bruna = is_in_queue("Bruna");
    antonio = is_in_queue("Antonio");
    antonia = is_in_queue("Antonia");
    carlos = is_in_queue("Carlos");
    carla = is_in_queue("Carla");
}

/**
 * ordena a fila de pessoas esperando para usar o assento e chama a funcao load_queue par carregar a fila
 * faz as comparações necessárias e retorna a posição do próximo a sentar
 * @param T_PEOPLE **peoples_queue salva um ponteiro para a fila como auxiliar para algumas funções deste arquivo para se evitar parametros repetitivos
 * @param int people_qtd salva qtd de pessoas na fila para se evitar parametros repetitivos
 * @return int posicao do proximo
 */
int next(T_PEOPLE **peoples_queue, int people_qtd) {

    load_queue(peoples_queue,people_qtd);

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