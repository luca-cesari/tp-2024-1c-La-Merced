#!/bin/bash

cd ../kernel/
rm -f *.config

echo "PUERTO_ESCUCHA=8003" >> kernel.config
echo "" >> kernel.config
echo "IP_MEMORIA=127.0.0.1" >> kernel.config
echo "PUERTO_MEMORIA=8002" >> kernel.config
echo "" >> kernel.config
echo "IP_CPU=127.0.0.1" >> kernel.config
echo "PUERTO_CPU_DISPATCH=8006" >> kernel.config
echo "PUERTO_CPU_INTERRUPT=8007" >> kernel.config
echo "" >> kernel.config
