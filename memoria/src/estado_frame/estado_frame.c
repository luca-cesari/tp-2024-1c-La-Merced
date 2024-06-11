#include "estado_frame.h"

t_estado_frame *bitmap;

// es la cantidad de elementos (frames) que tiene el array
// no es la cantidad de bytes que ocupa
u_int32_t frames_totales;
u_int32_t frames_libres;

void inicializar_bitmap_estados()
{
   frames_totales = get_tamanio_memoria() / get_tamanio_pagina();
   frames_libres = frames_totales;
   bitmap = malloc(frames_totales * sizeof(t_estado_frame));
}

u_int32_t get_cantidad_frames()
{
   return frames_totales;
}

void set_estado_frame(u_int32_t frame, t_estado_frame estado)
{
   if (bitmap[frame] == estado)
      return;

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
}

u_int32_t get_frame_libre()
{
   for (u_int32_t i = 0; i < frames_totales; i++)
   {
      if (bitmap[i] == LIBRE)
         return i;
   }
   return -1;
}

u_int32_t get_cantidad_frames_disponibles()
{
   return frames_libres;
}

void destruir_bitmap_estados()
{
   free(bitmap);
}
