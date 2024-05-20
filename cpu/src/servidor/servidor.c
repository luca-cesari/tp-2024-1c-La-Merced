#include "servidor.h"

void iniciar_servidor(){

    char* puerto_escucha = get_puerto_escucha();

    int32_t* fd_dispatch = malloc(sizeof(int32_t));
    *fd_dispatch = crear_servidor(puerto_escucha);

    int32_t* fd_interrupt = malloc(sizeof(int32_t));
    *fd_interrupt = crear_servidor(puerto_escucha);

    pthread_t hilo_dispatch, hilo_interrupt;
    pthread_create(&hilo_dispatch, NULL, &escuchar_conexiones, fd_dispatch);
    pthread_create(&hilo_interrupt, NULL, &escuchar_conexiones, fd_interrupt);
    pthread_detach(hilo_dispatch);
    pthread_detach(hilo_interrupt);
}

