#include <stdlib.h>
#include <commons/process.h>

struct PCB
{
   int32_t pid;
   int32_t program_counter;
   u_int32_t quantum;
   // cpu registers
};

void crear_proceso();
void destruir_proceso();