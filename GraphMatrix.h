#pragma once
#include <string>
#include <vector>

class GraphMatrix {
private:
	int v;
	int* path;

public:
	std::vector<std::vector<int>> adjMatrix;
	GraphMatrix();
	GraphMatrix(int v);
	~GraphMatrix();
	bool readFromFile(std::string fileName);
	void addEdge(int i, int j, int value);
	void removeEdge(int i, int j);
	void displayMatrix();
    void tspBruteForce();
    void testbench(int repeat_test, int method, double alpha, double beta, int iterations, int optimal_solution, std::string header, std::string output_name);
};
