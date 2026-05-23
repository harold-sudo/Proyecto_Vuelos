#ifndef RETO3_H
#define RETO3_H

#include <string>
#include <unordered_map>
#include <vector>

struct AeropuertoReto3 {
    std::string id;
    std::string iata;
    double lat;
    double lon;
    std::string nombre;
};

void reto3DiameTroDijkstra(const std::unordered_map<std::string, AeropuertoReto3>& aeropuertos, 
                           std::unordered_map<std::string, std::vector<std::string>>& grafo);

#endif