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

/**
 *Indica quantas vezes as pessoas ainda poderao sentar
 */
int missing_sit_times;

/**
 *Indica a posicao do próximo passageiro ao encontrar deadlock
 */
int deadlock_pos = -1;


/**
* senta no assento do onibus e espera um segundo
* @param T_PEOPLE *people recebe um ponteiro para a pessoa(thread que esta executando a ação)
*/
void sit_down(T_PEOPLE *people)
{
    printf("%s sentou\n", people->name);
    sleep(1);
}


/**
* função usada para verificar a ocorrencia de deadlocks
 * @return retorna 1 se encontra deadlock e 0 se não encontrar
*/
int verify_deadlock(){
    load_queue(peoples_queue,peoples_pos);
    if(((antonio>=0 || antonia>=0) && (times_to_sit(antonio)==last_sit.times || times_to_sit(antonia)==last_sit.times)) &&
       ((bruno>=0 || bruna>=0) && (times_to_sit(bruno)==last_sit.times || times_to_sit(bruna)==last_sit.times))&&
       ((carlos>=0 || carla>=0) && (times_to_sit(carlos)==last_sit.times || times_to_sit(bruna)==last_sit.times)))
        return 1;
    return 0;
}


/**
 * codigo fonte da thread de monitor
 */
void monitor_code(){
    for (;;) {
        sleep(5);

        /**verifica se os passageiros nn passaram pelo onibus todas aas vezes que disseram*/
        if(missing_sit_times==0){
            printf("O cobrador terminou seu serviço!");
            break;
        }

        /**verifica verifica um deadlock e então avisa quem será liberado*/
        if(verify_deadlock()>0){
            unsigned int rand_next = random_num(peoples_pos);
            printf("Cobrador Encontrou um Deadlock, liberando %s\n",peoples_queue[rand_next]->name);
            deadlock_pos =  rand_next;
        }
    }
}


/**
 * levanta do assento
 * @param T_PEOPLE *people recebe um ponteiro para a pessoa(thread que esta executando a ação)
 */
void put_up(T_PEOPLE *people){

    printf("%s levantou do assento\n", people->name);

    pthread_mutex_lock(&lock);

    /**salva o ultimo a sentar*/
    save_last(*peoples_queue[0]);

    /**remove um da fila*/
    for (int i = 0; i < peoples_pos-1; ++i) {
        peoples_queue[i] = peoples_queue[i+1];
    }

    peoples_pos--;

    /**se houver um deadlock espera ate que o mesmo seja resolvido */
    while(verify_deadlock()>0){
        if(deadlock_pos>=0)
            break;
    }

    /**ordena a fila*/
    int next_pos;

    /**se não houve um deadlock então procura o próximo na fila*/
    if(deadlock_pos<0){
        next_pos = next(peoples_queue, peoples_pos-1);
    }
    /**se houve então pega quem o monitor liberou para sentar*/
    else{
        next_pos = deadlock_pos;
        deadlock_pos=-1;
    }

    /**passa o proximo da fila pra primeira posição e manten os outros em suas respectivas*/
    while (next_pos>0){
        T_PEOPLE *aux;
        aux = peoples_queue[next_pos];
        peoples_queue[next_pos] = peoples_queue[next_pos-1];
        peoples_queue[next_pos-1]=aux;
        next_pos--;
    }

    /**se houver alguem no onibus então avisa a primeira pessoa da fila para sentar*/
    if(peoples_pos>0){
        pthread_cond_signal(&peoples_queue[0]->sig_consumer);
    }

    pthread_mutex_unlock(&lock);
}



/**
 * entra na fila
 * @param T_PEOPLE *people recebe um ponteiro para a pessoa(thread que esta executando a ação)
 */
void enter_queue(T_PEOPLE *people){
    peoples_queue[peoples_pos] = people;
    peoples_pos++;
}



/**
 * espera para então sentar
 * @param T_PEOPLE *people recebe um ponteiro para a pessoa(thread que esta executando a ação)
 */
void wait(T_PEOPLE *people) {
    printf("%s entrou no onibus\n", people->name);

    pthread_mutex_lock(&lock);

    enter_queue(people);

    if(peoples_pos>1){
        pthread_cond_wait(&peoples_queue[peoples_pos-1]->sig_consumer,&lock);
    }

    pthread_mutex_unlock(&lock);
}



/**
 * sai do onibus e espera de 3 a 5 segundo
 * @param T_PEOPLE *people recebe um ponteiro para a pessoa(thread que esta executando a ação)
 */
void exit_bus(T_PEOPLE *people){
    printf("%s saiu do ônibus\n", people->name);
    sleep(random_num(5));
}

/**
* realiza todo o proceeso da thread função principal da mesma
* @param T_PEOPLE *people recebe um ponteiro para a pessoa(thread que esta executando a ação)
*/
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
        missing_sit_times--;
    }

    return NULL;
}

/**
*cria as threads de pessoas e do monitor
*/
int main(int argc, char **argv) {
    pthread_t t_monitor;
    pthread_t t_people[MAX_PEOPLES];
    int i,rc;

    srand(time(NULL)); /** escolhe a seed do geradir de aleatorios*/

    int sitting_time = parse_arguments(argc, argv);

    missing_sit_times=sitting_time*MAX_PEOPLES;

    for(i=0;i<MAX_PEOPLES;i++) {
        if ((rc = pthread_create(&t_people[i], NULL,to_sit,(void *)new_people(sitting_time))))
            printf("Error creating the consumer thread..\n");
    }

    pthread_create(&t_monitor, NULL,(void *) monitor_code, NULL);


    for(i=0;i<MAX_PEOPLES;i++) {
        pthread_join(t_people[i], NULL);
    }

    pthread_join(t_monitor, NULL);

    return 0;
}