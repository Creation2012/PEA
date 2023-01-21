#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <random>

class AntColony{
    private:
        std::vector<std::vector<int>> adjMatrix;
        int v;

    public:
        AntColony(std::vector<std::vector<int>> adjMatrix, int v);

        int Algorithm();
};