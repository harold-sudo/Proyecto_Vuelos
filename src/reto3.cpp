#include "reto3.h"
#include <iostream>
#include <queue>
#include <cmath>
#include <iomanip>
#include "dijkstra.h"

using namespace std;

namespace {

using GrafoPonderado = unordered_map<string, vector<pair<string, double>>>;

void construirGrafoPonderado(const unordered_map<string, AeropuertoReto3>& aeropuertos,
                             const unordered_map<string, vector<string>>& grafo,
                             GrafoPonderado& grafo_ponderado) 
{
    grafo_ponderado.clear();
    grafo_ponderado.reserve(grafo.size());

    for (const auto& [id_origen, destinos_directos] : grafo) {
        for (const string& id_destino : destinos_directos) {
            auto iteradorOrigen = aeropuertos.find(id_origen);
            auto iteradorDestino = aeropuertos.find(id_destino);
            if (iteradorOrigen == aeropuertos.end() || iteradorDestino == aeropuertos.end()) {
                continue;
            }

            double peso = calcularHaversine(iteradorOrigen->second.lat,
                                            iteradorOrigen->second.lon,
                                            iteradorDestino->second.lat,
                                            iteradorDestino->second.lon);
            grafo_ponderado[id_origen].push_back({id_destino, peso});
        }
    }
}

void inicializarDistancias(const unordered_map<string, AeropuertoReto3>& aeropuertos,
                           unordered_map<string, double>& distancias) 
{
    distancias.clear();
    distancias.reserve(aeropuertos.size());

    for (const auto& [id,_] :aeropuertos) 
    {
        distancias[id] = 1e9;
    }
}

// Usamos implementación genérica en dijkstra.h / dijkstra.cpp

void actualizarDiametro(const string& id_origen,
                        const unordered_map<string, double>& distancias,
                        double& max_distancia_total,
                        string& id_origen_lejano,
                        string& id_destino_lejano) 
{
    for (const auto& [id_destino, distancia] : distancias) 
    {
        if (distancia < 1e8 && distancia > max_distancia_total && id_origen != id_destino) 
        {
            max_distancia_total = distancia;
            id_origen_lejano = id_origen;
            id_destino_lejano = id_destino;
        }
    }
}

void mostrarResultadoReto3(const unordered_map<string, AeropuertoReto3>& aeropuertos,
                           const string& id_origen_lejano,
                           const string& id_destino_lejano,
                           double max_distancia_total) 
{
    cout << "\n[RESULTADO RETO 3]" << endl;
    if (id_origen_lejano != "") 
    {
        cout << "Par de aeropuertos que definen el Diametro Efectivo del Grafo:" << endl;
        cout << "  Origen:  " << aeropuertos.at(id_origen_lejano).nombre << " (ID: " << id_origen_lejano << ")" << endl;
        cout << "  Destino: " << aeropuertos.at(id_destino_lejano).nombre << " (ID: " << id_destino_lejano << ")" << endl;
        cout << "  Distancia de la ruta mas optima: " << fixed << setprecision(2) << max_distancia_total << " km" << endl;
    } 
    else 
    {
        cout << "No se pudieron calcular rutas validas." << endl;
    }
}

} // namespace

void reto3DiameTroDijkstra(const unordered_map<string, AeropuertoReto3>& aeropuertos, unordered_map<string, vector<string>>& grafo) 
{
    double max_distancia_total = 0.0;
    string id_origen_lejano = "", id_destino_lejano = "";

    cout << "Calculando distancias minimas (Dijkstra multi-origen)." << endl;
    int procesados = 0;

    GrafoPonderado grafo_ponderado;
    construirGrafoPonderado(aeropuertos, grafo, grafo_ponderado);

    for (auto const& [id_origen, destinos_directos] : grafo) 
    {
        if (destinos_directos.empty()) continue;

        unordered_map<string, double> distancias;
        inicializarDistancias(aeropuertos, distancias);
        dijkstra(grafo_ponderado, id_origen, distancias, nullptr);

        actualizarDiametro(id_origen, distancias, max_distancia_total, id_origen_lejano, id_destino_lejano);

        procesados++;
        if (procesados % 500 == 0) 
        {
            cout << "  Progreso: " << procesados << " aeropuertos analizados como origen." << endl;
        }
    }

    mostrarResultadoReto3(aeropuertos, id_origen_lejano, id_destino_lejano, max_distancia_total);
}