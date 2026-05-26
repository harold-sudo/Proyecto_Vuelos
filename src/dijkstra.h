#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
// Grafo ponderado: id -> vector de (vecino, peso)
using GrafoPonderado = unordered_map<string,vector<pair<string, double>>>;

void dijkstra(const GrafoPonderado& grafo,
              const string& origen,
              unordered_map<string, double>& distancias,
              unordered_map<string, string>* predecesores = nullptr);

#endif // DIJKSTRA_H
