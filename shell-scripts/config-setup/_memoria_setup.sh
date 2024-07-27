#!/bin/bash

cd ../memoria/
rm -f *.config

cd ../c-comenta-pruebas
cwd=$(pwd)

cd ../memoria/

echo "PUERTO_ESCUCHA=8002" >>memoria.config
echo "" >>memoria.config
echo "PATH_INSTRUCCIONES=${cwd}" >>memoria.config
echo "" >>memoria.config
