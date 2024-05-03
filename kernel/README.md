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
- [X] Revisar el `main.c`, quizá mejor abstraer la lógica de conexiones
- [ ] Revisar los header files, documentar y definir nombres de parámetros en caso de ser necesario.
- [ ] Implementar servidor para las interfaces (sacarlo del main y lanzar las rutinas correspondientes)

### Planificador

- [ ] Pensar e Implementar como planificar a corto plazo (según el config)
- [ ] Empezar a loggear en donde corresponde
- [ ] Implementar pausa y reanudación de planificadores
- [ ] Implementar función `modificar_grado_multiprogramacion()`

### Consola

- [X] Implementar EJECUTAR_SCRIPT (Esta implementado pero hay que testear)
- [ ] Implementar PROCESO_ESTADO

## Notas

### Manejo de Interfaces I/O

Idea para manejar el estado blocked respecto de las interfaces I/O:

- Usar diccionario (`t_dictionary` de la commons) para el estado BLOCKED, (hay que armar un `t_mutex_dictionary` si se usara esta solución).
- Las claves serán los nombres de las interfaces (o alguna estructura que facilite ciertas operaciones).
- El valor de cada clave es una cola (ver bien si usar `t_queue` o `t_mutex_queue`).

Manejo de conexiones y desconexiones:

- Si una interfaz se conecta, se hace un `dictionary_put`, con el nombre de la interfaz como clave y una cola vacía como valor.
- Si una interfaz se desconecta:
  - Se hace un `dictionary_remove`.
  - Con la cola retornada, pasar los procesos a EXIT y destruír la cola.

Manejo de peticiones para I/O:

- Validar existencia por `dictionary_has_key`.
- Validar estado de conexión por `dictionary_get`, si es `NULL`, entonces está desconectada.
- Validar operación a realizar, (no c todavía, pero se puede hacer por alguna estructura auxiliar, o preguntando a alguna entidad).
- Encolar el proceso para que espere su turno. (capaz acá hay que pensar mejor como comunicar el estado de la interfaz; marcando con una variable, o simplemente asumirlo y que los procesos vayan entrando una vez que el anterior sale).
