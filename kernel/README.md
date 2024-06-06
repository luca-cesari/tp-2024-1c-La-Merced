# Kernel

## TODO

### Generales

- [X] Definir los motivos de finalización para el planificador en lugar del logger.
- [ ] Implementar función `log_ingreso_a_ready()` (logger/logger.c)
- [ ] Testear la config:
  - Si devuelven lo que tienen que devolver
  - Si se libera la memoria como corresponde con el destroy
- [X] Implementar envío y recepción de PCB (con la CPU):
  - Implementar en utils como protocolo de comunicación CPU <-> Kernel
  - Implemenar serialización y deserialización
- [X] Revisar el `main.c`, quizá mejor abstraer la lógica de conexiones
- [ ] Revisar los header files, documentar y definir nombres de parámetros en caso de ser necesario.
- [X] Implementar servidor para las interfaces (sacarlo del main y lanzar las rutinas correspondientes)

### Conexiones

- [X] Implementar (para las conexiones compartidas entre hilos) mutex.
- [X] Implementar operaciones con memoria

### Planificador

- [X] Pensar e Implementar como planificar a corto plazo (según el config)
- [X] Empezar a loggear en donde corresponde
- [ ] Implementar pausa y reanudación de planificadores
- [ ] Implementar función `modificar_grado_multiprogramacion()`
- [X] Refactorizar estados, unificar los strusts que se puedan unificar

### Consola

- [X] Implementar EJECUTAR_SCRIPT (Esta implementado pero hay que testear)
- [ ] Implementar PROCESO_ESTADO

## Problemas

- Mucho código repetido en los estados (son casi todos iguales)
- Definición de estados de proceso en el logger, capaz es mejor definilo en otro lugar y consumirlo en logger y planificador (sería más correcto conceptualmente)

## Notas
