#include "tlb.h"

entrada_tlb *tlb;
u_int32_t cantidad_entradas;
u_int32_t primera_entrada_libre;
algoritmo_tlb algoritmo;

static entrada_tlb crear_entrada_tlb(u_int32_t pid, u_int32_t pagina, u_int32_t marco);
static u_int32_t index_of(u_int32_t pid, u_int32_t pagina);
static entrada_tlb get_entrada(u_int32_t index);
static entrada_tlb remove_entrada(u_int32_t index);

u_int8_t inicializar_tlb()
{
   cantidad_entradas = get_cant_entradas_tlb();
   if (cantidad_entradas == 0)
      return 0;

   primera_entrada_libre = 0;
   algoritmo = get_algoritmo_tlb();

   tlb = malloc(cantidad_entradas * sizeof(entrada_tlb));
   for (u_int32_t i = 0; i < cantidad_entradas; i++)
      tlb[i] = crear_entrada_tlb(0, 0, 0);

   return 1;
}

void cargar_nueva_entrada(u_int32_t pid, u_int32_t pagina, u_int32_t marco)
{
   if (primera_entrada_libre == cantidad_entradas)
   {
      remove_entrada(0);
      log_reemplazo_de_entrada();
   }

   tlb[primera_entrada_libre] = crear_entrada_tlb(pid, pagina, marco);
   primera_entrada_libre++;
}

int32_t get_marco(u_int32_t pid, u_int32_t numero_pagina)
{
   u_int32_t index = index_of(pid, numero_pagina);
   if (index == -1)
      return -1; // TLB miss

   entrada_tlb entrada;
   switch (algoritmo)
   {
   case FIFO:
      entrada = get_entrada(index);
      break;
   case LRU:
      entrada = remove_entrada(index);
      // se supone que tras un remove_entrada,
      // hay por lo menos una entrada libre
      tlb[primera_entrada_libre] = entrada;
      primera_entrada_libre++;
      break;
   }

   // debug_tlb("hit: \n");
   return entrada.marco; // TLB hit
}

void destruir_tlb()
{
   free(tlb);
}

static entrada_tlb crear_entrada_tlb(u_int32_t pid, u_int32_t pagina, u_int32_t marco)
{
   entrada_tlb entrada;
   entrada.pid = pid;
   entrada.pagina = pagina;
   entrada.marco = marco;
   return entrada;
}

static u_int32_t index_of(u_int32_t pid, u_int32_t pagina)
{
   for (u_int32_t i = 0; i < cantidad_entradas; i++)
   {
      if (tlb[i].pid == pid && tlb[i].pagina == pagina)
         return i;
   }

   return -1;
}

static entrada_tlb get_entrada(u_int32_t index)
{
   return tlb[index];
}

static entrada_tlb remove_entrada(u_int32_t index)
{
   entrada_tlb a_remover = tlb[index];
   int32_t ultimo_indice = cantidad_entradas - 1;

   for (u_int32_t i = index; i < ultimo_indice; i++)
      tlb[i] = tlb[i + 1];

   tlb[ultimo_indice] = crear_entrada_tlb(0, 0, 0);
   primera_entrada_libre--;
   return a_remover;
}

void debug_tlb(char *msj)
{
   printf("%s", msj);
   for (int i = 0; i < cantidad_entradas; i++)
      printf("pid: %d,  pagina: %d \n", tlb[i].pid, tlb[i].pagina);
}