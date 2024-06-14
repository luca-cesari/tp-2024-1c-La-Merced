#include "instrucciones.h"

t_list *lista_procesos;

void inicializar_memoria_instrucciones()
{
    lista_procesos = list_create();
}

void cargar_proceso_a_memoria(int32_t pid, char *path)
{
    t_list *instrucciones = list_create();

    // char *path_completo = path;
    char *path_completo = string_new();
    string_append(&path_completo, get_path_instrucciones());
    string_append(&path_completo, path);

    instrucciones = leer_instrucciones(path_completo);

    t_proceso_instrucciones *proceso = malloc(sizeof(t_proceso_instrucciones));
    proceso->pid = pid;
    proceso->path = strdup(path);
    proceso->instrucciones = instrucciones;
    list_add(lista_procesos, proceso);
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
        char *linea_sin_salto = eliminar_salto_linea(linea);
        list_add(instrucciones, linea_sin_salto);
    }

    free(linea);
    fclose(archivo);
    return instrucciones;
}

void eliminar_proceso(u_int32_t pid)
{
    int es_proceso_buscado(void *elemento)
    {
        t_proceso_instrucciones *proceso_instruccion = (t_proceso_instrucciones *)elemento;
        return proceso_instruccion->pid == pid;
    };
    t_proceso_instrucciones *proceso = list_find(lista_procesos, (void *)es_proceso_buscado);

    list_remove_element(lista_procesos, proceso);
    free(proceso->path);
    list_destroy_and_destroy_elements(proceso->instrucciones, free);
    free(proceso);
}

char *proxima_instruccion(u_int32_t pid, int32_t program_counter)
{
    int es_proceso_buscado(void *elemento)
    {
        return ((t_proceso_instrucciones *)elemento)->pid == pid;
    };
    t_proceso_instrucciones *proceso = list_find(lista_procesos, (void *)es_proceso_buscado);

    char *instrucion = (char *)list_get(proceso->instrucciones, program_counter);
    printf("%s", instrucion);
    return instrucion;
}
