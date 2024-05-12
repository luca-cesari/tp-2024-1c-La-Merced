#include "generica.h"

generica_config config;

void inicializar_interfaz_generica(void)
{
    config = get_generica_config();

    gen_sleep(3);

    conectar_con_kernel();

    liberar_conexion_kernel();

}

void gen_sleep(int32_t cant_unidad_trabajo)
{

    int32_t tiempo_en_ms = config.tiempo_unidad_trabajo;
    usleep(cant_unidad_trabajo * tiempo_en_ms);
    
}