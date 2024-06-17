#!/bin/bash

cd ../entradasalida/
rm -f *.config

i=1;
for interface_name in "$@" 
do
   file_name=$interface_name".config";

   echo "IP_KERNEL=127.0.0.1" >> $file_name
   echo "PUERTO_KERNEL=8003" >> $file_name
   echo "" >> $file_name
   echo "IP_MEMORIA=127.0.0.1" >> $file_name
   echo "PUERTO_MEMORIA=8002" >> $file_name
   echo "" >> $file_name

   i=$((i + 1));
done
