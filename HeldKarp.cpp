#include "HeldKarp.h"

void HeldKarp::Algorithm(std::vector <std::vector<int>> adjMatrix, int v)
{
	unsigned long int vertices = v;
	unsigned long int n = 1 << (vertices - 1);
	unsigned long int minPath = INT_MAX;
	std::vector<std::vector<unsigned long int>> minArray(n, std::vector<unsigned long int>(vertices, (static_cast<unsigned long int>(INT_MAX))));
	std::vector<unsigned long int> path(vertices + 2, 0);
	//std::vector<unsigned long int> tempPath;	

	// Zgodnie z teoretyczną złożonością O(n^2 * 2^n)
	for (unsigned long int i = 1; i < n; i++)
	{
		for (unsigned long int j = 0; j < vertices - 1; j++)
		{
			int p = 1 << j; // wyliczanie kolejnych setów

			if (i && p)
			{ 
				if (i == p) // tworzenie setów podstawowych [2, {}], gdzie {} - pusty set
				{
					minArray[i][j] = adjMatrix[vertices - 1][j];
					//path.push_back
				}
				else		// tworzenie kombinacji pozostałych setów np. [2, {1,3}] -> przechodzący przez 0 i/lub 1, 3 i na końcu 2
				{
					unsigned long int preVert = i ^ p; 
					for (unsigned long int o = 0; o < vertices - 1; o++)
					{
						if (preVert & 1 << o){
							//int pathValue = adjMatrix[o][j] + minArray[preVert][o];
							//if (pathValue < minArray[i][j])
							//	minArray[i][j] = pathValue;
							minArray[i][j] = std::min(minArray[i][j], adjMatrix[j][o] + minArray[preVert][o]);
							path[i] = preVert;
						} 
					}
				}
			}
		}
	}

	unsigned long int eq;
	//unsigned long int path;
	unsigned long int check;

	// stworzenie poprawego cyklu hamiltona - powrót do wierzchołka startowego
	for (unsigned long int i = 0; i < vertices - 1; i++)
	{
		unsigned long int preVert = 1 << ((vertices-1 )- 1);
		eq = adjMatrix[i][vertices - 1] + minArray[preVert][i];
		check = minPath;		
		minPath = std::min(minPath, eq);
		if(minPath != check){
			//path = preVert;
		}
	}

	return;
}