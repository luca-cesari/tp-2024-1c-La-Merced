#include "instrucciones.h"

//para mi el pid lo tendria que crear en la funcion, no por parametro


t_list* lista_procesos = list_create();


/*Para cargar un proceso a memoria tengo que agregarlo a la lista de procesos,
esto implica que debo añadir a la lista el pid, path y una lista de instrucciones*/

void cargar_proceso_a_memoria(int32_t pid, char* path)
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

t_list* leer_instrucciones(char* path)
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
    if(archivo == NULL){
        perror("Error al abrir el archivo");
        return NULL;
    }
    char *linea = NULL;
    size_t len = 0;
    ssize_t read;
    t_list *instrucciones = list_create();

    int32_t numero_instruccion = 1; // inicializo el numero en 1

    while ((read = getline(&linea, &len, archivo)) != -1)
    {
        t_instruccion* instruccion = malloc(sizeof(t_instruccion));
        instruccion->num_instruccion = numero_instruccion;
        instruccion->instruccion = strdup(linea);
        list_add(instrucciones, instruccion);
        numero_instruccion++;
    }
    fclose(archivo);
    free(linea);
    return instrucciones;
}


void eliminar_proceso(t_pcb *pcb){
    //busco el proceso en la lista
    for(int i = 0; i < list_size(lista_procesos); i++){
        t_proceso_instrucciones *proceso = list_get(lista_proceso, i);
        if (proceso->pid == pcb->pid){
            free(proceso->path);
            for(int j = 0; j < list_size(proceso->instrucciones); j++){
                char *intruccion = list_get(proceso->instrucciones);
                free(instruccion);
            }
            list_destroy(proceso->instrucciones);
            list_remove(list_procesos, i);
            free(proceso);
            break;
        }
    }
}

t_instruccion* proxima_instruccion(t_pcb *pcb){
    for(int i = 0; i < list_size(lista_procesos); i++){
        //busca el proceso en la lista de procesos por pid
        t_proceso_instrucciones *proceso = list_get(lista_proceso, i);
        if (proceso->pid == pid){

            //busco la instruccion con el pc 
            for(int j = 0; j < list_size(proceso->instrucciones); j++){

                t_instruccion* instruccion = list_get(proceso->instrucciones, j);
                
                if(instruccion->num_instruccion == pcb->program_counter){
                    return instruccion;
                }
            }
        }
    }
}