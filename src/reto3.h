#ifndef RETO3_H
#define RETO3_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
struct AeropuertoReto3 {
    string id;
    string iata;
    double lat;
    double lon;
    string nombre;
};

void reto3DiameTroDijkstra(const unordered_map<string, AeropuertoReto3>& aeropuertos, 
                           unordered_map<string, vector<string>>& grafo);

#endif