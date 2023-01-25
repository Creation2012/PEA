#include <iostream>
#include "GraphMatrix.h"
#include "HeldKarp.h"
#include "SimulatedAnnealing.h"
#include "AntColony.h"
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <windows.h>
#include <map>
#include <set>
#include <cmath>

GraphMatrix::GraphMatrix()
{
	this->v = 0;
	this->adjMatrix.resize(0);
}

GraphMatrix::GraphMatrix(int v)
{
	this->v = v;
	this->adjMatrix.resize(v);
	for (int i = 0; i < v; i++) {
		this->adjMatrix[i] = std::vector<int>(v);
		for (int j = 0; j < v; j++) {
			this->adjMatrix[i][j] = 0;
		}
	}
}

GraphMatrix::~GraphMatrix()
{
	this->adjMatrix.~vector();
}

bool GraphMatrix::readFromFile(std::string fileName)
{
	std::fstream dataFile;
	int temp;
	dataFile.open(fileName);
	if (!dataFile.is_open())
		return false;

	dataFile >> this->v;
	this->adjMatrix.resize(this->v);

	for (int i = 0; i < this->v; i++) {
		this->adjMatrix[i] = std::vector<int>(this->v);
		for (int j = 0; j < this->v; j++) {
			dataFile >> temp;
			this->addEdge(i,j,temp);
		}
	}

	dataFile.close();
	return true;
}

// Dodawanie dla grafu nieskierowanego z wagami
void GraphMatrix::addEdge(int i, int j, int value)
{
	adjMatrix[i][j] = value;
}

void GraphMatrix::removeEdge(int i, int j)
{
	adjMatrix[i][j] = 0;
}

void GraphMatrix::displayMatrix()
{
	std::cout << "Macierz sasiedztwa: " << std::endl;
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			std::cout << std::setw(3) << adjMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart);

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

void GraphMatrix::testbench(int algorithm_option, int repeat_test, int optimal_solution, std::string header, std::string output_name) 
{
	std::ofstream file(output_name, std::ios::app);
	double count = 0;
	double avg = 0;
	int path_cost;
	int* path = new int[this->v];

	file << header << "\n";
	for (int i = 0; i < repeat_test; i++) {
		printf("Iteration: %d\n",i);

		AntColony *solution= new AntColony(adjMatrix,v);
		StartCounter();
		path_cost = solution->Algorithm();
		count += GetCounter();

		if(path_cost == 0) {
			std::cout << "Path error!";
			return;
		}

		file << count << ";" << path_cost << "\n";
		count = 0;
		delete solution;
	}

	delete path;

	file.close();
}