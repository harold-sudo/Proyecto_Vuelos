#ifndef RETO1_H
#define RETO1_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
#include "aeropuerto.h"

using AeropuertoReto1 = Aeropuerto;

void reto1AlcanceBFS(const string& consulta, 
                     const unordered_map<string, AeropuertoReto1>& aeropuertos, 
                     unordered_map<string,vector<string>>& grafo);

#endif