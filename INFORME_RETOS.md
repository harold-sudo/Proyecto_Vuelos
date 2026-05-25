INFORME: Análisis de Rutas Aéreas con Algoritmos de Grafos
===========================================================

Fecha: 25 de mayo de 2026
Autor: (completa con tu nombre)

Resumen
-------
Este informe documenta el desarrollo del proyecto "Análisis de Rutas Aéreas". Se explica el modelado del problema, los objetivos, la construcción y limpieza de datos, el desarrollo y análisis de los retos (1,2,3 y bonus), complejidad temporal y espacial, conclusiones y referencias.

1. Descripción del problema: Contexto y modelado del grafo
--------------------------------------------------------
Contexto:
- Se trabaja sobre el dataset OpenFlights que contiene información de aeropuertos (`airports.dat`) y rutas (`routes.dat`).
- El objetivo es analizar la red de rutas aéreas como un grafo dirigido: los aeropuertos son nodos y cada vuelo comercial entre dos aeropuertos es una arista dirigida.

Modelado del grafo:
- Vertices (V): identificación de aeropuertos por su `ID` en `airports.dat`. Se eligió usar `ID` (cadena) como clave porque es único y evita ambigüedades con códigos IATA faltantes.
- Aristas (E): una arista dirigida `u -> v` existe si hay al menos una ruta que va desde el aeropuerto `u` hacia `v`. En el caso del reto 5 (bonus), las aristas se filtran por la aerolínea.
- Pesos: para problemas de distancia (reto 3 y 5), cada arista tiene un peso igual a la distancia geográfica (km) calculada con la fórmula de Haversine usando latitud/longitud de los aeropuertos.

2. Objetivos
-----------
- Implementar algoritmos clásicos de grafos para analizar propiedades de la red de rutas aéreas.
- Reto 1: Calcular el alcance de un aeropuerto con un máximo de 3 escalas.
- Reto 2: Identificar componentes fuertemente conexas y su tamaño máximo.
- Reto 3: Encontrar el par de aeropuertos cuya distancia mínima total sea la mayor (diámetro efectivo) usando Dijkstra con pesos Haversine.
- Reto 4 (implementado parcialmente en el enunciado): Red mínima en Sudamérica (no implementado en código actual).
- Reto 5 (BONUS): Encontrar el camino más corto entre origen y destino usando solo una aerolínea.

3. Desarrollo
-------------
3.1 Limpieza y carga de datos
- Archivos principales:
  - `data/airports.dat`: contiene ID, nombre, IATA, lat, lon, etc.
  - `data/routes.dat`: contiene Airline, AirlineID, Source airport ID, Destination airport ID, etc.
- En el repositorio se incluyen versiones limpias (`src/airports_clean.dat`, `src/routes_clean.dat`) para pruebas más rápidas y sin campos inválidos.

Pasos de limpieza realizados (según el notebook y el código fuente):
- Filtrar filas con campos `\\N` críticos (ID o coordenadas faltantes).
- Mantener solo columnas necesarias: para aeropuertos (ID, name, IATA, lat, lon); para rutas (Airline, SourceID, DestID).
- Eliminar rutas duplicadas (mismo par origen-destino) cuando se construye el grafo o subgrafo por aerolínea.

Archivos usados en el código:
- [src/main.cpp](src/main.cpp): función `cargarAeropuertos("airports_clean.dat")` y `cargarGrafo("routes_clean.dat")`.
- [notebook/ProyectoAnálisis.ipynb](notebook/ProyectoAnálisis.ipynb) contiene la versión exploratoria y scripts de limpieza.

3.2 Construcción del grafo
- Representación elegida: `unordered_map<string, vector<string>> grafo;` donde la clave es `origin_id` y el vector almacena `destination_id`s.
- Razón: acceso O(1) a la lista de vecinos y simple iteración de aristas salientes por origen.
- Alternativa: `unordered_map<string, unordered_set<string>>` para evitar duplicados sin usar un `set` auxiliar, pero el vector ofrece mejor rendimiento si se garantizan rutas únicas.

3.3 Reto 1 — Alcance personalizado (BFS)
- Archivo: [src/reto1.cpp](src/reto1.cpp)

Algoritmo:
- Buscar `id_origen` por IATA o buscando coincidencia en el nombre (ambos uppercased).
- BFS con cola de pares `(id_aeropuerto, escalas)` y `unordered_set visitados`.
- Encolar vecinos solo si `escalas < 3`.

