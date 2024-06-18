#ifndef REGISTROS_H
#define REGISTROS_H

#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <pcb/pcb.h>
#include <extended/string.h>

void inicializar_registros(void);
void set_registros(t_pcb *pcb);
void* get_puntero_registro(char *registro);
int32_t get_valor_registro(char *registro);
u_int8_t get_valor_registro_8bit(char *registro);
u_int32_t get_valor_registro_32bit(char *registro);
void set_valor_registro(char *registro, u_int32_t valor);
void set_valor_registro_8bit(char *registro, u_int8_t valor);
void set_valor_registro_32bit(char *registro, u_int32_t valor);
u_int8_t get_tamanio_registro(char *registro);

#endif // REGISTROS_H