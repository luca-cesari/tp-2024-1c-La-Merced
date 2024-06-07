#include "instrucciones.h"

t_list *lista_procesos;
sem_t hay_proceso_en_lista;

void inicializar_memoria_instrucciones()
{
    /*Inicializo la lista de procesos*/
    lista_procesos = list_create();
    sem_init(&hay_proceso_en_lista, 0, 0);
}

void cargar_proceso_a_memoria(int32_t pid, char *path)
{
    t_list *instrucciones = list_create();

    instrucciones = leer_instrucciones(path);

    t_proceso_instrucciones *proceso = malloc(sizeof(t_proceso_instrucciones));
    proceso->pid = pid;
    proceso->path = strdup(path);
    proceso->instrucciones = instrucciones;

    list_add(lista_procesos, proceso);
    sem_post(&hay_proceso_en_lista);
}

t_list *leer_instrucciones(char *path)
{
    FILE *archivo = fopen(path, "r");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }

    char *linea = NULL;
    size_t len = 0;
    ssize_t read;
    t_list *instrucciones = list_create();

    while ((read = getline(&linea, &len, archivo)) != -1)
    {
        list_add(instrucciones, strdup(linea));
    }

    free(linea);
    fclose(archivo);
    return instrucciones;
}

void eliminar_proceso(t_pcb *pcb)
{
    sem_wait(&hay_proceso_en_lista);
    int es_proceso_buscado(void *elemento)
    {
        return ((t_proceso_instrucciones *)elemento)->pid == pcb->pid;
    };
    t_proceso_instrucciones *proceso = list_find(lista_procesos, (void *)es_proceso_buscado);

    list_remove_element(lista_procesos, proceso);
    free(proceso->path);
    list_destroy_and_destroy_elements(proceso->instrucciones, free);
    free(proceso);
}

char *proxima_instruccion(t_pcb *pcb)
{
    sem_wait(&hay_proceso_en_lista);
    int es_proceso_buscado(void *elemento)
    {
        return ((t_proceso_instrucciones *)elemento)->pid == pcb->pid;
    };
    t_proceso_instrucciones *proceso = list_find(lista_procesos, (void *)es_proceso_buscado);

    char *instrucion = (char *)list_get(proceso->instrucciones, pcb->program_counter);
    sem_post(&hay_proceso_en_lista);
    return instrucion;
}
