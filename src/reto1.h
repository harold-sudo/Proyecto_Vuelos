#ifndef RETO1_H
#define RETO1_H

#include <string>
#include <unordered_map>
#include <vector>

// Necesitamos la estructura básica para mapear datos
struct AeropuertoReto1 {
    std::string id;
    std::string iata;
    double lat;
    double lon;
    std::string nombre;
};

void reto1AlcanceBFS(const std::string& consulta, 
                     const std::unordered_map<std::string, AeropuertoReto1>& aeropuertos, 
                     std::unordered_map<std::string, std::vector<std::string>>& grafo);

#endif