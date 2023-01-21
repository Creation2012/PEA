#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <random>

class SimulatedAnnealing{
private:
    std::vector<std::vector<int>> adjMatrix;
    int v;

public:
    SimulatedAnnealing(std::vector<std::vector<int>> adjMatrix, int v);

    std::vector<int> gen_first_path();
    std::vector<int> gen_random_path();
    std::vector<int> gen_greedy_path();

    int cost_function(std::vector<int> path);

    void boltzmann_cooling(double *temperature, double alpha, double b, int k);
    void cauchy_cooling(double *temperature, double alpha, double b, int k);
    void geometric_cooling(double *temperature, double alpha, int k);

    int longest_edge();
	int Algorithm(int path_method, double temperature, double temperature_final, double alpha, int epoka, int neighbourhood_type, int cooling_method);
};