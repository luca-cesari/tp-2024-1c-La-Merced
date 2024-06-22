#include "dialfs.h"

t_dictionary *dicc_instrucciones;

void inicializar_interfaz_dialfs()
{
   inicializar_dicc_instrucciones(dicc_instrucciones);
   inicializar_archivo_bloques();
   inicializar_archivo_bitmap();

   while (1)
   {
      t_io_request *peticion_io = esperar_instruccion();

      sleep(get_tiempo_unidad_trabajo() / 1000);

      void (*funcion)(char *, u_int32_t) = dictionary_get(dicc_instrucciones, peticion_io->instruction);
      if (funcion == NULL)
      {
         enviar_respuesta(INVALID_INSTRUCTION);
         continue;
      }

      funcion(peticion_io->arguments, peticion_io->pid);
   }
}
