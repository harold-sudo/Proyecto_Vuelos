#ifndef RETO5_H
#define RETO5_H

#include <string>
#include <unordered_map>

using namespace std;
#include "aeropuerto.h"

using AeropuertoReto5 = Aeropuerto;
void reto5PasajeroLeal(const unordered_map<string, AeropuertoReto5>& aeropuertos,
                       const string& ruta_rutas = "");

#endif