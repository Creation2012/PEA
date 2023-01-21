#include "BruteForce.h"

void BruteForce::Algorithm(std::vector<std::vector<int>> adjMatrix, int v)
{
	// Wstępna inicjalizacja zmiennych
	int minPath = INT_MAX;
	std::vector<int> path;
	std::vector<int> shortestPath; 
	std::vector<int> vArray;

	for (int i = 1; i < v; i++) {
		// Wektor kolejnych wierzchołków do permutacji
		vArray.push_back(i);
	}

	do {
		// Reset kosztu scieżki, oraz dodanie do nowej ścieżki wierzchołka startowego
		int pathValue = 0;
		path.push_back(0);
		int k = 0;
		for (int i = 0; i < vArray.size(); i++) {
			// Wykluczenie wartości -1 tzw. odwołania wierzchołka do samego siebie oraz braku ścieżki, reprezentowane wartości 0
			if (adjMatrix[k][vArray[i]] > 0) {
				pathValue += adjMatrix[k][vArray[i]];
				path.push_back(vArray[i]);
				k = vArray[i];
			}
		}

		// Zamknięcie cyklu, dodanie wierzchołka startowego, do kosztu drogi i samej drogi
		pathValue += adjMatrix[k][0];
		path.push_back(0);

		// Szukanie najkrótszej ścieżki
		if (minPath > pathValue) {
			// Przypisanie najkrótszej ścieżki i drogi wierzchołków
			minPath = pathValue;
			shortestPath = path;
		}

		// Restart drogi
		path.clear();

	// Funkcja wykonująca kolejne permutacje danego wektora
	} while (next_permutation(vArray.begin(), vArray.end()));

	return;
}