#include "memoria.h"

int32_t fd_memoria;
pthread_mutex_t mutex_memoria;

int8_t conectar_con_memoria()
{
   mem_config mem = get_memoria_config();

   fd_memoria = crear_conexion(mem.ip, mem.puerto);
   int32_t response = handshake(fd_memoria, KERNEL);
   if (response == -1)
   {
      liberar_conexion(fd_memoria);
      return -1;
   }

   pthread_mutex_init(&mutex_memoria, NULL);
   return 0;
}

int8_t reservar_paginas(t_pcb *pcb)
{
   pthread_mutex_lock(&mutex_memoria);
   enviar_pcb(fd_memoria, pcb);
   // t_pcb *header = pcb_recv(fd_memoria);
   pthread_mutex_unlock(&mutex_memoria);

   // if (header->tipo == ERROR)
   // {
   //    printf("Error al reservar paginas\n");
   //    exit(EXIT_FAILURE);
   // }

   return 0;
}

int8_t liberar_memoria(t_pcb *pcb)
{
   pthread_mutex_lock(&mutex_memoria);
   enviar_pcb(fd_memoria, pcb);
   // t_pcb *header = pcb_recv(fd_memoria);
   pthread_mutex_unlock(&mutex_memoria);

   // if (header->tipo == ERROR)
   // {
   //    printf("Error al liberar memoria\n");
   //    exit(EXIT_FAILURE);
   // }

   return 0;
}

void liberar_conexion_memoria()
{
   liberar_conexion(fd_memoria);
}