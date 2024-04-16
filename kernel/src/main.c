#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <readline/readline.h>

#include <commons/log.h>

#include <sockets/servidor.h>
#include <sockets/cliente.h>

#include "config/config.h"

int main(void)
{
    t_config *config = iniciar_config();
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

    int cpu_interrupt = crear_conexion(cpu.ip, cpu.puerto_interrupt);
    int32_t res_cpu_interrupt = handshake(cpu_interrupt, KERNEL);
    if (res_cpu_interrupt == -1)
    {
        liberar_conexion(cpu_interrupt);
        // lo mismo, habaría que manejarlo
    }

    // Por el momento se liberan los dos sockets asi nomas
    // porque el proceso termina, pero cuando esté el modulo de
    // I/O, se va a quedar escuchando
    liberar_conexion(cpu_dispatch);
    liberar_conexion(cpu_interrupt);

    return EXIT_SUCCESS;
}
