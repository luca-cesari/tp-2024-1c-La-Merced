#ifndef INTERRUPCION_H
#define INTERRUPCION_H

#include <stdlib.h>
#include <pthread.h>

void inicializar_interrupcion(void);
void set_interrupcion(int32_t);
void reset_interrupcion(void);
int8_t hay_interrupcion(void);
int32_t tipo_interrupcion();

#endif // INTERRUPCION_H