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

class Node {
	public:
	int index;
	int x;
	int y;

	public:
	Node();
	void setup_node(int index, int x, int y);
	int getX();
	int getY();
};

Node::Node() {
	this->index = 0;
	this->x = 0;
	this->y = 0;
}

int length_between_nodes(Node a, Node b);

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
	{
		printf("Problem with file - file is opened\n");
		return false;
	}

	dataFile >> this->v;
	printf("current size: %i\n",this->v);
	

	// node version
	//if(this->v == -1) {
	//	std::vector<Node> nodes;
	//	dataFile >> this->v;
	//	this->adjMatrix.resize(this->v);
	//	nodes.resize(this->v);
	//	for(int i=0; i<this->v; i++) {
	//		this->adjMatrix[i] = std::vector<int>(this->v);
	//	}
	//	int index;
	//	int x;
	//	int y;
	//	for(int i=0; i<this->v; i++) {
	//		dataFile >> index;
	//		dataFile >> x;
	//		dataFile >> y;
	//		nodes[i].setup_node(index-1,x,y);
	//	}

	//	for(int i=0; i<this->v; i++) {
	//		for(int j=0; j<this->v; j++) {
	//			this->adjMatrix[i][j] = length_between_nodes(nodes[i], nodes[j]);
	//		}
	//	}
	//}
	//else {
		this->adjMatrix.resize(this->v);

		for (int i = 0; i < this->v; i++) {
			this->adjMatrix[i] = std::vector<int>(this->v);
			for (int j = 0; j < this->v; j++) {
				dataFile >> temp;
				this->addEdge(i,j,temp);
			}
		}
	//}

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

void GraphMatrix::testbench(int repeat_test, int method, double alpha, double beta, int iterations, int optimal_solution, std::string header, std::string output_name) 
{
	std::ofstream file(output_name, std::ios::app);
	double count = 0;
	double avg = 0;
	int path_cost;
	int* path = new int[this->v];

	file << header << "\n";
	for (int i = 0; i < repeat_test; i++) {
		printf("Iteration: %d\n",i);

		AntColony *solution= new AntColony(adjMatrix, this->v, method, alpha, beta, iterations);
		StartCounter();
		path_cost = solution->Algorithm(optimal_solution);
		count += GetCounter();

		if(path_cost == 0) {
			std::cout << "Path error!";
			return;
		}

		file << count << ";" << path_cost << ";\n";
		printf("Result: %i\n", path_cost);
		count = 0;
		delete solution;
	}

	delete path;

	file.close();
}

//Node::Node(int index, int x, int y)
//{
//	this->index = index;
//	this->x = x;
//	this->y = y;
//}

void Node::setup_node(int index, int x, int y)
{
	this->index = index;
	this->x = x;
	this->y = y;
}

int Node::getX()
{
	return this->x;
}

int Node::getY()
{
	return this->y;
}

int length_between_nodes(Node a, Node b) {
	int length;
	if(a.index == b.index) {
		return -1;
	}
	length = pow(abs(a.getX() - b.getX()),2) + pow(abs(a.getY() - b.getY()),2);
	return length;
}
