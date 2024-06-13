#ifndef MMU_H
#define MMU_H

#include <stdio.h>
#include <stdlib.h>
#include <mem_request/cpu.h>

#include "logger/logger.h"
#include "conexion/memoria.h"
#include "tlb.h"

void inicializar_mmu(void);
u_int32_t get_direccion_fisica(u_int32_t pid, u_int32_t direccion_logica);
void destruir_mmu(void);

#endif // MMU_H