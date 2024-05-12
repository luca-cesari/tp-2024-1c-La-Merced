#include "instrucciones.h"

FILE *recibir_pseudocodigo();

void *generar_instruccion(char *instruccion, char **parametros)
{
    if (strcmp(instruccion, SET) == 0)
    {
        return set(parametros[0], parametros[1]);
    }
    else if (strcmp(instruccion, SUM) == 0)
    {
        return sum(parametros[0], parametros[1]);
    }
    else if (strcmp(instruccion, SUB) == 0)
    {
        return sub(parametros[0], parametros[1]);
    }
    else if (strcmp(instruccion, JNZ) == 0)
    {
        return jnz(parametros[0], parametros[1]);
    }
    else if (strcmp(instruccion, IO_GEN_SLEEP) == 0)
    {
        return io_gen_sleep(parametros[0], parametros[1]);
    }
    else
    {
        // En caso de instrucción desconocida
        printf("Instrucción desconocida: %s\n", instruccion);
    }
}

void *procesar_instruccion(const char *linea)
{

    char **tokens;
    char **parametros;
    tokens = string_split(linea, " ");
    char *instruccion = tokens[0];
    for (int i = 1; tokens[i] != NULL; i++)
    {
        string_array_push(parametros, tokens[i]);
    }
    for (int i = 0; tokens[i] != NULL; i++)
    { // no se si hay que hacerlo, capaz puedo ponerlo en el otro if, pero no tendria que sumarle nada a la i
        free(tokens[i]);
    }
    free(tokens);

    return generar_instruccion(instruccion, parametros);
}

void almacenar_instrucciones(void *instruccion, t_dictionary *diccionario, int key)
{ // en el diccionario de las commons, la key esta como char*, no se porque no tira error si la mia es int
    dictionary_put(diccionario, key, instruccion);
}

void obtener_instrucciones()
{
    FILE *pseudocodigo = recibir_pseudocodigo();
    char linea[100];
    t_dictionary instrucciones = *dictionary_create();
    int key = 1; // sera necesario, sino lo es, mejor lista
    while (fgets(linea, sizeof(linea), pseudocodigo) != NULL)
    {
        void *instruccion = procesar_instruccion(linea);
        almacenar_instruccion(instruccion, instrucciones, key); // no hace nada, mas que dictionary_pot, solo  esta por temas de declaratividad, ver
        key++;
    }
}
