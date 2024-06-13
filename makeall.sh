#!/bin/bash

exit_code=0

error() {
   echo "Error: compile error for $1"
   exit $exit_code
}

clean_and_make() {
   make clean
   make 
   exit_code=$?
   cd ..

   if [ $exit_code -ne 0 ]; then
      error $1
   fi
}

cd utils/ && clean_and_make "utils"
cd kernel/ && clean_and_make "kernel"
cd cpu/ && clean_and_make "cpu"
cd memoria/ && clean_and_make "memoria"
cd entradasalida/ && clean_and_make "entradasalida"

echo "All modules compiled successfully"
