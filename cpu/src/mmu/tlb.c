#include "tlb.h"

struct TablaPaginas *tlb;
int entradas = 0;
u_int32_t cant_entradas_tlb;
int tlb_top = 0;

void inicializar_tlb()
{
   cant_entradas_tlb = get_cant_entradas_tlb();
   tlb = (struct TablaPaginas *)malloc(cant_entradas_tlb * sizeof(struct TablaPaginas));
   // Inicializar la TLB con valores nulos
   for (int i = 0; i < cant_entradas_tlb - 1; i++)
   {
      tlb[i].pid = 0;
      tlb[i].pagina = 0;
      tlb[i].marco = 0;
   }
}

void cargar_matriz_tlb(u_int32_t pid, u_int32_t numero_pagina, u_int32_t numero_marco)
{
   if (entradas < cant_entradas_tlb - 1)
   {
      tlb[entradas].pid = pid;
      tlb[entradas].pagina = numero_pagina;
      tlb[entradas].marco = numero_marco;
      entradas++;
      if (entradas >= cant_entradas_tlb - 1)
      {
         entradas = 0;
      }
   }
}

u_int32_t buscar_en_tlb(u_int32_t pid, u_int32_t numero_pagina)
{
   int inicio_busqueda = tlb_top;
   for (int i = inicio_busqueda; i < cant_entradas_tlb - 1; i++)
   {
      if (tlb[i].pagina == numero_pagina && tlb[i].pid == pid)
      {
         tlb_top = i;
         return tlb[i].marco; // TLB hit
      }
   }
   // Si no se encontró en la parte inicial, buscamos desde 0 hasta inicio_busqueda - 1
   for (int i = 0; i < inicio_busqueda; i++)
   {
      if (tlb[i].pagina == numero_pagina && tlb[i].pid == pid)
      {
         tlb_top = i;
         return tlb[i].marco; // TLB hit
      }
   }
   // TLB miss: devolver -1
   return -1;
}

void destruir_tlb()
{
   free(tlb);
}
