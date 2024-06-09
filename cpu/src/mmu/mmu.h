#ifndef CPU_MMU_H
#define CPU_MMU_H

#include <stdio.h>
#include <stdlib.h>

#include <commons/process.h>
#include <commons/collections/list.h>
#include <mem_request/cpu.h>
#include <conexion/memoria.h>
#include "tlb.h"

u_int32_t mmu(u_int32_t direccion_logica_hex);

#endif // CPU_MMU_H