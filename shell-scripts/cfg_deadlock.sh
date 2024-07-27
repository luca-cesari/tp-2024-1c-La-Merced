#!/bin/bash

bash config-setup/_kernel_setup.sh
bash config-setup/_memoria_setup.sh
bash config-setup/_cpu_setup.sh
bash config-setup/_entradasalida_setup.sh ESPERA
cd ..

# Kernel
cd kernel/
echo "ALGORITMO_PLANIFICACION=FIFO" >> kernel.config
echo "QUANTUM=1500" >> kernel.config
echo "RECURSOS=[RA, RB, RC, RD]" >> kernel.config
echo "INSTANCIAS_RECURSOS=[1, 1, 1, 1]" >> kernel.config
echo "GRADO_MULTIPROGRAMACION=10" >> kernel.config
cd ..

# Memoria
cd memoria/
echo "TAM_MEMORIA=1024" >> memoria.config
echo "TAM_PAGINA=32" >> memoria.config
echo "RETARDO_RESPUESTA=1000" >> memoria.config
cd ..

# CPU
cd cpu/
echo "CANTIDAD_ENTRADAS_TLB=0" >> cpu.config
echo "ALGORITMO_TLB=FIFO" >> cpu.config
cd ..

cd entradasalida/

# IO
echo "TIPO_INTERFAZ=GENERICA" >> ESPERA.config
echo "TIEMPO_UNIDAD_TRABAJO=500" >> ESPERA.config
