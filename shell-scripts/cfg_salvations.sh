#!/bin/bash

bash config-setup/_kernel_setup.sh
bash config-setup/_memoria_setup.sh
bash config-setup/_cpu_setup.sh
bash config-setup/_entradasalida_setup.sh FS TECLADO MONITOR
cd ..

# Kernel
cd kernel/
echo "ALGORITMO_PLANIFICACION=VRR" >> kernel.config
echo "QUANTUM=5000" >> kernel.config
echo "RECURSOS=[RA, RB, RC, RD]" >> kernel.config
echo "INSTANCIAS_RECURSOS=[1,1,1,1]" >> kernel.config
echo "GRADO_MULTIPROGRAMACION=3" >> kernel.config
cd ..

# Memoria
cd memoria/
echo "TAM_MEMORIA=4096" >> memoria.config
echo "TAM_PAGINA=32" >> memoria.config
echo "RETARDO_RESPUESTA=100" >> memoria.config
cd ..

# CPU
cd cpu/
echo "CANTIDAD_ENTRADAS_TLB=16" >> cpu.config
echo "ALGORITMO_TLB=LRU" >> cpu.config
cd ..

cd entradasalida/

# GENERICA

echo "TIPO_INTERFAZ=GENERICA" >> SLP1.config
echo "TIEMPO_UNIDAD_TRABAJO=50" >> SLP1.config

echo "TIPO_INTERFAZ=GENERICA" >> ESPERA.config
echo "TIEMPO_UNIDAD_TRABAJO=500" >> ESPERA.config

echo "TIPO_INTERFAZ=GENERICA" >> GENERICA.config
echo "TIEMPO_UNIDAD_TRABAJO=250" >> GENERICA.config

# TECLADO
echo "TIPO_INTERFAZ=STDIN" >> TECLADO.config
echo "TIEMPO_UNIDAD_TRABAJO=50" >> TECLADO.config

# MONITOR
echo "TIPO_INTERFAZ=STDOUT" >> MONITOR.config
echo "TIEMPO_UNIDAD_TRABAJO=50" >> MONITOR.config
