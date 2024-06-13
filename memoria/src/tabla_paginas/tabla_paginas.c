#include "tabla_paginas.h"

t_list *lista_tablas;

void inicializar_tabla_paginas()
{
   lista_tablas = list_create();
}

void crear_tabla_de_paginas_para_proceso(u_int32_t pid)
{
   t_proceso_tabla *tabla_paginas = malloc(sizeof(t_proceso_tabla));
   tabla_paginas->pid = pid;
   tabla_paginas->lista_frames = list_create();
   list_add(lista_tablas, tabla_paginas);
   log_creacion_tabla_paginas(pid, 0);
}

void destruir_tabla_de_paginas_para_proceso(u_int32_t pid)
{
   t_proceso_tabla *tabla_paginas = get_tabla_proceso(pid);
   int cont_paginas_liberadas = 0;

   t_list_iterator *iterador = list_iterator_create(tabla_paginas->lista_frames);
   while (list_iterator_has_next(iterador))
   {
      u_int32_t *nro_frame = (u_int32_t *)list_iterator_next(iterador);
      set_estado_frame(*nro_frame, LIBRE);
      cont_paginas_liberadas++;
   }

   list_destroy_and_destroy_elements(tabla_paginas->lista_frames, &free);

   list_iterator_destroy(iterador);
   free(tabla_paginas);

   log_destruccion_tabla_paginas(pid, cont_paginas_liberadas);
}

t_proceso_tabla *get_tabla_proceso(u_int32_t pid)
{
   int es_tabla_buscada(void *elemento)
   {
      t_proceso_tabla *tabla = (t_proceso_tabla *)elemento;
      return tabla->pid == pid;
   };
   return list_find(lista_tablas, (void *)es_tabla_buscada);
}

u_int32_t obtener_marco(u_int32_t pid, u_int32_t nro_pag)
{
   t_proceso_tabla *proceso_tabla = get_tabla_proceso(pid);
   u_int32_t *valor = (u_int32_t *)list_get(proceso_tabla->lista_frames, nro_pag);

   if (valor != NULL)
   {
      log_acceso_tabla_paginas(pid, nro_pag, *valor);
   }

   return valor == NULL ? -1 : *valor;
}

void destruir_tabla_paginas()
{
   list_destroy_and_destroy_elements(lista_tablas, &free);
}
