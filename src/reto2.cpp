#include "reto2.h"
#include <iostream>
#include <unordered_set>

using namespace std;

// Función auxiliar: Primer paso de Kosaraju (DFS para guardar orden de finalización)
void dfsOrdenFinalizacion(const string& nodo, unordered_map<string, vector<string>>& grafo, unordered_set<string>& visitados, vector<string>& pila) 
{
    visitados.insert(nodo);
    for (const string& vecino : grafo[nodo]) {
        if (visitados.find(vecino) == visitados.end()) {
            dfsOrdenFinalizacion(vecino, grafo, visitados, pila);
        }
    }
    pila.push_back(nodo); // Al terminar de explorar, va a la pila
}

// Función auxiliar: Segundo paso de Kosaraju (DFS sobre el grafo invertido)
void dfsSCC(const string& nodo, unordered_map<string, vector<string>>& grafo_transpuesto, unordered_set<string>& visitados, int& tamano_componente) 
{
    visitados.insert(nodo);
    tamano_componente++;
    for (const string& vecino : grafo_transpuesto[nodo]) {
        if (visitados.find(vecino) == visitados.end()) {
            dfsSCC(vecino, grafo_transpuesto, visitados, tamano_componente);
        }
    }
}

void reto2Kosaraju(const vector<string>& todos_los_ids, unordered_map<string, vector<string>>& grafo) 
{
    vector<string> pila;
    unordered_set<string> visitados;

    //  Ejecutar DFS en todos los nodos para obtener el orden de finalización
    for (const string& id : todos_los_ids) 
    {
        if (visitados.find(id) == visitados.end()) 
        {
            dfsOrdenFinalizacion(id, grafo, visitados, pila);
        }
    }

    //  Construir el Grafo Transpuesto (Invertir la dirección de las aristas)
    unordered_map<string, vector<string>> grafo_transpuesto;
    for (auto const& [origen, destinos] : grafo) 
    {
        for (const string& destino : destinos) 
        {
            grafo_transpuesto[destino].push_back(origen);
        }
    }

    //  Procesar los nodos en el orden inverso determinado por la pila
    visitados.clear();
    int total_scc = 0;
    int tamano_maximo = 0;

    while (!pila.empty()) 
    {
        string nodo = pila.back();
        pila.pop_back();

        if (visitados.find(nodo) == visitados.end()) 
        {
            int tamano_actual = 0;
            dfsSCC(nodo, grafo_transpuesto, visitados, tamano_actual);
            total_scc++;
            if (tamano_actual > tamano_maximo) 
            {
                tamano_maximo = tamano_actual;
            }
        }
    }

    cout << "\n[RESULTADO RETO 2]" << endl;
    cout << "Cantidad total de componentes fuertemente conexas (Grupos aislados): " << total_scc << endl;
    cout << "Tamanio del grupo de conectividad mas grande: " << tamano_maximo << " aeropuertos." << endl;
}