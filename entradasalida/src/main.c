#include "config/config.h"
#include "logger/logger.h"

#include "conexion/kernel.h"
#include "conexion/memoria.h"

#include "interfaz/generica.h"
#include "interfaz/stdin.h"
#include "interfaz/stdout.h"
#include "interfaz/dialfs/dialfs.h"

int main(int argc, char **argv)
{
    char *nombre_interfaz = argv[1];
    printf("Nombre de la interfaz: %s\n", nombre_interfaz);
    char *ruta_config = argv[2];
    printf("Ruta del archivo de configuracion: %s\n", ruta_config);

    iniciar_config(ruta_config);
    iniciar_logger(nombre_interfaz);

    conectar_con_kernel(nombre_interfaz);
    conectar_con_memoria();

    switch (get_tipo_interfaz())
    {
    case GENERICA:
        iniciar_rutina_interfaz_generica();
        break;
    case STDIN:
        iniciar_rutina_interfaz_stdin();
        break;
    case STDOUT:
        inicializar_interfaz_stdout();
        break;
    case DIALFS:
        inicializar_interfaz_dialfs();
        break;
    default:
        destruir_config();
        return EXIT_FAILURE;
    }

    liberar_conexion_kernel();
    liberar_conexion_memoria();

    destruir_config();
    return EXIT_SUCCESS;
}
