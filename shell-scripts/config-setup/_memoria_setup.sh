#!/bin/bash

cd ../memoria/
rm -f *.config

echo "PUERTO_ESCUCHA=8002" >> memoria.config
echo "" >> memoria.config
echo "PATH_INSTRUCCIONES=/home/utnso/scripts-pruebas" >> memoria.config
echo "" >> memoria.config

