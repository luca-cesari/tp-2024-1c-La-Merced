#include "io_request.h"

t_io_request *crear_io_request(u_int32_t pid, char *nombre_interfaz, char *instruccion, char *argumentos)
{
   t_io_request *io_request = malloc(sizeof(t_io_request));

   io_request->pid = pid;
   io_request->inteface_name = nombre_interfaz;
   io_request->instruction = instruccion;
   io_request->arguments = argumentos;

   return io_request;
}

t_packet *serializar_io_request(t_io_request *io_request)
{
   t_packet *paquete = crear_paquete();

   agregar_a_paquete(paquete, &io_request->pid, sizeof(u_int32_t));
   agregar_a_paquete(paquete, io_request->inteface_name, strlen(io_request->inteface_name) + 1);
   agregar_a_paquete(paquete, io_request->instruction, strlen(io_request->instruction) + 1);
   agregar_a_paquete(paquete, io_request->arguments, strlen(io_request->arguments) + 1);

   return paquete;
}

void enviar_io_request(int32_t fd_conexion, t_io_request *io_request)
{
   t_packet *paquete = serializar_io_request(io_request);
   enviar_paquete(paquete, fd_conexion);
   eliminar_paquete(paquete);
}

t_io_request *recibir_io_request(int32_t fd_conexion)
{
   t_list *paquete = recibir_paquete(fd_conexion);
   t_io_request *io_request = malloc(sizeof(t_io_request));

   io_request->pid = *(u_int32_t *)list_get(paquete, 0);
   io_request->inteface_name = strdup(list_get(paquete, 1));
   io_request->instruction = strdup(list_get(paquete, 2));
   io_request->arguments = strdup(list_get(paquete, 3));

   list_destroy(paquete);
   return io_request;
}

void destruir_io_request(t_io_request *io_request)
{
   free(io_request->inteface_name);
   free(io_request->instruction);
   free(io_request->arguments);
   free(io_request);
}