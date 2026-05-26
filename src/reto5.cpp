#include "reto5.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "dijkstra.h"

using namespace std;

namespace {

struct DatosConsultaReto5 
{
    string origen;
    string destino;
    string aerolinea;
};

string normalizarTexto(string texto) 
{
    transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char caracter) 
    {
        return static_cast<char>(toupper(caracter));
    });
    return texto;
}

vector<string> obtenerRutasPosibles(const string& ruta_rutas) 
{
    if (!ruta_rutas.empty()) 
    {
        return {ruta_rutas};
    }

    return 
    {
        "routes.dat",
        "data/routes.dat",
        "../data/routes.dat",
        "../routes.dat"
    };
}

DatosConsultaReto5 solicitarDatosReto5() 
{
    DatosConsultaReto5 datos;

    cout << "Ingrese el origen (nombre o codigo IATA): ";
    getline(cin >> ws, datos.origen);

    cout << "Ingrese el destino (nombre o codigo IATA): ";
    getline(cin >> ws, datos.destino);

    cout << "Ingrese el codigo de la aerolinea (ej. 2B): ";
    getline(cin >> ws, datos.aerolinea);

    return datos;
}

const AeropuertoReto5* buscarAeropuertoPorConsulta(
    const string& consulta,
    const unordered_map<string, AeropuertoReto5>& aeropuertos,
    string& id_encontrado) {
    string consulta_normalizada = normalizarTexto(consulta);

    for (const auto& [id_aeropuerto, aeropuerto] : aeropuertos) {
        string nombre_normalizado = normalizarTexto(aeropuerto.nombre);
        string iata_normalizado = normalizarTexto(aeropuerto.iata);

        if (consulta_normalizada == iata_normalizado ||
            nombre_normalizado.find(consulta_normalizada) != string::npos) {
            id_encontrado = id_aeropuerto;
            return &aeropuerto;
        }
    }

    id_encontrado.clear();
    return nullptr;
}

bool abrirArchivoDeRutas(ifstream& archivo, const string& ruta_rutas) 
{
    for (const string& ruta_posible : obtenerRutasPosibles(ruta_rutas)) {
        archivo.open(ruta_posible);
        if (archivo.is_open()) {
            return true;
        }
        archivo.clear();
    }

    return false;
}

unordered_map<string, vector<string>> cargarGrafoDeAerolinea(const string& ruta_rutas,
                                                            const string& codigo_aerolinea,
                                                            const unordered_map<string, AeropuertoReto5>& aeropuertos) 
{
    unordered_map<string, vector<string>> grafo;
    ifstream archivo;

    if (!abrirArchivoDeRutas(archivo, ruta_rutas)) {
        cout << "Error: No se pudo abrir el archivo de rutas para el bonus." << endl;
        return grafo;
    }

    string linea;
    unordered_set<string> rutas_unicas;
    string codigo_aerolinea_normalizado = normalizarTexto(codigo_aerolinea);
    int rutas_validas = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) 
        {
            continue;
        }

        stringstream flujo(linea);
        string codigo_linea, origen_iata, origen_id, destino_iata, destino_id;

        getline(flujo, codigo_linea, ',');
        getline(flujo, origen_iata, ',');
        getline(flujo, origen_id, ',');
        getline(flujo, destino_iata, ',');
        getline(flujo, destino_id, ',');

        if (normalizarTexto(codigo_linea) != codigo_aerolinea_normalizado) 
        {
            continue;
        }

        if (origen_id.empty() || destino_id.empty() || origen_id == "\\N" || destino_id == "\\N") 
        {
            continue;
        }

        if (aeropuertos.find(origen_id) == aeropuertos.end() || aeropuertos.find(destino_id) == aeropuertos.end()) 
        {
            continue;
        }

        string clave_ruta = origen_id + "->" + destino_id;
        if (!rutas_unicas.insert(clave_ruta).second) 
        {
            continue;
        }

        grafo[origen_id].push_back(destino_id);
        rutas_validas++;
    }

    cout << "Cargadas " << rutas_validas << " rutas para la aerolinea " << codigo_aerolinea << "." << endl;
    return grafo;
}

void inicializarDistancias(const unordered_map<string, AeropuertoReto5>& aeropuertos,
                           unordered_map<string, double>& distancias) 
{
    for (const auto& [id_aeropuerto, aeropuerto] : aeropuertos) 
    {
        (void)aeropuerto;
        distancias[id_aeropuerto] = numeric_limits<double>::infinity();
    }
}

// Usaremos la implementación genérica de Dijkstra (dijkstra.cpp). La construcción
// de grafo ponderado para la aerolinea se realiza en evaluarRutaReto5.

