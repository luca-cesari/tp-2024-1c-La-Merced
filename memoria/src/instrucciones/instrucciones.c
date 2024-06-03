#include "instrucciones.h"

t_list *lista_procesos;

void inicializar_memoria_instrucciones()
{
    /*Inicializo la lista de procesos*/
    lista_procesos = list_create();
}

/*Para cargar un proceso a memoria tengo que agregarlo a la lista de procesos,
esto implica que debo añadir a la lista el pid, path y una lista de instrucciones*/

void cargar_proceso_a_memoria(int32_t pid, char *path)
{
    /*Para obtener la lista de instrucciones primero debo usar el path para
    leer las instrucciones que hay en un archivo de pseudocodigo*/
    t_list *instrucciones = list_create();

    instrucciones = leer_instrucciones(path);
    /*Una vez que tengo la lista de instrucciones, puedo crear un proceso de instrucciones
    y añadirlo a la lista de procesos*/
    t_proceso_instrucciones *proceso = malloc(sizeof(t_proceso_instrucciones));
    proceso->pid = pid;
    proceso->path = strdup(path);
    proceso->instrucciones = instrucciones;
    list_add(lista_procesos, proceso);
}

t_list *leer_instrucciones(char *path)
{
    /*Un ejemplo de las instrucciones que pueden venir en el archivo son las siguientes:
    SET AX 1
    SET BX 1
    SET PC 5
    SUM AX BX
    Simplemente tengo que guardarlas como strings en la lista de instrucciones a medida
    que las voy leyendo. Cada instruccion es una linea nueva. La CPU luego se encargará de interpretar
    estos strings*/
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
    int es_proceso_buscado(void *elemento)
    {
        return ((t_proceso_instrucciones *)elemento)->pid == pcb->pid;
    };
    t_proceso_instrucciones *proceso = list_find(lista_procesos, (void *)es_proceso_buscado);

    char *instrucion = (char *)list_get(proceso->instrucciones, pcb->program_counter);
    printf("%s", instrucion);
    return instrucion;
}