Complejidad:
- Tiempo: O(V + E') donde E' son las aristas exploradas hasta 3 niveles desde el origen.
- Espacio: O(V) para `visitados`.

3.4 Reto 2 — Grupos y aislamiento aéreo (Kosaraju)
- Archivo: [src/reto2.cpp](src/reto2.cpp)

Algoritmo:
- Primera DFS para calcular orden de finalización (pila).
- Construir grafo transpuesto (invertir aristas).
- Segunda fase: procesar nodos en orden inverso y ejecutar DFS en el transpuesto para encontrar cada SCC.

Complejidad:
- Tiempo: O(V + E)
- Espacio: O(V + E)

3.5 Reto 3 — Máxima eficiencia / Diámetro (Dijkstra)
- Archivo: [src/reto3.cpp](src/reto3.cpp)

Algoritmo:
- Para cada aeropuerto que tiene rutas de salida, ejecutar Dijkstra (peso = Haversine entre aeropuertos de arista).
- Mantener la distancia máxima encontrada entre pares (distancia mínima entre par) como diámetro efectivo.

Implementación (puntos clave):
- Haversine:
  - Convertir grados a radianes con PI.
  - Aplicar fórmula para obtener distancia en km.
- Dijkstra:
  - Min-heap `priority_queue` con `greater<>`.
  - `unordered_map<string,double> distancias` inicializadas con `1e9` como infinito.
  - Relajación de aristas y actualización de heap.

Complejidad:
- Tiempo: O(V * (E log V)) en implementación ingenua donde se ejecuta Dijkstra desde muchos orígenes.
- Espacio: O(V)

Observaciones: el cómputo puede ser costoso para todos los orígenes; se puede paralelizar o muestrear para optimizar.

3.6 Reto 5 — Pasajero leal (Dijkstra sobre subgrafo de aerolínea)
- Archivo: [src/reto5.cpp](src/reto5.cpp)

Algoritmo:
- Pedir origen, destino y código de aerolínea al usuario.
- Leer `routes.dat` (original) y construir subgrafo solo con rutas cuya columna `Airline` coincide con el código ingresado.
- Aplicar Dijkstra sobre este subgrafo (peso = Haversine) desde `id_origen` hasta `id_destino`.
- Reconstruir camino usando `predecesores`.

Complejidad:
- Lectura/filtrado: O(L) donde L = número de líneas en `routes.dat`.
- Dijkstra en subgrafo: O(E' log V').

4. Análisis de complejidad temporal y espacial
--------------------------------------------
Resumen por reto:
- Reto 1 (BFS hasta 3 niveles): Tiempo O(V + E') restringido a un radio de 3 saltos; Espacio O(V)
- Reto 2 (Kosaraju): Tiempo O(V + E); Espacio O(V + E)
- Reto 3 (Dijkstra repetido): Tiempo O(K * (E log V)) si se ejecuta Dijkstra desde K orígenes (K puede ser V en el peor caso); Espacio O(V)
- Reto 5 (Dijkstra en subgrafo): Tiempo O(L + E' log V') donde L es tamaño de `routes.dat` y E',V' son tamaño del subgrafo filtrado; Espacio O(V')

Notas sobre O-notation práctica:
- En grafos dispersos E ~ O(V). En redes de rutas aéreas, el grado medio puede ser pequeño, pero ciertos hubs elevan E.
- Las implementaciones usan `unordered_map` y `unordered_set` para accesos O(1) promedio.

5. Conclusiones
---------------
- El modelado como grafo dirigido es natural y permite aplicar herramientas clásicas de análisis de redes.
- Las decisiones prácticas (usar ID como clave, limpiar datos) son fundamentales: sin ello los algoritmos fallarían por claves faltantes.
- Dijkstra con Haversine es una buena aproximación para distancia efectiva, pero presupone que las rutas pueden conectarse en secuencia sin restricciones operativas (es decir, no considera tiempos de vuelo ni horarios).
- El reto 5 demuestra cómo restringir el análisis por operador (aerolínea) cambia drásticamente la conectividad del subgrafo.

6. Bibliografía y referencias
-----------------------------
- OpenFlights.org datasets — airports.dat and routes.dat
- Cormen, Leiserson, Rivest, Stein. Introduction to Algorithms — algoritmos BFS, Dijkstra y teoría de grafos.
- Fórmula de Haversine — para cálculo de distancias en la superficie terrestre.


---

Apéndice: comandos útiles
------------------------
- Compilar:

```bash
cd src
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp reto1.cpp reto2.cpp reto3.cpp reto5.cpp -o proyecto_vuelos_bonus
```

- Ejecutar:

```bash
./proyecto_vuelos_bonus
```

- Generar PDF desde Markdown (opcional, necesita `pandoc`):

```bash
pandoc -s INFORME_RETOS.md -o Informe_Final.pdf --pdf-engine=xelatex
```


Fin del informe.
