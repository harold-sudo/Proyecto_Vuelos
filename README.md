# Proyecto_Vuelos — Explicación completa

Este README explica en detalle cómo funciona el proyecto, cómo se resuelve cada reto, cómo se leen los datos, y las razones relevantes detrás de declaraciones y decisiones de diseño en el código fuente. Está escrito para que cualquier persona (incluso sin mucha experiencia) pueda entender el flujo, los algoritmos y por qué el código está organizado de la forma actual.

---

## Estructura del repositorio (resumen)

- `data/` — Contiene los archivos originales `airports.dat` y `routes.dat` (dataset OpenFlights) que tienen información completa.
- `src/` — Código fuente en C++:
  - `main.cpp` — Programa principal y menú.
  - `reto1.h` / `reto1.cpp` — Reto 1: Alcance personalizado (BFS)
  - `reto2.h` / `reto2.cpp` — Reto 2: Componentes fuertemente conexas (Kosaraju)
  - `reto3.h` / `reto3.cpp` — Reto 3: Diámetro efectivo (Dijkstra + Haversine)
  - `reto5.h` / `reto5.cpp` — Reto opcional: Pasajero leal (camino más corto con una sola aerolínea)
  - `airports_clean.dat`, `routes_clean.dat` — versiones simplificadas / limpiadas usadas por el programa para rapidez en pruebas.

---

## Notas generales de diseño

- Se utiliza un grafo dirigido: cada aeropuerto es un nodo y cada ruta (vuelo) desde A hacia B es una arista dirigida `A -> B`.
- Los algoritmos elegidos corresponden a problemas clásicos:
  - BFS (Breadth-First Search) para alcance limitado por número de escalas.
  - Kosaraju para encontrar componentes fuertemente conexas (SCCs).
  - Dijkstra para caminos de menor distancia cuando las aristas tienen pesos (en este caso, distancias geográficas en km calculadas por Haversine).
- Para el bonus (reto 5) se requiere filtrar rutas por aerolínea. El archivo `routes.dat` original contiene el código de aerolínea por fila y por eso el programa puede construir un subgrafo por aerolínea para aplicar Dijkstra.
- El código prefiere claridad y pequeñas funciones con responsabilidad única (lectura, parseo, construir grafo, algoritmo), lo cual facilita pruebas y mantenibilidad.

---

## Formato de los archivos de datos (qué columnas se usan)

- `airports.dat` (filas con coma): columnas de interés (índices basados en OpenFlights):
  - ID (columna 0) — identificador numérico del aeropuerto (usado como clave en el grafo).
  - Name (columna 1) — nombre legible del aeropuerto.
  - IATA (columna 4) — código IATA de 3 letras (puede estar vacío `\\N`).
  - Latitude (columna 6), Longitude (columna 7) — coordenadas geográficas usadas para calcular distancias.

- `routes.dat` (filas con coma): columnas de interés:
  - Airline (columna 0) — código de la aerolínea que opera la ruta (ej. `2B`).
  - Source airport ID (columna 3) — ID numérico origen.
  - Destination airport ID (columna 5) — ID numérico destino.

Notas: los archivos pueden contener registros inválidos (`\\N`) o rutas hacia/desde aeropuertos no incluidos en `airports.dat`. En la carga, se filtran esas filas.

---

## Compilar y ejecutar

Para compilar (desde `src/`):

```bash
cd src
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp reto1.cpp reto2.cpp reto3.cpp reto5.cpp -o proyecto_vuelos_bonus
```

Ejecutar:

```bash
./proyecto_vuelos_bonus
```

El programa muestra un menú con opciones para ejecutar cada reto. Los archivos `*_clean.dat` deben estar en `src/` o ajustar las rutas en `main.cpp`.

---

## Archivo `main.cpp` — lectura y estructura principal

Resumen de responsabilidades:
- Leer los archivos limpios `airports_clean.dat` y `routes_clean.dat`.
- Construir la estructura de datos en memoria (mapa de aeropuertos y el grafo de rutas dirigidas).
- Adaptar estructuras a las definidas por cada reto modular (cada reto define su propio `struct` para claridad y encapsulación).
- Presentar un menú y delegar la ejecución a las funciones de los retos.

Puntos de código relevantes y explicación línea a línea (resumida):

