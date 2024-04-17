#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <readline/readline.h>

#include <commons/log.h>

#include <sockets/servidor.h>
#include <sockets/cliente.h>

#include "config/config.h"

void *atender_interfaz_es(void *fd_ptr)
{
    int32_t fd_conexion = *((int32_t *)fd_ptr);

    // atender handsake (para saber quienes el cliente)
    uint32_t modulo_cliente = recibir_cliente(fd_conexion);

    if (modulo_cliente != E_S)
    {
        printf("Error de Cliente \n");
        return NULL;
    }

    printf("Interfaz E/S conectada \n");
    recibir_mensaje(fd_conexion);

    return NULL;
}

int main(void)
{
    t_config *config = iniciar_config();

    // Conexion con CPU
    struct CPU cpu = get_cpu_config(config);

    // Deberían ser variables globales (?
    // y estar en rutinas diferentes (?, o sea hilos diferentes
    int32_t cpu_dispatch = crear_conexion(cpu.ip, cpu.puerto_dispatch);
    int32_t res_cpu_dispatch = handshake(cpu_dispatch, KERNEL);
    if (res_cpu_dispatch == -1)
    {
        liberar_conexion(cpu_dispatch);
        // alguna forma de avisar que falló, o no c
        // return EXIT_FAILURE ??
    }

    int32_t cpu_interrupt = crear_conexion(cpu.ip, cpu.puerto_interrupt);
    int32_t res_cpu_interrupt = handshake(cpu_interrupt, KERNEL);
    if (res_cpu_interrupt == -1)
    {
        liberar_conexion(cpu_interrupt);
        // lo mismo, habaría que manejarlo
    }

    // Conexion con Memoria
    struct MEM mem = get_memoria_config(config);

    int32_t mem_peticion = crear_conexion(mem.ip, mem.puerto);
    int32_t res_mem_peticion = handshake(mem_peticion, KERNEL);
    if (res_mem_peticion == -1)
    {
        liberar_conexion(mem_peticion);
        // lo mismo, habaría que manejarlo
    }

    // Escuchar Interfaces
    char *puerto_escucha = get_puerto_escucha(config);
    int32_t fd_escucha = iniciar_servidor(puerto_escucha);

    while (1)
    {
        esperar_cliente(fd_escucha, &atender_interfaz_es);
    }

    liberar_conexion(cpu_dispatch);
    liberar_conexion(cpu_interrupt);
    liberar_conexion(mem_peticion);

    return EXIT_SUCCESS;
}
