#include "instrucciones.h"

t_list *lista_procesos;

void inicializar_memoria_instrucciones()
{
    lista_procesos = list_create();
}

void cargar_proceso_a_memoria(int32_t pid, char *path)
{
    char *path_completo = string_new();
    char *path_base = get_path_instrucciones();
    string_append(&path_completo, path_base);
    string_append(&path_completo, path);

    t_list *instrucciones = leer_instrucciones(path_completo);
    // free(path_base);
    free(path_completo);

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
        list_add(instrucciones, eliminar_salto_linea(linea));
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
    list_remove_and_destroy_all_by_condition(lista_procesos, (void *)es_proceso_buscado, &destruir_proceso_instrucciones);
}

void destruir_proceso_instrucciones(void *elemento)
{
    t_proceso_instrucciones *proceso = (t_proceso_instrucciones *)elemento;
    free(proceso->path);
    list_destroy_and_destroy_elements(proceso->instrucciones, &free);
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
    printf("%s\n", instrucion);
    return instrucion;
}
