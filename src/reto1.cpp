#include "reto1.h"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

void reto1AlcanceBFS(const string& consulta, 
                     const unordered_map<string, AeropuertoReto1>& aeropuertos, 
                     unordered_map<string, vector<string>>& grafo) {
    string id_origen = "";
    string consulta_upper = consulta;
    transform(consulta_upper.begin(), consulta_upper.end(), consulta_upper.begin(), ::toupper);

    // Buscar el aeropuerto por código IATA o coincidencia en el nombre
    for (auto const& [id, aero] : aeropuertos) {
        string nombre_upper = aero.nombre;
        string iata_upper = aero.iata;
        transform(nombre_upper.begin(), nombre_upper.end(), nombre_upper.begin(), ::toupper);
        transform(iata_upper.begin(), iata_upper.end(), iata_upper.begin(), ::toupper);

        if (consulta_upper == iata_upper || nombre_upper.find(consulta_upper) != string::npos) {
            id_origen = id;
            break;
        }
    }

    if (id_origen == "" || grafo.find(id_origen) == grafo.end()) {
        cout << "Aeropuerto no encontrado o no tiene rutas de salida registradas." << endl;
        return;
    }

    // Algoritmo BFS limitado a un máximo de 3 escalas
    unordered_set<string> visitados;
    queue<pair<string, int>> cola; // Guarda: (id_aeropuerto, cantidad_escalas)

    visitados.insert(id_origen);
    cola.push({id_origen, 0});

    while (!cola.empty()) {
        auto [actual, escalas] = cola.front();
        cola.pop();

        // Si aún no pasamos el límite de 3 escalas, podemos explorar sus vecinos directos
        if (escalas < 3) {
            for (const string& vecino : grafo[actual]) {
                if (visitados.find(vecino) == visitados.end()) {
                    visitados.insert(vecino);
                    cola.push({vecino, escalas + 1});
                }
            }
        }
    }

    cout << "\n[RESULTADO RETO 1]" << endl;
    cout << "Desde '" << aeropuertos.at(id_origen).nombre 
         << "' (IATA: " << aeropuertos.at(id_origen).iata << ")," << endl;
    cout << "se pueden alcanzar " << visitados.size() 
         << " aeropuertos distintos con un maximo de 3 escalas." << endl;
}