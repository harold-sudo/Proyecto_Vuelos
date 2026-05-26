#include "dijkstra.h"

#include <queue>
#include <utility>

using namespace std;
void dijkstra(const GrafoPonderado& grafo,
              const string& origen,
              unordered_map<string, double>& distancias,
              unordered_map<string,string>* predecesores) 
{
    using NodoDist = pair<double,string>;
    struct Greater 
    {
        bool operator()(const NodoDist& a, const NodoDist& b) const { return a.first > b.first; }
    };

    priority_queue<NodoDist, vector<NodoDist>, Greater> pq;

    distancias[origen] = 0.0;
    pq.push({0.0, origen});

    while (!pq.empty()) 
    {
        auto [dist_actual, nodo] = pq.top();
        pq.pop();

        if (dist_actual > distancias[nodo]) continue;

        auto it = grafo.find(nodo);
        if (it == grafo.end()) continue;

        for (const auto& [vecino, peso] : it->second) 
        {
            double posible = dist_actual + peso;
            if (posible < distancias[vecino]) 
            {
                distancias[vecino] = posible;
                if (predecesores) 
                {
                    (*predecesores)[vecino] = nodo;
                }
                pq.push({posible, vecino});
            }
        }
    }
}
