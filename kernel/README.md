# Kernel

## TODO

### Generales

- [ ] Implementar función `log_ingreso_a_ready()` (logger/logger.c)
- [ ] Testear la config:
  - Si devuelven lo que tienen que devolver
  - Si se libera la memoria como corresponde con el destroy
- [ ] Implementar envío y recepción de PCB (con la CPU):
  - Implementar en utils como protocolo de comunicación CPU <-> Kernel
  - Implemenar serialización y deserialización
- [ ] Revisar el `main.c`, quizá mejor abstraer la lógica de conexiones

### Planificador

- [ ] Pensar e Implementar como planificar a corto plazo (según el config)
- [ ] Empezar a loggear en donde corresponde
- [ ] Implementar pausa y reanudación de planificadores
- [ ] Implementar función `modificar_grado_multiprogramacion()`

### Consola

- [ ] Implementar EJECUTAR_SCRIPT
- [ ] Implementar PROCESO_ESTADO

## Notas
