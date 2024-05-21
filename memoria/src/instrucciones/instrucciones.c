#include "instrucciones.h"

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
    char *linea = NULL;
    size_t len = 0;
    ssize_t read;
    t_list *instrucciones = list_create();
    while ((read = getline(&linea, &len, archivo)) != -1)
    {
        char *instruccion = strdup(linea);
        list_add(instrucciones, instruccion);
    }
    fclose(archivo);
    free(linea);
    return instrucciones;
}