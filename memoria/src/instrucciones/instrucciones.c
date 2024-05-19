/*#include "instrucciones.h"

FILE *recibir_pseudocodigo();

int contarLineas(FILE *archivo)
{
    int contador = 0;
    char caracter;

    while ((caracter = fgetc(archivo)) != EOF)
    {
        if (caracter == '\n')
        { // si el caracter es un salto de línea, incrementa el contador
            contador++;
        }
    }

    // si el archivo no termina con un salto de línea, se cuenta una última línea
    if (contador != 0)
    {
        contador++;
    }

    return contador;
}

char *obtener_array_instrucciones()
{
    FILE *pseudocodigo = recibir_pseudocodigo();
    char linea[100];
    int num_instrucciones = contarLineas(pseudocodigo);
    // t_list *instrucciones = list_create();
    char *instrucciones[num_instrucciones];
    int i = 0;
    while (fgets(linea, sizeof(linea), pseudocodigo) != NULL) // lee cada linea del archivo y lo guarda en el array
    {
        instrucciones[i] = linea;
        i++;
    }
    return instrucciones[];
}

char *obtener_instruccion(int PC)
{   // vamos a hacer que el pc tenga los accesos a memoria en HEXA(pregunta), ver
    // cada vez que quiere una instruccion tiene que obtener todo el array, no esta bueno, pero hasta que no veamos memoria no se puede hacer mucho
    char *instrucciones[] = obtener_array_instrucciones();
    return instrucciones[PC];
}
*/