- `struct Aeropuerto { string id; string iata; double lat; double lon; string nombre; }`:
  - `id`: clave primaria (cadena) usada en el grafo (se mantiene como cadena aunque el CSV dé números; evita conversiones y facilita map keys).
  - `iata`: código IATA opcional; útil para buscar por usuario.
  - `lat`, `lon`: coordenadas para calcular distancias (Haversine).
  - `nombre`: nombre legible para salidas.

- `cargarAeropuertos(const string& ruta)`:
  - Abre el archivo con `ifstream archivo(ruta)`.
  - Lee línea a línea y hace `stringstream ss(linea)` para separar por comas.
  - Extrae `id, iata, s_lat, s_lon, nombre` con `getline(ss, campo, ',')`.
  - Convierte lat/lon con `stod(s_lat)` y construye la entrada `Aeropuerto`.
  - Inserta en `mapa[id] = aero`.
  - Por qué `stod` y no `stof`? `stod` devuelve `double` que es la precisión deseada para cálculos Haversine.

- `cargarGrafo(const string& ruta)`:
  - Abre `routes_clean.dat` y por cada línea toma `origen` y `destino`.
  - Agrega `grafo[origen].push_back(destino)` creando una lista de adyacencia dirigida.
  - `unordered_map<string, vector<string>>` permite acceso O(1) al listado de vecinos.

- En `main()` se verifica que los mapas no estén vacíos: si lo están, el programa aborta con código `1`.

- Se crean adaptadores `mapa_r1`, `mapa_r3`, `mapa_r5` (copias con los `struct`s esperados por cada reto) simplemente para mantener tipos separados y evitar dependencias cruzadas.

---

## Reto 1 — `reto1.h` / `reto1.cpp` (Alcance personalizado con BFS)

Objetivo: Dado un aeropuerto (por IATA o nombre), calcular cuántos aeropuertos distintos son alcanzables con un máximo de 3 escalas.

Estrategia algorítmica:
- Aplicar BFS (búsqueda en anchura) desde el nodo origen.
- Mantener un conjunto `visitados` para no procesar nodos dos veces.
- Mantener una cola de pares `(id_aeropuerto, escalas_usadas)` para limitar a 3 escalas.

Explicación de responsabilidades (funciones):
- `reto1AlcanceBFS(const string& consulta, const unordered_map<string, AeropuertoReto1>& aeropuertos, unordered_map<string, vector<string>>& grafo)`
  - Entrada `consulta`: texto ingresado por usuario (IATA o nombre parcial).
  - Buscar el `id_origen` comparando IATA exacto y búsqueda por `nombre.find(consulta)` (ambos en mayúsculas para evitar falsos negativos por casos diferentes).
  - Si no hay origen o no existen rutas salientes, informar y retornar.
  - Inicializar `visitados` y `queue<pair<string,int>> cola`.
  - Inserta `(id_origen, 0)` y marca `visitados`.
  - Mientras la cola no esté vacía, saca el frente `(actual, escalas)` y, si `escalas < 3`, itera vecinos `grafo[actual]`.
  - Para cada vecino no visitado, marca visitado y encola `(vecino, escalas+1)`.
  - Al finalizar, `visitados.size()` indica la cantidad de aeropuertos distintos alcanzables (incluye el propio origen según la implementación). Si se desea excluir el origen, restar 1.

Motivos de decisiones relevantes:
- Usamos `unordered_set` para `visitados` por su costo O(1) promedio.
- Limitar escalas encolando el número actual de escalas evita re-calcular distancias y mantiene el BFS simple.

Complejidad:
- Tiempo: O(V + E) restringido por la capa de búsqueda hasta 3 niveles; en el peor caso se expanden los nodos hasta ese nivel.
- Espacio: O(V) para los `visitados`.

---

## Reto 2 — `reto2.h` / `reto2.cpp` (Kosaraju — SCCs)

Objetivo: Contar cuántos grupos aislados (componentes fuertemente conexas) existen y el tamaño del grupo más grande.

Estrategia:
- Kosaraju es un algoritmo clásico en dos pasos:
  1. Hacer DFS de todos los nodos y registrar el orden de finalización en una pila.
  2. Construir el grafo transpuesto (invertir aristas) y procesar nodos en orden de la pila (último en entrar, primero en salir). Cada DFS en el grafo transpuesto genera una SCC.

