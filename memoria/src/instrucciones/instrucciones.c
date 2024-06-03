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

    int32_t numero_instruccion = 0; // cambio la inicializacion a 0 porq el en el pcb arranca en 0

    while ((read = getline(&linea, &len, archivo)) != -1)
    {
        t_instruccion *instruccion = malloc(sizeof(t_instruccion));
        instruccion->num_instruccion = numero_instruccion;
        instruccion->instruccion = strdup(linea);
        list_add(instrucciones, instruccion);
        numero_instruccion++;
    }
    fclose(archivo);
    free(linea);
    return instrucciones;
}

void eliminar_proceso(t_pcb *pcb)
{
    // busco el proceso en la lista
    for (int i = 0; i < list_size(lista_procesos); i++)
    {
        t_proceso_instrucciones *proceso = list_get(lista_procesos, i);
        if (proceso->pid == pcb->pid)
        {
            free(proceso->path);
            for (int j = 0; j < list_size(proceso->instrucciones); j++)
            {
                char *instruccion = list_get(proceso->instrucciones, j);
                free(instruccion);
            }
            list_destroy(proceso->instrucciones);
            list_remove(lista_procesos, i);
            free(proceso);
            break;
        }
    }
}

char *proxima_instruccion(t_pcb *pcb)
{
    int es_proceso_buscado(void *elemento)
    {
        return ((t_proceso_instrucciones *)elemento)->pid == pcb->pid;
    };

    t_proceso_instrucciones *proceso = list_find(lista_procesos, (void *)es_proceso_buscado);

    t_instruccion *instrucion = (t_instruccion *)list_get(proceso->instrucciones, pcb->program_counter);

    printf("%s", instrucion->instruccion);

    return instrucion->instruccion;
}
