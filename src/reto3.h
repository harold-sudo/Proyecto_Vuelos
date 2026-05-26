#ifndef RETO3_H
#define RETO3_H

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
#include "aeropuerto.h"

using AeropuertoReto3 = Aeropuerto;

void reto3DiameTroDijkstra(const unordered_map<string, AeropuertoReto3>& aeropuertos, 
                           unordered_map<string, vector<string>>& grafo);

#endif