#include "mmu.h"

int tlb_top = 0;

u_int32_t mmu(u_int32_t direccion_logica)
{
   u_int32_t pid = process_getpid();

   u_int32_t tamanio_pagina = 32; // lo tengo que traer desde la memoria
   u_int32_t numero_pagina = direccion_logica / tamanio_pagina;
   u_int32_t desplazamiento = direccion_logica - (numero_pagina * tamanio_pagina);

   u_int32_t numero_marco;
   if (buscar_en_tlb(pid, numero_pagina) != -1)
   {
      numero_marco = buscar_en_tlb(pid, numero_pagina);
      return ((numero_marco * tamanio_pagina) + desplazamiento);
   }
   else
   {
      // le envia a memoria pid y numero de pag para buscar en la tabla de paginas
      return ((numero_marco * tamanio_pagina) + desplazamiento);
      cargar_matriz_tlb(pid, numero_pagina, numero_marco);
   }
}
