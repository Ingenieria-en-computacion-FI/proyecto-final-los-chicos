# Bitácora IA
**Mini OS Project — Estructuras de Datos y Algoritmos I**  
**Herramienta utilizada:** Claude (Anthropic)  
**Facultad de Ingeniería - UNAM - Semestre 2026-2**  
**Profesora:** Ing. Adara Mercado Martínez

---

## Nombre del equipo

**Los Chicos** → Brigada 5

## Integrantes del equipo

| Nombre |
|---|
| Chávez Sánchez Ehidan Javier |
| Salgado Sandoval Alexis Uriel |
| Villaruel Sánchez David Uriel |
| Castillo Vizcarra Ihsahn |

---

## Análisis de la estructura del proyecto

### Prompt utilizado

> Se requiere la planificación para la implementación de estructuras de datos lineales, diseño de algoritmos, programación modular y la adaptación a través de lenguaje C y Python para la integración de la simulación de un mini OS capaz de administrar procesos, memoria, recursos, estructuras dinámicas, algoritmos de optimización; teniendo por objetivo el uso correcto y el análisis formal de dichas estructuras. Ayúdame a trazar una ruta de acciones para la correcta implementación del proyecto; cabe recalcar que aunque ayudarás a la implementación de código, es vital no sustituir el razonamiento humano. [se adjunta imagen de la estructura localizada en el repositorio de GitHub]

### Respuesta de la IA

Claude analizó el problema a implementar e indicó posibles rutas de adaptación. A partir de esto se eligió la siguiente ruta de codificación propuesta por la IA:

| Paso | Módulo | Descripción |
|---|---|---|
| 1 | `include/` | Diseño de headers — definición de todos los `.h` con structs |
| 2 | `src/` | Estructuras de datos base — Stack, Queue, LinkedList, etc. |
| 3 | `src/memory/` | Gestión de memoria — archivos de administración de bloques |
| 4 | `src/scheduler/` | Schedulers — FIFO, Round Robin y SJF |
| 5 | `src/algorithms/` | Algoritmos — fuerza bruta, greedy, backtracking, divide y vencerás, DP |
| 6 | `src/utils/` | Utilidades — gestión de procesos con parámetros de tiempo y memoria |
| 7 | `scripts/` | Python — gráficas, automatización y benchmarking |
| 8 | `src/main.c` | Integración — conectar todos los módulos en un flujo completo |
| 9 | `tests/` | Pruebas y compilación — corregir errores y verificar resultados |

### Errores encontrados

Ninguno. Se decidió que esa ruta era la mejor a implementar por el equipo.

### Correcciones y validaciones

No hubo correcciones. Se validó la ruta a seguir por el equipo. Además, en la codificación se demostró que fue la más óptima.

---

## Diseño de estructuras de datos lineales

### Prompt utilizado

> Necesito implementar las estructuras de datos lineales para un simulador de OS en C. Requiero headers para: pila con historial de ejecución, cola FIFO para scheduler, cola circular para Round Robin, lista ligada ordenada por PID y lista doblemente ligada para bloques de memoria. ¿Puedes proponer algunas ideas concretas para la definición de los TDA requeridos, asegurando que cumplan con los requisitos para la implementación de gestión de memoria en el mini OS? A través de estos códigos que te envío recicla código y propón la pila, lista y lista doblemente enlazada; adaptándolos al proyecto según las necesidades establecidas.

### Respuesta de la IA

La IA propuso los 5 tipos de datos abstractos reservando memoria con `malloc/free` por nodo. Para `circular_queue.c` usó aritmética módulo `% CIRCULAR_QUEUE_MAX` para el wrap-around sin redimensión. La lógica para la pila, lista enlazada y doblemente enlazada fue propuesta a través del reciclaje de código de prácticas pasadas; la IA la adaptó al proyecto con parámetro `pid` y `Process` para la gestión de memoria, permitiendo al equipo asimilar las diferencias.

### Errores encontrados

- `circular_queue.c` usaba `fprintf(stderr)` en cada operación, innecesariamente verboso.
- `linked_list.c` no manejaba correctamente la inserción ordenada cuando la lista tenía más de 2 nodos con PIDs desordenados.

### Correcciones realizadas

- El equipo simplificó `circular_queue.c` — haciendo que las funciones retornaran `0` en caso de fallo sin mensajes de error.
- El equipo corrigió la lógica de `list_insert_sorted` separando el caso de lista vacía del caso general.

### Validaciones realizadas