Puntos clave del código:
- `dfsOrdenFinalizacion(nodo, grafo, visitados, pila)` -> marca visitado y recorre vecinos; al terminar, `pila.push_back(nodo)`.
- `dfsSCC(nodo, grafo_transpuesto, visitados, tamano_componente)` -> recorre en el grafo transpuesto y cuenta tamaño.
- Construcción del grafo transpuesto: iterar todas las aristas `for (auto [origen, destinos] : grafo)` y agregar `grafo_transpuesto[destino].push_back(origen)`.
- Después de procesar todos los nodos, `total_scc` y `tamano_maximo` quedan calculados.

Complejidad:
- Tiempo: O(V + E) — dos DFS más la construcción del transpuesto.
- Espacio: O(V + E) para las estructuras auxiliares.

Motivo de Kosaraju: es simple de implementar y muy eficiente en tiempo lineal; es adecuado para grafos grandes si la memoria lo permite.

---

## Reto 3 — `reto3.h` / `reto3.cpp` (Dijkstra + Haversine)

Objetivo: Encontrar el par de aeropuertos cuya distancia mínima total (camino más corto según pesos km) sea la mayor entre todos los pares — esto aproxima el "diámetro" efectivo del grafo ponderado.

Estrategia:
- Para cada aeropuerto que tenga rutas de salida, ejecutar Dijkstra para obtener distancias mínimas desde ese origen a todos los demás aeropuertos.
- Para cada distancia finita encontrada, compararla con la máxima global y actualizar si es mayor.
- Calcular peso de cada arista usando la distancia geográfica (Haversine) entre coordenadas de los aeropuertos.

Detalles importantes:
- Haversine es la fórmula para distancia entre dos puntos sobre la esfera (aprox. Tierra). Se usan `double` y radianes para mayor precisión.
- Dijkstra se implementa con una `priority_queue` que actúa como min-heap (con `greater<>`).
- Se inicializa un `unordered_map<string,double> distancias` con valores grandes (ej. `1e9`) que actúan como `infinito`.

Razones y decisiones:
- Ejecutar Dijkstra desde cada origen es costoso (O(V * (E log V)) en práctica), pero para datasets medianos es aceptable. Se puede optimizar almacenando caminos si fuera necesario.
- Se muestran mensajes de progreso cada cierto número de orígenes para que el usuario tenga feedback en ejecuciones largas.

---

## Reto 5 — `reto5.h` / `reto5.cpp` (BONUS: El Pasajero Leal)

Objetivo: Encontrar el camino más corto (en km) entre un origen y un destino usando exclusivamente vuelos operados por una sola aerolínea.

Estrategia:
1. Pedir al usuario: origen (nombre o IATA), destino (nombre o IATA) y código de aerolínea.
2. Construir un subgrafo que contenga sólo las rutas donde la columna `Airline` coincide con el código pedido (esta información viene de `routes.dat`, no de los archivos limpios).
3. Ejecutar Dijkstra sobre el subgrafo para obtener la distancia mínima desde origen a destino. El peso de cada arista se calcula con Haversine usando coordenadas del aeropuerto.
4. Reconstruir el camino usando un mapa de `predecesores` y mostrarlo junto a la distancia total.

Puntos de implementación relevantes:
- Durante el parseo de `routes.dat`, se normaliza el código de aerolínea y se usa un `unordered_set` para evitar rutas duplicadas.
- Se filtran rutas donde `origin_id` o `destination_id` sean `\\N` o no existan en `airports`.
- Dijkstra y reconstrucción: se almacena `predecesores[vecino] = actual` cuando se mejora la distancia; al final, se reconstruye el camino iterando desde destino hasta origen usando este mapa.

Complejidad:
- Lectura del archivo y filtrado: O(L) donde L es filas de `routes.dat`.
- Dijkstra sobre el subgrafo: O(E' log V') donde E'/V' son tamaños del subgrafo filtrado por aerolínea.

---

## Notas sobre estilo: `using namespace std;` en todos los archivos

El proyecto fue ajustado a tu preferencia para contener `using namespace std;` en todos los archivos fuente y cabecera. Esto hace el código más corto visualmente (puedes escribir `string` en lugar de `std::string`), pero en proyectos más grandes esto puede introducir conflictos de nombres. En este contexto de repositorio académico y archivos pequeños, la decisión facilita lectura sin riesgo significativo.

---

## Explicación de fragmentos críticos (línea por línea, con motivos)

A continuación explico algunos fragmentos que son esenciales. No repetiré cada `#include` trivial, sino las líneas que contienen lógica o decisiones importantes.

### Lectura de aeropuertos (cargarAeropuertos)

