#include "reto1.h"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

namespace {

void normalizarTexto(string& texto) 
{
    transform(texto.begin(), texto.end(), texto.begin(), ::toupper);
}

void buscarIdOrigen(const string& consulta,
                    const unordered_map<string, AeropuertoReto1>& aeropuertos,
                    string& id_origen) {
    string consulta_upper = consulta;
    normalizarTexto(consulta_upper);
    id_origen.clear();

    for (const auto& [id, aeropuerto] : aeropuertos) {
        string nombre_upper = aeropuerto.nombre;
        string iata_upper = aeropuerto.iata;
        normalizarTexto(nombre_upper);
        normalizarTexto(iata_upper);

        if (consulta_upper == iata_upper || nombre_upper.find(consulta_upper) != string::npos) {
            id_origen = id;
            return;
        }
    }
}

void ejecutarBfsConLimite(const string& id_origen,
                          const unordered_map<string, vector<string>>& grafo,
                          unordered_set<string>& visitados) {
    queue<pair<string, int>> cola;

    visitados.clear();
    visitados.insert(id_origen);
    cola.push({id_origen, 0});

    while (!cola.empty()) {
        auto [actual, escalas] = cola.front();
        cola.pop();

        if (escalas >= 3) {
            continue;
        }

        for (const string& vecino : grafo.at(actual)) {
            if (visitados.insert(vecino).second) {
                cola.push({vecino, escalas + 1});
            }
        }
    }
}

void mostrarResultadoReto1(const unordered_map<string, AeropuertoReto1>& aeropuertos,
                           const string& id_origen,
                           size_t cantidad_visitados) {
    cout << "\n[RESULTADO RETO 1]" << endl;
    cout << "Desde '" << aeropuertos.at(id_origen).nombre
         << "' (IATA: " << aeropuertos.at(id_origen).iata << ")," << endl;
    cout << "se pueden alcanzar " << cantidad_visitados
         << " aeropuertos distintos con un maximo de 3 escalas." << endl;
}

} // namespace

void reto1AlcanceBFS(const string& consulta, 
                     const unordered_map<string, AeropuertoReto1>& aeropuertos, 
                     unordered_map<string, vector<string>>& grafo) 
{
    string id_origen;
    buscarIdOrigen(consulta, aeropuertos, id_origen);

    if (id_origen == "" || grafo.find(id_origen) == grafo.end()) 
    {
        cout << "Aeropuerto no encontrado o no tiene rutas de salida registradas." << endl;
        return;
    }

    unordered_set<string> visitados;
    ejecutarBfsConLimite(id_origen, grafo, visitados);
    mostrarResultadoReto1(aeropuertos, id_origen, visitados.size());
}