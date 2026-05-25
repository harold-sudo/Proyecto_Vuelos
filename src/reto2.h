#ifndef RETO2_H
#define RETO2_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
void reto2Kosaraju(const vector<string>& todos_los_ids, 
                   unordered_map<string,vector<string>>& grafo);

#endif