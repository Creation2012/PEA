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
    int method;
    int alpha;
    int beta;
    int iterations;

    AntColony(std::vector<std::vector<int>> adjMatrix, const int numberOfCities, const int method, const double alpha, const double beta, const int iterations);
    int Algorithm(int optimal_solution);
    void init_pheromones();
    void refresh_pheromones();
};
