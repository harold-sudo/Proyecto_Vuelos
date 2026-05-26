#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

// Incluimos las cabeceras de nuestros 3 retos individuales
#include "reto1.h"
#include "reto2.h"
#include "reto3.h"
#include "reto5.h"
#include "aeropuerto.h"

using namespace std;

// Usamos 'Aeropuerto' definido en aeropuerto.h

unordered_map<string, Aeropuerto> cargarAeropuertos(const string& ruta) 
{
    unordered_map<string, Aeropuerto> mapa;
    ifstream archivo(ruta);
    string linea;

    if (!archivo.is_open()) 
    {
        cout << "Error: No se pudo abrir el archivo " << ruta << endl;
        return mapa;
    }

    while (getline(archivo, linea)) 
    {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string id, iata, s_lat, s_lon, nombre;

        getline(ss, id, ',');
        getline(ss, iata, ',');
        getline(ss, s_lat, ',');
        getline(ss, s_lon, ',');
        getline(ss, nombre, '\n');

        Aeropuerto aero = {id, iata, stod(s_lat), stod(s_lon), nombre};
        mapa[id] = aero;
    }
    archivo.close();
    cout << "Cargados " << mapa.size() << " aeropuertos desde los datos limpios." << endl;
    return mapa;
}

unordered_map<string, vector<string>> cargarGrafo(const string& ruta) 
{
    unordered_map<string, vector<string>> grafo;
    ifstream archivo(ruta);
    string linea;

    if (!archivo.is_open()) 
    {
        cout << "Error: No se pudo abrir el archivo " << ruta << endl;
        return grafo;
    }

    int conteo = 0;
    while (getline(archivo, linea)) 
    {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string origen, destino;

        getline(ss, origen, ',');
        getline(ss, destino, '\n');

        grafo[origen].push_back(destino);
        conteo++;
    }
    archivo.close();
    cout << "Cargadas " << conteo << " rutas en el grafo." << endl;
    return grafo;
}

int main() 
{
    // Cargar estructuras principales
    unordered_map<string, Aeropuerto> mapa_base = cargarAeropuertos("airports_clean.dat");
    unordered_map<string, vector<string>> grafo = cargarGrafo("routes_clean.dat");

    if (mapa_base.empty() || grafo.empty()) 
    {
        cout << "Error critico: Verifica la existencia de 'airports_clean.dat' y 'routes_clean.dat'." << endl;
        return 1;
    }

    // Adaptar estructuras base a los tipos esperados por los retos modulares
    unordered_map<string, AeropuertoReto1> mapa_r1;
    unordered_map<string, AeropuertoReto3> mapa_r3;
    unordered_map<string, AeropuertoReto5> mapa_r5;
    vector<string> todos_los_ids;

    for (auto const& [id, a] : mapa_base) 
    {
        mapa_r1[id] = {a.id, a.iata, a.lat, a.lon, a.nombre};
        mapa_r3[id] = {a.id, a.iata, a.lat, a.lon, a.nombre};
        mapa_r5[id] = {a.id, a.iata, a.lat, a.lon, a.nombre};
        todos_los_ids.push_back(id);
    }

    int opcion;
    do {
        cout << "\n=========================================" << endl;
        cout << "   SISTEMA DE ANALISIS DE RUTAS (C++)    " << endl;
        cout << "=========================================" << endl;
        cout << "1. Reto 1: Alcance Personalizado (BFS)" << endl;
        cout << "2. Reto 2: Grupos y Aislamiento Aereo (Kosaraju)" << endl;
        cout << "3. Reto 3: Maxima Eficiencia / Diametro (Dijkstra)" << endl;
        cout << "4. Reto 5: El Pasajero Leal (una sola aerolinea)" << endl;
        cout << "5. Salir del programa" << endl;
        cout << "Seleccione una opcion (1-5): ";
        cin >> opcion;

        if (opcion == 1) 
        {
            string consulta;
            cout << "Ingrese codigo IATA o nombre del aeropuerto: ";
            cin.ignore();
            getline(cin, consulta);
            reto1AlcanceBFS(consulta, mapa_r1, grafo);
        } 
        else if (opcion == 2) {
            reto2Kosaraju(todos_los_ids, grafo);
        } 
        else if (opcion == 3) {
            reto3DiameTroDijkstra(mapa_r3, grafo);
        }
        else if (opcion == 4) {
            
            reto5PasajeroLeal(mapa_r5, "");
        }
    } while (opcion != 5);

    cout << "\n¡Programa finalizado correctamente!" << endl;
    return 0;
}