- Se insertaron 5 procesos con PIDs desordenados y se verificó que `list_print` los mostrara en orden ascendente.
- Se probó enqueue/dequeue en la cola circular con más elementos que `CIRCULAR_QUEUE_MAX/2` para verificar el wrap-around.

---

## Administración de memoria

### Prompt utilizado

> Revisa los siguientes archivos de administración de memoria implementados por el equipo: `first_fit.c` con búsqueda lineal retornando el `start` del bloque asignado, `best_fit.c` con búsqueda exhaustiva del menor desperdicio usando variable `menor_diferencia`, `worst_fit.c` con búsqueda del bloque de mayor tamaño usando variable `mayor_tamano`, `coalescence.c` con fusión de bloques libres adyacentes sin avanzar el puntero al fusionar para absorber múltiples bloques en una pasada, y `divide_v.c` con estrategia de desplazamiento de bloques ocupados al inicio recalculando `start` con variable `ubiactual`. Verifica que cumplan con lo que pide el proyecto, indica exactamente qué líneas cambiar si hay errores y si algún archivo está mal ubicado o mal nombrado.

### Respuesta de la IA

La IA detectó que `mm_allocate_first_fit` y `mm_allocate_best_fit` no recibían `pid` como parámetro, lo que impedía que `mm_free` liberara correctamente los bloques. También detectó que `divide_v.c` era en realidad la compactación y estaba mal ubicado. Asimismo relacionó incongruencias en la sintaxis de los archivos de gestión de memoria; recalcó que la estructura era correcta pero algunas funciones estaban inicializadas con diferente nombre en archivos dependientes unos de otros. Al final propuso los ajustes con los parámetros requeridos y en correcta sintaxis, así como mejoras como la anidación de bucles en `coalescence.c` para la evaluación de bloques libres.

### Errores encontrados

- `mm_allocate_first_fit` y `mm_allocate_best_fit` no asignaban `pid` al bloque — `mm_free` no encontraba qué liberar.
- Al compilar: `multiple definition of mm_allocate_first_fit` porque estaba en `memory_manager.c` y en `first_fit.c` simultáneamente.
- `divide_v.c` nombrado y ubicado incorrectamente — debía ser `compaction.c` en `src/memory/`.

### Correcciones realizadas

- El equipo agregó `int pid` como parámetro a las 3 funciones de allocate en `.h` y `.c`.
- El equipo eliminó las funciones duplicadas de `memory_manager.c`.
- El equipo renombró `mm_divide_v` a `mm_compact` y actualizó `memory_manager.h`.
- El equipo usó la propuesta de anidación de ciclos para evaluación de bloques libres en `coalescence.c`.

### Validaciones realizadas

- Se asignaron 3 procesos, se liberó el del medio y se verificó que `mm_coalesce` fusionara los bloques libres adyacentes.
- Se confirmó que `mm_free` liberaba correctamente usando el PID tras la corrección.

---

## Schedulers

### Prompt utilizado

> Revisa e implementa los schedulers del proyecto. El equipo tiene `round_robin.c` con las indicaciones de implementar quantum, cola circular, reinserción y tiempos restantes pero sin código. Necesitamos `fifo.c` y `sjf.c` también. Los 3 deben usar las estructuras ya implementadas: `Queue` para FIFO, `CircularQueue` para Round Robin y ordenamiento por `burst_time` para SJF. Deben registrar historial en `Stack` y guardar procesos terminados en `LinkedList`. Sin espacios de alineación entre asignaciones.

### Respuesta de la IA

La IA propuso los 3 schedulers con `SchedulerResult` retornando tiempo promedio de espera y turnaround. Para SJF propuso ordenamiento por selección O(n²) sobre una copia del arreglo para no modificar el original.

### Errores encontrados

- Los schedulers generados tenían funciones `mock_memory_allocate` y `mock_memory_free` definidas pero no usadas — warnings de compilación con `-Wunused-function`.
- `scheduler_print_result` estaba declarada en `scheduler.h` pero no implementada en ningún `.c`.

### Correcciones realizadas

- El equipo eliminó las funciones mock de los 3 schedulers.
- El equipo creó `scheduler_utils.c` en `src/scheduler/` con la implementación de `scheduler_print_result` y `scheduler_reset_processes`.

### Validaciones realizadas

- Se verificó el ejemplo del PDF con Round Robin quantum=2: P1(5), P2(3), P3(4) ejecutando en el orden correcto.
- Se confirmó que SJF ejecuta en orden ascendente de `burst_time`.

