#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <random>
#include <stdlib.h>
#include <time.h>

class AntColony{
    public:
        std::vector<Ant> ants;

        AntColony(std::vector<std::vector<int>> adjMatrix, const int numberOfCities);
        int Algorithm();
        void init_pheromones();
        void refresh_pheromones();
};
