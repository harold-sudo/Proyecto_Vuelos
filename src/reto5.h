#ifndef RETO5_H
#define RETO5_H

#include <string>
#include <unordered_map>

using namespace std;

struct AeropuertoReto5 {
    string id;
    string iata;
    double lat;
    double lon;
    string nombre;
};
void reto5PasajeroLeal(const unordered_map<string, AeropuertoReto5>& aeropuertos,
                       const string& ruta_rutas = "");

#endif