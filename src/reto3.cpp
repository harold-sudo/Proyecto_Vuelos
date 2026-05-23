#include "reto3.h"
#include <iostream>
#include <queue>
#include <cmath>
#include <iomanip>

using namespace std;

// Constantes matemáticas internas para Haversine
const double RADIO_TERRESTRE = 6371.0;
const double PI = 3.14159265358979323846;

double gradosARadianes(double grados) {
    return grados * PI / 180.0;
}

double calcularHaversine(double lat1, double lon1, double lat2, double lon2) {
    double lat1_rad = gradosARadianes(lat1);
    double lon1_rad = gradosARadianes(lon1);
    double lat2_rad = gradosARadianes(lat2);
    double lon2_rad = gradosARadianes(lon2);

    double dlon = lon2_rad - lon1_rad;
    double dlat = lat2_rad - lat1_rad;

    double a = pow(sin(dlat / 2.0), 2) + cos(lat1_rad) * cos(lat2_rad) * pow(sin(dlon / 2.0), 2);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return RADIO_TERRESTRE * c;
}

void reto3DiameTroDijkstra(const unordered_map<string, AeropuertoReto3>& aeropuertos, unordered_map<string, vector<string>>& grafo) {
    double max_distancia_total = 0.0;
    string id_origen_lejano = "", id_destino_lejano = "";

    cout << "Calculando distancias minimas (Dijkstra multi-origen). Esto puede demorar un momento..." << endl;
    int procesados = 0;

    for (auto const& [id_origen, destinos_directos] : grafo) {
        if (destinos_directos.empty()) continue;

        // Min-Heap para Dijkstra: almacena pares de (distancia_acumulada, id_nodo)
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
        unordered_map<string, double> distancias;

        // Inicializar todas las distancias en un valor infinito simulado
        for (auto const& [id, aero] : aeropuertos) {
            distancias[id] = 1e9;
        }

        distancias[id_origen] = 0.0;
        pq.push({0.0, id_origen});

        while (!pq.empty()) {
            auto [dist_actual, u] = pq.top();
            pq.pop();

            if (dist_actual > distancias[u]) continue;

            for (const string& v : grafo[u]) {
                if (aeropuertos.find(u) != aeropuertos.end() && aeropuertos.find(v) != aeropuertos.end()) {
                    auto const& aeroA = aeropuertos.at(u);
                    auto const& aeroB = aeropuertos.at(v);
                    
                    // Peso dinámico calculado mediante Haversine
                    double peso = calcularHaversine(aeroA.lat, aeroA.lon, aeroB.lat, aeroB.lon);

                    if (distancias[u] + peso < distancias[v]) {
                        distancias[v] = distancias[u] + peso;
                        pq.push({distancias[v], v});
                    }
                }
            }
        }

        // Buscar el camino mínimo más largo desde este origen actual
        for (auto const& [id_destino, dist] : distancias) {
            if (dist < 1e8 && dist > max_distancia_total && id_origen != id_destino) {
                max_distancia_total = dist;
                id_origen_lejano = id_origen;
                id_destino_lejano = id_destino;
            }
        }

        procesados++;
        if (procesados % 500 == 0) {
            cout << "  Progreso: " << procesados << " aeropuertos analizados como origen." << endl;
        }
    }

    cout << "\n[RESULTADO RETO 3]" << endl;
    if (id_origen_lejano != "") {
        cout << "Par de aeropuertos que definen el Diametro Efectivo del Grafo:" << endl;
        cout << "  Origen:  " << aeropuertos.at(id_origen_lejano).nombre << " (ID: " << id_origen_lejano << ")" << endl;
        cout << "  Destino: " << aeropuertos.at(id_destino_lejano).nombre << " (ID: " << id_destino_lejano << ")" << endl;
        cout << "  Distancia de la ruta mas optima: " << fixed << setprecision(2) << max_distancia_total << " km" << endl;
    } else {
        cout << "No se pudieron calcular rutas validas." << endl;
    }
}