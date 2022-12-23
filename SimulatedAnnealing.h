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
    int cost_function(std::vector<int> path);
    void geometric_cooling(double *temperature, double alpha);
	void Algorithm();
};