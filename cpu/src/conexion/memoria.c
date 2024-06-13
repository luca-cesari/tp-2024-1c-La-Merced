#include "memoria.h"

int32_t fd_memoria;
u_int32_t tamanio_pagina;

int8_t conectar_con_memoria()
{
   mem_config mem = get_memoria_config();

   fd_memoria = crear_conexion(mem.ip, mem.puerto);
   int32_t response = handshake(fd_memoria, CPU);
   if (response == -1)
   {
      liberar_conexion(fd_memoria);
      return -1;
   }

   tamanio_pagina = recibir_senial(fd_memoria);

   return 0;
}

void enviar_mem_request(t_cpu_mem_req *mem_request)
{
   enviar_cpu_mem_request(fd_memoria, mem_request);
}

char *recibir_instruccion()
{
   char *instruccion = recibir_mensaje(fd_memoria);

   return instruccion;
}

char *recibir_confirmacion()
{
   char *instruccion = recibir_mensaje(fd_memoria);

   return instruccion;
}

u_int32_t recibir_valor_numerico()
{
   return recibir_senial(fd_memoria);
}

void *recibir_paquete_de_memoria()
{
   t_list *paquete = recibir_paquete(fd_memoria);
   return list_get(paquete, 0);
}

u_int32_t recibir_marco()
{
   return recibir_senial(fd_memoria);
}

u_int32_t get_tamanio_pagina()
{
   return tamanio_pagina;
}

void liberar_conexion_memoria()
{
   liberar_conexion(fd_memoria);
}