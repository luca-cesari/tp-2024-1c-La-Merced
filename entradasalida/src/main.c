#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <sockets/sockets.h>
#include "config/config.h"
#include "interfaz/generica.h"
#include "interfaz/stdin.h"
#include "interfaz/stdout.h"
#include "interfaz/dialfs.h"

int main(int argc, char **argv)
{
    // char *nombre_interfaz = argv[1];
    char *ruta_config = argv[2];

    t_config *config = config_create(ruta_config);
    iniciar_config(); //Por ahora uso el config predeterminado, en un futuro se usara el que viene por parámetro en el main


    switch (get_tipo_interfaz())
    {
    case GENERICA:
        inicializar_interfaz_generica();
    case STDIN:
        inicializar_interfaz_stdin();
    case STDOUT:
        inicializar_interfaz_stdout();
    case DIALFS:
        inicializar_interfaz_dialfs();
    default:
        break;
    }

    
    /*
    // Conectar con Kernel
    kernel_config ker = get_kernel_config();

    int32_t fd_kernel = crear_conexion(ker.ip , ker.puerto);
    handshake(fd_kernel, E_S);
    enviar_mensaje(fd_kernel, 41);

    // Conectar con Memoria
    mem_config mem = get_memoria_config();

    int32_t fd_memoria = crear_conexion(mem.ip, mem.puerto);
    handshake(fd_memoria, E_S);
    enviar_mensaje(fd_memoria, 42);
    
    liberar_conexion(fd_kernel);
    liberar_conexion(fd_memoria);
    */
    return 0;
}
