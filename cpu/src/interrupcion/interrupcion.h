#ifndef INTERRUPCION_H
#define INTERRUPCION_H

#include <stdlib.h>
#include <pthread.h>

void inicializar_interrupcion(void);
void set_interrupcion(void);
void reset_interrupcion(void);
int8_t hay_interrupcion(void);

#endif // INTERRUPCION_H