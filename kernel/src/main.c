#include <stdio.h>
#include <commons/log.h>

#include <sockets/sockets.h>

#include "config/config.h"
#include "consola/consola.h"

const int32_t *grado_multiprogramacion;

void *atender_interfaz_es(void *fd_ptr)
{
    int32_t fd_conexion = *((int32_t *)fd_ptr);
    free(fd_ptr); // Hace falta?

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
    // Temporal para probar nomas,
    // no tiene q estar aca
    iniciar_consola();

    t_config *config = iniciar_config();

    int32_t fd_escucha;

    int32_t fd_dispatch;
    int32_t fd_interrupt;
    int32_t fd_memoria;

    // Conexion con CPU
    struct cpu_config cpu = get_cpu_config(config);

    fd_dispatch = crear_conexion(cpu.ip, cpu.puerto_dispatch);
    int32_t res_dispatch = handshake(fd_dispatch, KERNEL);
    if (res_dispatch == -1) // Hace falta?
    {
        liberar_conexion(fd_dispatch);
        return EXIT_FAILURE;
    }
    enviar_mensaje(fd_dispatch, 10); // mensaje de prueba

    fd_interrupt = crear_conexion(cpu.ip, cpu.puerto_interrupt);
    int32_t res_interrupt = handshake(fd_interrupt, KERNEL);
    if (res_interrupt == -1)
    {
        liberar_conexion(fd_interrupt);
        return EXIT_FAILURE;
    }
    enviar_mensaje(fd_interrupt, 11); // mensaje de prueba

    // Funcion para manejar la conexion con la CPU

    // Conexion con Memoria
    struct mem_config mem = get_memoria_config(config);

    fd_memoria = crear_conexion(mem.ip, mem.puerto);
    int32_t res_memoria = handshake(fd_memoria, KERNEL);
    if (res_memoria == -1)
    {
        liberar_conexion(fd_memoria);
        return EXIT_FAILURE;
    }
    enviar_mensaje(fd_memoria, 12); // mensaje de prueba

    // Escuchar Interfaces
    char *puerto_escucha = get_puerto_escucha(config);
    fd_escucha = iniciar_servidor(puerto_escucha);
    while (1)
    {
        esperar_cliente(fd_escucha, &atender_interfaz_es);
    }

    liberar_conexion(fd_dispatch);
    liberar_conexion(fd_interrupt);
    liberar_conexion(fd_memoria);

    return EXIT_SUCCESS;
}
