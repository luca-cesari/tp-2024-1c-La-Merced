#include "estado_frame.h"

t_estado_frame *bitmap;
u_int32_t frames_totales;
u_int32_t frames_libres;
pthread_mutex_t frames_mutex;

void inicializar_bitmap_estados()
{
   frames_totales = get_tamanio_memoria() / get_tamanio_pagina();
   frames_libres = frames_totales;
   bitmap = malloc(frames_totales * sizeof(t_estado_frame));
   pthread_mutex_init(&frames_mutex, NULL);
}

u_int32_t get_cantidad_frames()
{
   pthread_mutex_lock(&frames_mutex);
   u_int32_t valor = frames_totales;
   pthread_mutex_unlock(&frames_mutex);
   return valor;
}

void set_estado_frame(u_int32_t frame, t_estado_frame estado)
{
   if (bitmap[frame] == estado)
      return;

   pthread_mutex_lock(&frames_mutex);
   switch (estado)
   {
   case OCUPADO:
      bitmap[frame] = OCUPADO;
      frames_libres--;
      break;
   case LIBRE:
      bitmap[frame] = LIBRE;
      frames_libres++;
      break;
   }
   pthread_mutex_unlock(&frames_mutex);
}

u_int32_t get_frame_libre()
{
   u_int32_t frame = -1;

   pthread_mutex_unlock(&frames_mutex);
   for (u_int32_t i = 0; i < frames_totales; i++)
   {
      if (bitmap[i] != LIBRE)
         continue;

      frame = i;
      break;
   }
   pthread_mutex_unlock(&frames_mutex);

   return frame;
}

u_int32_t get_cantidad_frames_disponibles()
{
   pthread_mutex_lock(&frames_mutex);
   u_int32_t valor = frames_libres;
   pthread_mutex_unlock(&frames_mutex);

   return valor;
}

void destruir_bitmap_estados()
{
   free(bitmap);
   pthread_mutex_destroy(&frames_mutex);
}
