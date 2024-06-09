#ifndef TLB_H
#define TLB_H

#include <stdlib.h>
#include <config/config.h>

struct TablaPaginas
{
   int pid;
   int pagina;
   int marco;
};
void inicializar_tlb();
void cargar_matriz_tlb(u_int32_t pid, u_int32_t numero_pagina, u_int32_t numero_marco);
u_int32_t buscar_en_tlb(u_int32_t pid, u_int32_t numero_pagina);
void destruir_tlb();

#endif // TLB_H