void reconstruirYMostrarCamino(const unordered_map<string, string>& predecesores,
                               const string& id_origen,
                               const string& id_destino,
                               const unordered_map<string, AeropuertoReto5>& aeropuertos,
                               double distancia_total,
                               const string& codigo_aerolinea) 
{
    vector<string> camino;
    string nodo_actual = id_destino;

    while (nodo_actual != id_origen) 
    {
        camino.push_back(nodo_actual);
        auto iter_predecesor = predecesores.find(nodo_actual);
        if (iter_predecesor == predecesores.end()) 
        {
            cout << "No fue posible reconstruir el camino mas corto." << endl;
            return;
        }
        nodo_actual = iter_predecesor->second;
    }

    camino.push_back(id_origen);
    reverse(camino.begin(), camino.end());

    cout << "\n[RESULTADO RETO 5]" << endl;
    cout << "Aerolinea utilizada: " << codigo_aerolinea << endl;
    auto it_origen = aeropuertos.find(id_origen);
    auto it_destino = aeropuertos.find(id_destino);
    if (it_origen != aeropuertos.end()) {
        cout << "Origen:  " << it_origen->second.nombre << " (IATA: " << it_origen->second.iata << ")" << endl;
    } else {
        cout << "Origen:  " << id_origen << " (IATA: ?)" << endl;
    }
    if (it_destino != aeropuertos.end()) {
        cout << "Destino: " << it_destino->second.nombre << " (IATA: " << it_destino->second.iata << ")" << endl;
    } else {
        cout << "Destino: " << id_destino << " (IATA: ?)" << endl;
    }
    cout << "Distancia total minima: " << fixed << setprecision(2) << distancia_total << " km" << endl;
    cout << "Camino: ";

    for (size_t indice = 0; indice < camino.size(); ++indice) 
    {
        const string& id_aeropuerto = camino[indice];
        auto it_ap = aeropuertos.find(id_aeropuerto);
        if (it_ap != aeropuertos.end()) {
            cout << it_ap->second.iata;
        } else {
            cout << id_aeropuerto;
        }

        if (indice + 1 < camino.size()) 
        {
            cout << " -> ";
        }
    }

    cout << endl;
}

void evaluarRutaReto5(const unordered_map<string, AeropuertoReto5>& aeropuertos,
                      const string& ruta_rutas,
                      const string& id_origen,
                      const string& id_destino,
                      const string& codigo_aerolinea) 
{
    if (id_origen == id_destino) 
    {
        cout << "El origen y el destino son el mismo aeropuerto. Distancia total: 0.00 km." << endl;
        return;
    }

    unordered_map<string, vector<string>> grafo_aerolinea = cargarGrafoDeAerolinea(ruta_rutas, codigo_aerolinea, aeropuertos);
    if (grafo_aerolinea.empty()) 
    
    {
        cout << "No se encontraron rutas validas para la aerolinea indicada." << endl;
        return;
    }

   
    if (grafo_aerolinea.find(id_origen) == grafo_aerolinea.end()) 
    {
        cout << "El aeropuerto de origen no tiene rutas de salida para esa aerolinea." << endl;
        return;
    }

    unordered_map<string, double> distancias;
    unordered_map<string, string> predecesores;

    inicializarDistancias(aeropuertos, distancias);

    // Construir grafo ponderado a partir del grafo de la aerolinea
    GrafoPonderado grafo_ponderado;
    grafo_ponderado.reserve(grafo_aerolinea.size());
    for (const auto& [origen, vecinos] : grafo_aerolinea) 
    {
        for (const string& destino_id : vecinos) 
        {
            auto itOrig = aeropuertos.find(origen);
            auto itDest = aeropuertos.find(destino_id);
            if (itOrig == aeropuertos.end() || itDest == aeropuertos.end()) continue;
            double peso = calcularDistanciaHaversine(itOrig->second.lat, itOrig->second.lon, itDest->second.lat, itDest->second.lon);
            grafo_ponderado[origen].push_back({destino_id, peso});
        }
    }

    dijkstra(grafo_ponderado, id_origen, distancias, &predecesores);

    if (distancias[id_destino] == numeric_limits<double>::infinity()) 
    {
        cout << "No existe un camino valido entre ambos aeropuertos usando solo la aerolinea indicada." << endl;
        return;
    }

    reconstruirYMostrarCamino(predecesores, id_origen, id_destino, aeropuertos, distancias[id_destino], codigo_aerolinea);
}

} // namespace

void reto5PasajeroLeal(const unordered_map<string, AeropuertoReto5>& aeropuertos,
                       const string& ruta_rutas) 
{
    if (aeropuertos.empty()) 
    {
        cout << "No hay aeropuertos cargados para ejecutar el reto 5." << endl;
        return;
    }

    DatosConsultaReto5 datos_consulta = solicitarDatosReto5();

    string id_origen;
    string id_destino;

    const AeropuertoReto5* aeropuerto_origen = buscarAeropuertoPorConsulta(datos_consulta.origen, aeropuertos, id_origen);
    const AeropuertoReto5* aeropuerto_destino = buscarAeropuertoPorConsulta(datos_consulta.destino, aeropuertos, id_destino);

    if (aeropuerto_origen == nullptr) 
    {
        cout << "Aeropuerto de origen no encontrado." << endl;
        return;
    }

    if (aeropuerto_destino == nullptr) 
    {
        cout << "Aeropuerto de destino no encontrado." << endl;
        return;
    }

    if (id_origen == id_destino) 
    {
        cout << "El origen y el destino son el mismo aeropuerto. Distancia total: 0.00 km." << endl;
        return;
    }

    evaluarRutaReto5(aeropuertos, ruta_rutas, id_origen, id_destino, datos_consulta.aerolinea);
}