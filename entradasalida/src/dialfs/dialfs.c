#include "dialfs.h"

void inicializar_interfaz_dialfs()
{
   inicializar_dicc_instrucciones();
   inicializar_archivo_bloques();
   inicializar_archivo_bitmap();

   while (1)
   {
      t_io_request *peticion_io = esperar_instruccion();

      usleep(get_tiempo_unidad_trabajo() * 1000);

      int8_t (*funcion)(char *, u_int32_t) = get_funcion_instruccion(peticion_io->instruction);
      if (funcion == NULL)
      {
         enviar_respuesta(INVALID_INSTRUCTION);
         destruir_io_request(peticion_io);
         continue;
      }

      int8_t io_result = funcion(peticion_io->arguments, peticion_io->pid);
      if (io_result == -1)
        {
            enviar_respuesta(FAILED);
            destruir_io_request(peticion_io);
            continue;
        }
      // log_operacion(peticion_io->pid, peticion_io->instruction);
      // enviar_respuesta(EXECUTED);
      destruir_io_request(peticion_io);
   }
}
