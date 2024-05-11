#include "generica.h"


void inicializar_interfaz_generica(void)
{
    t_dictionary* config = get_generica_config();

    conectar_con_kernel();

    liberar_conexion_kernel();

}

void gen_sleep(int32_t cant_unidad_trabajo, int32_t tiempo_en_ms)
{

    usleep(cant_unidad_trabajo * tiempo_en_ms);
    
}