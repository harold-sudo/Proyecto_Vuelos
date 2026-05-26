#ifndef AEROPUERTO_H
#define AEROPUERTO_H

#include <string>
#include <cmath>

using namespace std;

struct Aeropuerto {
    string id;
    string iata;
    double lat;
    double lon;
    string nombre;
};

// Calcula distancia en km entre dos coordenadas (Haversine)
inline double calcularDistanciaHaversine(double lat1, double lon1, double lat2, double lon2) {
    constexpr double RADIO_TERRESTRE = 6371.0;
    constexpr double PI = 3.14159265358979323846;

    auto gradosARadianes = [&](double grados) { return grados * PI / 180.0; };

    double lat1_rad = gradosARadianes(lat1);
    double lon1_rad = gradosARadianes(lon1);
    double lat2_rad = gradosARadianes(lat2);
    double lon2_rad = gradosARadianes(lon2);

    double dlon = lon2_rad - lon1_rad;
    double dlat = lat2_rad - lat1_rad;

    double haversine = pow(sin(dlat / 2.0), 2) + cos(lat1_rad) * cos(lat2_rad) * pow(sin(dlon / 2.0), 2);
    double centralAngle = 2.0 * atan2(sqrt(haversine), sqrt(1.0 - haversine));

    return RADIO_TERRESTRE * centralAngle;
}


inline double calcularHaversine(double lat1, double lon1, double lat2, double lon2) { return calcularDistanciaHaversine(lat1, lon1, lat2, lon2); }

#endif