```cpp
ifstream archivo(ruta);
string linea;

while (getline(archivo, linea)) {
    if (linea.empty()) continue; // ignorar líneas vacías
    stringstream ss(linea);
    string id, iata, s_lat, s_lon, nombre;

    getline(ss, id, ',');
    getline(ss, iata, ',');
    getline(ss, s_lat, ',');
    getline(ss, s_lon, ',');
    getline(ss, nombre, '\n');

    Aeropuerto aero = {id, iata, stod(s_lat), stod(s_lon), nombre};
    mapa[id] = aero;
}
```

- `ifstream archivo(ruta);` abre el archivo en modo lectura.
- `getline(archivo, linea)` lee cada línea completa; esto evita problemas con campos que contienen espacios.
- `stringstream ss(linea)` permite separar la línea por comas con `getline(ss, campo, ',')`.
- Se usa `stod` para convertir cadenas a `double` y así lograr precisión en cálculos geográficos.
- Se guarda en `mapa` usando la `id` como clave porque `id` es el identificador más estable (IATA puede faltar o repetirse).

### Construcción del grafo (cargarGrafo)

```cpp
while (getline(archivo, linea)) {
    if (linea.empty()) continue;
    stringstream ss(linea);
    string origen, destino;

    getline(ss, origen, ',');
    getline(ss, destino, '\n');

    grafo[origen].push_back(destino);
}
```

- `grafo` es `unordered_map<string, vector<string>>`:
  - La clave es `origen`.
  - El valor es la lista de `destino`s accesibles directamente.
- Emplear vector permite aristas paralelas; si se desea evitar duplicados, usar `unordered_set` en su lugar.

### BFS con límite de escalas

- Uso de `queue<pair<string,int>> cola` para llevar (nodo, nivel) permite rastrear el número de escalas desde el origen.
- `unordered_set<string> visitados` evita volver a procesar nodos.
- Encolamos vecinos solo si `escalas < 3`.

¿Por qué BFS y no DFS? Porque BFS encuentra todos los nodos al menor número de aristas (escalas) primero, lo cual encaja con la restricción por número de escalas.

### Kosaraju (dos DFS)

- La primera DFS registra `orden de finalización`: esto captura una topología parcial útil para procesar SCCs en orden.
- Invertimos todas las aristas para formar el grafo transpuesto. Al DFS sobre el transpuesto en orden inverso de finalización, cada recorrido visita exactamente una SCC.

### Dijkstra y Haversine

- Cada arista tiene peso: la distancia en km entre aeropuertos conectados por ese vuelo.
- Haversine devuelve una buena aproximación de la distancia sobre la superficie de la Tierra (en km) usando lat/lon en radianes.
- `priority_queue<pair<double,string>, vector<pair<double,string>>, greater<pair<double,string>>>` funciona como un min-heap por `double`.
- `unordered_map<string,double> distancias` almacena la mejor distancia conocida a cada nodo; inicializada con `infinito`.
- Al relajar una arista `u->v` con peso `w`: si `dist[u] + w < dist[v]` entonces `dist[v] = dist[u] + w` y `predecesores[v] = u`.

### Filtrado por aerolínea (reto 5)

- Leemos `routes.dat` y verificamos `Airline` (columna 0). Si coincide con la aerolínea solicitada, consideramos la ruta.
- Convertimos las columnas `origin_id` y `dest_id` a cadenas y verificamos que existan en `mapa_aeropuertos`.
- Usamos `unordered_set` para evitar agregar la misma arista repetidamente si el dataset contiene duplicados.

---

## Siguientes pasos y recomendaciones

- Si quieres que elimine por completo cualquier `std::` redundante (es decir, usar `string` en todo el código), puedo aplicar cambios consistentes en todos los `.cpp` y `.h` del proyecto.
- Para datasets grandes, recomiendo usar estructuras más eficientes y considerar almacenar los pesos en una sola estructura en vez de recalcular Haversine repetidamente, o usar un índice espacial si se requiere clustering geográfico.
- Si deseas, puedo también refactorizar más los retos para que cada uno tenga:
  - Función de carga de sus propios datos
  - Funciones puras (sin IO) que realicen los cálculos
  - Funciones de presentación (IO) separadas

---

## Contacto

Si quieres que haga la refactorización completa y automática en todos los archivos (simplificar lógica interna, eliminar redundancias y uniformar uso de `using namespace std;`), dime y lo hago en el siguiente paso. Incluyo ya los pasos planificados como TODOs en la herramienta de sesión.


*** Fin del README ***
