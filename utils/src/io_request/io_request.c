#include "io_request.h"

t_io_request *crear_io_request(u_int32_t pid, char *nombre_interfaz, char *instruccion, char *argumentos)
{
   t_io_request *io_request = malloc(sizeof(t_io_request));

   io_request->pid = pid;
   io_request->inteface_name = strdup(nombre_interfaz);
   io_request->instruction = strdup(instruccion);
   io_request->arguments = strdup(argumentos);

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

void vaciar_io_request(t_io_request *io_request)
{
   free(io_request->inteface_name);
   free(io_request->instruction);
   free(io_request->arguments);

   io_request->inteface_name = strdup("");
   io_request->instruction = strdup("");
   io_request->arguments = strdup("");
}

void print_io_request(t_io_request *io_request)
{
   printf("[+] IO Request:\n");
   printf("    - PID: %d\n", io_request->pid);
   printf("    - Interface Name: %s\n", io_request->inteface_name);
   printf("    - Instruction: %s\n", io_request->instruction);
   printf("    - Arguments: %s\n", io_request->arguments);
}

void destruir_io_request(t_io_request *io_request)
{
   free(io_request->inteface_name);
   free(io_request->instruction);
   free(io_request->arguments);
   free(io_request);
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

void enviar_io_response(int32_t fd_conexion, t_io_response response)
{
   enviar_senial(response, fd_conexion);
}

t_io_response recibir_io_response(int32_t fd_conexion)
{
   return recibir_senial(fd_conexion);
}