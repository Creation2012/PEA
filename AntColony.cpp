#include "AntColony.h"

const double ALPHA = 1;      // pheromone importance
const double BETA = 5;       // distance importance
const double RHO = 0.5;      // pheromone evaporation rate
double Q = 100;        // pheromone deposit rate
const double INIT_PHER = 0.01; // initial pheromone level
const int MAX_ITER = 1000;   // maximum number of iterations
const double BETA_Q  = 0.01;
const double Q_0 = 0.9;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0,1);

std::vector<std::vector<double>> pheromones;
std::vector<std::vector<int>> graph;
int numberOfCities;

class Ant {
public:
    std::vector<int> tour;
        double tourLength;
        std::vector<int> visited;

        Ant();

        int nextCity(int currCity);

        double desirability(int i, int j);
};

AntColony::AntColony(std::vector<std::vector<int>> adjMatrix, int v)
{
    copy(adjMatrix.begin(),adjMatrix.end(),back_inserter(graph));
    numberOfCities = v;
    init_pheromones();
}

/*
    ANT COLONY OPTIMALIZATION

    dopóki kryterium zatrzymania nie wystąpiło powtarzaj
    1. wylosuj dla każdej mrówki losowe miasto początkowe
    2. na podstawie lokalnej ilości feromonu i pewnej heurystyki wybierz kolejną krawędź
    3. po osiągnięciu celu uaktualnij ilość feromonu wg określonych (dla danego algorytmu) zasad
    
*/

int AntColony::Algorithm()
{
    int method = 1;
    srand(time(NULL));
    std::vector<Ant> ants;
    ants.resize(numberOfCities);
    // ending 
bool done = false;
//while (!done) {
for(int iter = 0; iter<MAX_ITER; iter++) {
    // ant beggining
    for(int i=0; i<numberOfCities; i++) 
    {
        ants[i].tour.push_back(rand() % numberOfCities); 
        ants[i].visited[ants[i].tour.back()] = true;
    }

    // move each ant
    for (int i = 0; i < numberOfCities; i++) {
        int currCity = ants[i].tour.back();
        int next = ants[i].nextCity(currCity);
        if(next == -1)
            break;
        ants[i].tour.push_back(next);
        ants[i].visited[next] = true;
        ants[i].tourLength += graph[currCity][next];
    }

    // update the pheromones
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            pheromones[i][j] *= (1.0 - RHO); // pheromone evaporation
        }
    }
    for (int i = 0; i < numberOfCities; i++) {
        int currCity = ants[i].tour[0];
        for (int j = 1; j < ants[i].tour.size(); j++) {
            int nextCity = ants[i].tour[j];
            double deposit = Q / ants[i].tourLength;
            if(method == 1) {
                deposit = Q / ants[i].tourLength;
            } else if(method == 2) {
                deposit = Q_0 / ants[i].tourLength;
            }
            pheromones[currCity][nextCity] += deposit;
            currCity = nextCity;
        }
        // DAS method : adjust the pheromone deposit rate based on tour length
        if(method == 1) {
            Q = Q / (1 + BETA_Q * ants[i].tourLength);
        }
    }

    done = true;
    std::vector<int> firstTour = ants[0].tour;
    for (int i = 1; i < numberOfCities; i++) {
        if (ants[i].tour != firstTour) {
            done = false;
            break;
        }
    }
}

    int bestTourIndex = 0;
    double bestTourLength = ants[0].tourLength;
    for(int i=1; i<numberOfCities; i++) 
    {
        if(ants[i].tourLength < bestTourLength)
        {
            bestTourIndex = i;
            bestTourLength = ants[i].tourLength;
        }
    }

    bestTourLength += ants[bestTourIndex].tour[0];
    ants[bestTourIndex].tour.push_back(ants[bestTourIndex].tour[0]);

    printf("Best tour: ");
    for(int i=0; i<numberOfCities+1; i++)
    {
        printf("%i ", ants[bestTourIndex].tour[i]);
    }

    return bestTourLength;
}

void AntColony::init_pheromones()
{
    pheromones.resize(numberOfCities, std::vector<double>(numberOfCities));
    for(int i=0; i<numberOfCities; i++)
    {
        for(int j=0; j<numberOfCities; j++)
        {
            pheromones.at(i).at(j) = INIT_PHER;
            //pheromones[i][j] = INIT_PHER;
        }
    }
}

void AntColony::refresh_pheromones()
{
    
}

Ant::Ant(){
    tourLength = 0.0;
    visited.resize(numberOfCities, false);
}

int Ant::nextCity(int currCity){
    double totalDesir = 0.0;
    double desir[numberOfCities];

    for(int i=0; i<numberOfCities; i++) {
        if(!visited[i]) {
            totalDesir += desirability(currCity, i);
        }
    }

    for(int i=0; i<numberOfCities; i++) {
        if(!visited[i]) {
            desir[i] = desirability(currCity, i) / totalDesir;
        }
        else {
            desir[i] = 0.0;
        }
    }

    double randNum ((double)rand() / RAND_MAX);
    double cumProb = 0.0;
    for(int i=0; i<numberOfCities; i++) {
        cumProb += desir[i];
        if(randNum <cumProb) {
            return i;
        }
    }
    return -1;
}

double Ant::desirability(int i, int j){
    double desir = pow(pheromones[i][j], ALPHA) * pow(1.0 / graph[i][j], BETA);
    return desir;
}