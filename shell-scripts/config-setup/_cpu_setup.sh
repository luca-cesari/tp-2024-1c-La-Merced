#!/bin/bash

cd ../cpu/
rm -f *.config

echo "PUERTO_ESCUCHA_DISPATCH=8006" >> cpu.config
echo "PUERTO_ESCUCHA_INTERRUPT=8007" >> cpu.config
echo "" >> cpu.config
echo "IP_MEMORIA=127.0.0.1" >> cpu.config
echo "PUERTO_MEMORIA=8002" >> cpu.config
echo "" >> cpu.config