#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <readline/readline.h>

#include <commons/log.h>
#include <commons/config.h>

#include <sockets/cliente.h>

struct cpu_conn
{
    char *ip;
    char *puerto_dispatch;
    char *puerto_interrupt;
};

// struct mem
// {
//     char *ip;
//     char *puerto;
// };

t_config *iniciar_config(void)
{
    return config_create("kernel.config");
}

struct cpu_conn get_cpu_config(t_config *config)
{
    struct cpu_conn cpu_config;

    cpu_config.ip = config_get_string_value(config, "IP_CPU");
    cpu_config.puerto_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    cpu_config.puerto_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");

    return cpu_config;
}

int main(void)
{
    t_config *config = iniciar_config();
    struct cpu_conn cpu = get_cpu_config(config);

    // Deberían ser variables globales (?
    // y estar en rutinas diferentes (?, o sea hilos diferentes
    int cpu_dispatch = crear_conexion(cpu.ip, cpu.puerto_dispatch);
    int32_t res_cpu_dispatch = handshake(cpu_dispatch, 0);
    if (res_cpu_dispatch == -1)
    {
        liberar_conexion(cpu_dispatch);
        // alguna forma de avisar que falló, o no c
        // return EXIT_FAILURE ??
    }

    int cpu_interrupt = crear_conexion(cpu.ip, cpu.puerto_interrupt);
    int32_t res_cpu_interrupt = handshake(cpu_interrupt, 0);
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
