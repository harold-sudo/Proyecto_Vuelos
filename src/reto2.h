#ifndef RETO2_H
#define RETO2_H

#include <string>
#include <unordered_map>
#include <vector>

void reto2Kosaraju(const std::vector<std::string>& todos_los_ids, 
                   std::unordered_map<std::string, std::vector<std::string>>& grafo);

#endif