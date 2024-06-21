#include "dialfs.h"

/*
Interfaces DialFS
Las interfaces DialFS son las más complejas de este trabajo práctico ya que las mismas interactúan con un sistema de archivos (filesystem) implementado por el grupo. Siempre va a consumir una unidad de TIEMPO_UNIDAD_TRABAJO.
En el caso de todas las peticiones que interactúen con el DialFS, la información a leer y/o escribir va a estar relacionada con una o más direcciones físicas de la memoria, por lo que el Módulo de Interfaz de I/O se va a tener que conectar con la memoria para pedirle o enviarle información.
Las instrucciones que aceptan estas interfaces son:
IO_FS_CREATE
IO_FS_DELETE
IO_FS_TRUNCATE
IO_FS_WRITE
IO_FS_READ
Sistema de archivos DialFS
Lo que busca este FS es ser una implementación simple que represente Asignación Contigua de bloques.
Para simplificar las estructuras que va a tener nuestro FS, vamos a contar con una serie de archivos para simular su funcionamiento.
El primero va a ser nuestro archivo de bloques (bloques.dat) el cual va a ser un archivo de tamaño definido mediante 2 parámetros del archivo de configuración: BLOCK_SIZE y BLOCK_COUNT, y el tamaño del archivo va a ser BLOCK_SIZE * BLOCK_COUNT.
El segundo archivo va a ser un archivo que va a contener un bitmap (bitmap.dat) indicando que bloques se encuentran libres y que bloques se encuentran ocupados dentro de nuestro FS, cualquier implementación que no utilice un bitmap será considerada una implementación equivocada y por consiguiente no se aprobará esa entrega del TP.
El tercer tipo de archivo va a ser un archivo de metadata, del cual vamos a tener varios en nuestro FS y va a ser un archivo cuyo nombre va a ser el nombre del archivo en el FS, por ej notas.txt y su contenido va a tener el bloque en el cual empieza el archivo y el tamaño del archivo en bytes:
BLOQUE_INICIAL=25
TAMANIO_ARCHIVO=1024
Creación de archivos
Al momento de crearse un archivo, va a comenzar ocupando un bloque del FS aunque su tamaño sea 0 y luego el mismo se podrá extender o disminuir por medio de de la función IO_FS_TRUNCATE.
Compactación
Puede darse la situación que al momento de querer ampliar un archivo, dispongamos del espacio disponible pero el mismo no se encuentre contiguo, por lo que vamos a tener que compactar nuestro FS para agrupar los bloques de los archivos de manera tal que quede todo el espacio libre contiguo para el archivo que se desea truncar. Luego de compactar el FS, se deberá esperar un tiempo determinado por el valor de configuración de RETRASO_COMPACTACION para luego continuar con la operación de ampliación del archivo.
Al leer el archivo de configuración solo le van a importar las propiedades de:
TIPO_INTERFAZ
TIEMPO_UNIDAD_TRABAJO
IP_KERNEL
PUERTO_KERNEL
IP_MEMORIA
PUERTO_MEMORIA
PATH_BASE_DIALFS
BLOCK_SIZE
BLOCK_COUNT
RETRASO_COMPACTACION

*/

dialfs_config dialfs_cfg;
t_dictionary *dicc_instrucciones;

void inicializar_interfaz_dialfs()
{
   dialfs_cfg = get_dialfs_config();

   inicializar_dicc_instrucciones(dicc_instrucciones);
   inicializar_archivo_bloques();
   inicializar_archivo_bitmap();
}

void inicializar_archivo_bloques() // tener en cuenta que si la carpeta no se encuentra creada tira excepcion
{
   char *path_bloques = string_from_format("%s/bloques.dat", dialfs_cfg.path_base_dialfs);
   FILE *bloques = fopen(path_bloques, "r");
   if (bloques == NULL)
   {
      bloques = fopen(path_bloques, "w");
      char *bloque = string_repeat('\0', dialfs_cfg.block_size);
      for (int i = 0; i < dialfs_cfg.block_count; i++)
      {
         fwrite(bloque, sizeof(char), dialfs_cfg.block_size, bloques);
      }
      fclose(bloques);
   }
   else
   {
      fclose(bloques);
   }
   free(path_bloques);
}

void inicializar_archivo_bitmap()
{
   char *path_bitmap = string_from_format("%s/bitmap.dat", dialfs_cfg.path_base_dialfs);
   FILE *bitmap = fopen(path_bitmap, "r");
   if (bitmap == NULL)
   {
      bitmap = fopen(path_bitmap, "w");
      char *bitmap_vacio = string_repeat('0', dialfs_cfg.block_count);
      fwrite(bitmap_vacio, sizeof(char), dialfs_cfg.block_count, bitmap);
      fclose(bitmap);
   }
   else
   {
      fclose(bitmap);
   }
   free(path_bitmap);
}