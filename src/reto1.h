#ifndef RETO1_H
#define RETO1_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
// Necesitamos la estructura básica para mapear datos
struct AeropuertoReto1 {
    string id;
    string iata;
    double lat;
    double lon;
    string nombre;
};

void reto1AlcanceBFS(const string& consulta, 
                     const unordered_map<string, AeropuertoReto1>& aeropuertos, 
                     unordered_map<string,vector<string>>& grafo);

#endif