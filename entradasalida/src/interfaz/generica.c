#include "generica.h"


void inicializar_interfaz_generica(void)
{
    t_dictionary* config = get_generica_config();

    // Conectar con Kernel. ESTO HABRÍA QUE ABSTRAERLO EN LA CARPETA CONEXION YA QUE TODOS LOS TIPOS DE INTERFACES SE TIENEN QUE CONECTAR
    kernel_config ker = get_kernel_config();

    int32_t fd_kernel = crear_conexion(ker.ip , ker.puerto);
    handshake(fd_kernel, E_S);
    enviar_mensaje(fd_kernel, 41);

    liberar_conexion(fd_kernel);

}

void gen_sleep(int32_t cant_unidad_trabajo, int32_t tiempo_en_ms)
{

    usleep(cant_unidad_trabajo * tiempo_en_ms);
    
}