#include "gestor_planificacion.h"

t_gestor_planificacion *semaforos_planificador;
estado_planificador estado;

void inicializar_gestor_planificacion()
{
   semaforos_planificador = malloc(sizeof(t_gestor_planificacion));

   semaforos_planificador->sem_ejecucion_proceso = malloc(sizeof(sem_t));
   sem_init(semaforos_planificador->sem_ejecucion_proceso, 0, 0);

   semaforos_planificador->sem_creacion_proceso = malloc(sizeof(sem_t));
   sem_init(semaforos_planificador->sem_creacion_proceso, 0, 0);

   semaforos_planificador->sem_manejo_desalojo = malloc(sizeof(sem_t));
   sem_init(semaforos_planificador->sem_manejo_desalojo, 0, 0);

   semaforos_planificador->sem_entrada_ready = malloc(sizeof(sem_t));
   sem_init(semaforos_planificador->sem_entrada_ready, 0, 0);

   estado = DETENIDO;
}

void destruir_gestor_planificacion()
{
   sem_destroy(semaforos_planificador->sem_ejecucion_proceso);
   free(semaforos_planificador->sem_ejecucion_proceso);

   sem_destroy(semaforos_planificador->sem_creacion_proceso);
   free(semaforos_planificador->sem_creacion_proceso);

   sem_destroy(semaforos_planificador->sem_manejo_desalojo);
   free(semaforos_planificador->sem_manejo_desalojo);

   sem_destroy(semaforos_planificador->sem_entrada_ready);
   free(semaforos_planificador->sem_entrada_ready);

   free(semaforos_planificador);
}

void habilitar_planificador()
{
   if (estado == EJECUTANDO)
      return;

   // El orden de los sem_post es importante
   sem_post(semaforos_planificador->sem_entrada_ready);
   sem_post(semaforos_planificador->sem_manejo_desalojo);
   sem_post(semaforos_planificador->sem_ejecucion_proceso);
   sem_post(semaforos_planificador->sem_creacion_proceso);

   estado = EJECUTANDO;
}

void deshabilitar_planificador()
{
   if (estado == DETENIDO)
      return;

   // El orden de los sem_wait es importante
   sem_wait(semaforos_planificador->sem_creacion_proceso);
   sem_wait(semaforos_planificador->sem_ejecucion_proceso);
   sem_wait(semaforos_planificador->sem_manejo_desalojo);
   sem_wait(semaforos_planificador->sem_entrada_ready);

   estado = DETENIDO;
}

void puede_crear_proceso()
{
   sem_wait(semaforos_planificador->sem_creacion_proceso);
   sem_post(semaforos_planificador->sem_creacion_proceso);
}

void puede_ejecutar_proceso()
{
   sem_wait(semaforos_planificador->sem_ejecucion_proceso);
   sem_post(semaforos_planificador->sem_ejecucion_proceso);
}

void puede_manejar_desalojo()
{
   sem_wait(semaforos_planificador->sem_manejo_desalojo);
   sem_post(semaforos_planificador->sem_manejo_desalojo);
}

void puede_entrar_a_ready()
{
   sem_wait(semaforos_planificador->sem_entrada_ready);
   sem_post(semaforos_planificador->sem_entrada_ready);
}