---

## Estrategias para la construcción de algoritmos

### Prompt utilizado

> Implementa fuerza bruta para búsqueda de huecos y detección de fragmentación, greedy como selector unificado de políticas de memoria, backtracking para guardar y restaurar estado de memoria, divide y vencerás para compactación con merge sort, y programación dinámica en sus 3 variantes: descendente, ascendente e incremental.

### Respuesta de la IA

La IA propuso la estructura de los 6 archivos. Para backtracking propuso `MemorySnapshot` con arreglos paralelos de `starts`, `sizes`, `frees` y `pids`. Para divide y vencerás propuso merge sort sobre punteros a bloques para ordenar por dirección de inicio.

### Errores encontrados

- `dp_topdown` daba resultado distinto a `dp_bottomup` e `dp_incremental`.
- El bug estaba en `main.c`: `int memo[(n+1)*(target+1)]` declarado como VLA en el stack causaba comportamiento indefinido.

### Correcciones realizadas

El equipo identificó que `memo` estaba declarado como VLA en el stack y lo migró al heap:

```c
// Antes:
int memo[(n + 1) * (target + 1)];

// Después:
int* memo = malloc((n + 1) * (target + 1) * sizeof(int));
free(memo);
```

### Validaciones realizadas

- Los 3 algoritmos DP dieron resultado consistente: NO es posible asignar 2048 unidades con 5 procesos cuya suma de memoria es 800.
- Se verificó que no hubiera fugas de memoria en el snapshot de backtracking.

---

## Integración C/Python y compilación final

### Prompt utilizado

> Genera los scripts Python necesarios para la integración con el simulador en C: un script que genere procesos aleatorios en CSV con diferentes escenarios y tamaños, un benchmark que ejecute el binario con subprocess y mida tiempos, un script de gráficas con matplotlib que lea los resultados y un validador de archivos CSV.

### Respuesta de la IA

La IA propuso la estructura de los 4 scripts: `generate_processes.py` con 4 escenarios configurables, `benchmark.py` midiendo tiempos con `perf_counter`, `graphs.py` generando 3 tipos de gráficas con matplotlib y `validate_results.py` verificando integridad de los CSV campo por campo.

### Errores encontrados

- `benchmark.py` llamaba con `--filename` pero `generate_processes.py` no tenía ese parámetro — todos los benchmarks fallaban con `ERROR INPUT`.
- `benchmark.py` no manejaba el caso en que el binario no existía.
- `generate_processes.py` no tenía el parámetro `--seed` para resultados reproducibles.

### Correcciones realizadas

El equipo agregó validación de existencia del binario en `benchmark.py`, el parámetro `--seed` y `--filename` en `generate_processes.py`:

```python
parser.add_argument("--filename", type=str, default=None)
filename = f"{args.filename}.csv" if args.filename else f"processes_{args.scenario}_{ts}.csv"
```

### Validaciones realizadas

- Se ejecutó `benchmark.py` con el binario compilado y se verificó que generara el CSV con las columnas correctas.
- Se corrió `validate_results.py` sobre los inputs generados y todos pasaron sin errores.

---

## Síntesis de uso de la Inteligencia Artificial — Claude

| Módulo | Contribución IA | Corrección del equipo |
|---|---|---|
| Ruta de implementación | Propuso la ruta de desarrollo | El equipo validó y adoptó la ruta |
| Headers `include/` | Propuso 6 headers con structs y prototipos | Fix redefiniciones, agregar `pid` |
| Estructuras base `src/` | Propuso implementación con `malloc/free` | Simplificación, fix inserción ordenada |
| Memoria `src/memory/` | Detectó errores y propuso ajustes | Eliminar duplicados, reubicar archivos |
| Schedulers `src/scheduler/` | Propuso los 3 schedulers | Eliminar funciones mock, agregar utils |
| Algoritmos `src/algorithms/` | Propuso estructura de los 6 archivos | Fix VLA en `memo` de DP |
| Scripts Python `scripts/` | Propuso estructura de los 4 scripts | Fix `--filename`, `--seed`, manejo de errores |
| Makefile | Propuso flag POSIX | El equipo verificó y aplicó |


NOTA: Toda la bitacora de la IA se hizo por parte del equipo en un archivo word, luego se le pidio a cloude que lo transformara a extencion md para incluirlo al repositorio de github; con el fin de dar una buena estructura a la bitacora, ya que ningun integrante conocia la sintaxis del tipo de extención.

