#include "mmu.h"

u_int32_t mmu(u_int32_t direccion_logica)
{
   u_int32_t pid = process_getpid();

   u_int32_t tamanio_pagina = get_tamanio_pagina(); // lo tengo que traer desde la memoria
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
      parametros parametro;
      parametro.nro_pag = numero_pagina;
      t_cpu_mem_req *mem_request = crear_cpu_mem_request(OBTENER_MARCO, pid, parametro);
      enviar_mem_request(mem_request);
      numero_marco = recibir_marco();
      return ((numero_marco * tamanio_pagina) + desplazamiento);
      cargar_matriz_tlb(pid, numero_pagina, numero_marco);
   }
}
