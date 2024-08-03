#include "mmu.h"

u_int8_t tlb_disponible;

static u_int32_t buscar_en_memoria(u_int32_t pid, u_int32_t numero_pagina);

void inicializar_mmu()
{
   tlb_disponible = inicializar_tlb();
}

u_int32_t get_direccion_fisica(u_int32_t pid, u_int32_t direccion_logica)
{
   u_int32_t tamanio_pagina = get_tamanio_pagina();
   u_int32_t numero_pagina = direccion_logica / tamanio_pagina;
   u_int32_t desplazamiento = direccion_logica - (numero_pagina * tamanio_pagina);
   u_int32_t numero_marco;

   numero_marco = tlb_disponible ? get_marco(pid, numero_pagina) : -1;

   if (numero_marco == -1) // TLB miss, sea porque no hay TLB o porque no se encontró
   {
      log_tlb_miss(pid, numero_pagina);
      numero_marco = buscar_en_memoria(pid, numero_pagina);
      log_obtener_marco(pid, numero_pagina, numero_marco);
      // debug_tlb("previo al reemplazo: \n");
      if (tlb_disponible)
         cargar_nueva_entrada(pid, numero_pagina, numero_marco);

      // debug_tlb("pos posreemplazo: \n");
   }
   else
      log_tlb_hit(pid, numero_pagina);

   return (numero_marco * tamanio_pagina) + desplazamiento;
}

void destruir_mmu()
{
   if (tlb_disponible)
      destruir_tlb();
}

static u_int32_t buscar_en_memoria(u_int32_t pid, u_int32_t numero_pagina)
{
   t_cpu_mem_req *mem_request = crear_nro_frame_request(pid, numero_pagina);
   enviar_mem_request(mem_request);
   destruir_cpu_mem_request(mem_request);
   return recibir_marco();
}