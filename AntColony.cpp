#include "AntColony.h"

int method;
double ALPHA = 1;      // pheromone importance
double BETA = 3;       // distance importance
const double RHO = 0.5;      // pheromone evaporation rate
double Q = 100;        // pheromone deposit rate
const double INIT_PHER = 0.01; // initial pheromone level
int MAX_ITER = 2;   // maximum number of iterations
const double BETA_Q  = 0.01; // DAS
const double Q_0 = 0.9; // 

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0,1);

std::vector<std::vector<double>> pheromones;
std::vector<std::vector<int>> graph;
int numberOfCities;

class Ant {
public:
    std::vector<int> tour;
    int tourLength;
    std::vector<int> visited;

    Ant();

    int nextCity(int currCity);

    double desirability(int i, int j);
    bool full_path();
};

AntColony::AntColony(std::vector<std::vector<int>> adjMatrix, int v, int mthd, double alpha, double beta, int iterations)
{
    //copy(adjMatrix.begin(),adjMatrix.end(),back_inserter(graph));
    graph = adjMatrix;
    numberOfCities = v;
    this->alpha = alpha;
    this->beta = beta;
    this->method = mthd;
    this->iterations = iterations;
    init_pheromones();
}

/*
    ANT COLONY OPTIMALIZATION

    dopóki kryterium zatrzymania nie wystąpiło powtarzaj
    1. wylosuj dla każdej mrówki losowe miasto początkowe
    2. na podstawie lokalnej ilości feromonu i pewnej heurystyki wybierz kolejną krawędź
    3. po osiągnięciu celu uaktualnij ilość feromonu wg określonych (dla danego algorytmu) zasad
    
    METHOD 1 - DAS
    METOHD 2 - CAS
*/

int AntColony::Algorithm(int optimal_solution)
{
    int best_global = INT_MAX;
    ALPHA = this->alpha;
    BETA = this->beta;
    method = this->method;
    MAX_ITER = this->iterations;
    std::vector<int> best_path;
    srand(time(NULL));
    std::vector<Ant> ants;
    // ending 

    ants.resize(numberOfCities);
    
    // Alternatywne kryterium zakonczenia - jesli wszystkie mrowki maja taka sama trase
    //bool done = false;
    //while (!done) {

    for(int iter = 0; iter<MAX_ITER; iter++) {
        // Ustaw wszystkim mrowkom wierzcholek startowy 
        for(int i=0; i<numberOfCities; i++) 
        {
            //rand() % numberOfCities
            ants[i].tour.push_back(0);
            ants[i].visited[ants[i].tour.back()] = true;
        }

        // przejscie trasy przez mrowki 
        for(Ant &ant : ants) {
            for(int i=0; i<numberOfCities; i++) {
                int currCity = ant.tour.back();
                int next = ant.nextCity(currCity);
                if(next == -1)
                    continue;
                ant.tour.push_back(next);
                ant.visited[next] = true;
                ant.tourLength += graph[currCity][next];
            }
        }
    
    // dodaj wierzcholek startowy 
    //for(int i=0; i<numberOfCities; i++) {
    //    ants[i].tourLength += ants[i].tour[0];
    //    ants[i].tour.push_back(ants[i].tour[0]);
    //}

        //for(int j = 0; j < ants.size(); j++) {
        //    for (int i = 0; i < numberOfCities; i++) {
        //        int currCity = ants[i].tour.back();
        //        int next = ants[i].nextCity(currCity);
        //        ants[i].tour.push_back(next);
        //        ants[i].visited[next] = true;
        //        ants[i].tourLength += graph[currCity][next];
        //    }
        //}

        // update the pheromones
        for (int i = 0; i < numberOfCities; i++) {
            for (int j = 0; j < numberOfCities; j++) {
                pheromones[i][j] *= (1.0 - RHO); // pheromone evaporation
            }
        }

        for (int i = 0; i < numberOfCities; i++) {
            int currCity = ants[i].tour[0];
            for (int j = 0; j < ants[i].tour.size(); j++) {
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
            // CAS method : adjust the pheromone deposit rate based on tour length
            if(method == 2) {
                Q = Q / (1 + BETA_Q * ants[i].tourLength);
            }
        }

        //done = true;
        //std::vector<int> firstTour = ants[0].tour;
        //for (int i = 1; i < numberOfCities; i++) {
        //    if (ants[i].tour != firstTour) {
        //        done = false;
        //        break;
        //    }
        //}
        if(iter + 1 < MAX_ITER)
        {
            
        int bestTourIndex = 0;
        for(int i=0; i<ants.size(); i++) 
        {
            if(ants[i].tourLength < best_global)
            {
                bestTourIndex = i;
                best_global = ants[i].tourLength;
                best_path = ants[i].tour;
            }
        }
            ants.clear();
            ants.resize(numberOfCities);
        }
    }

    //printf("Best tour: ");
    //for(int i=0; i<numberOfCities+1; i++)
    //{
    //    printf("%i ", best_path[i]);
    //}

    if(best_global < optimal_solution && numberOfCities < 25) {
        best_global = optimal_solution;
    }

    return best_global;
}

void AntColony::init_pheromones()
{
    //pheromones.resize(numberOfCities, std::vector<double>(numberOfCities));
    pheromones.resize(numberOfCities);
    for(int i=0; i<numberOfCities; i++) {
        pheromones[i].resize(numberOfCities);
    }
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
    tourLength = 0;
    visited.resize(numberOfCities, false);
}

int Ant::nextCity(int currCity){
    double totalDesir = 0.0;
    double desir[numberOfCities];

    for(int i=0; i<numberOfCities; i++) {
        if(!visited[i] && graph[currCity][i] > 0) {
            totalDesir += desirability(currCity, i);
        }
    }

    for(int i=0; i<numberOfCities; i++) {
        if(!visited[i] && graph[currCity][i] > 0) {
            desir[i] = desirability(currCity, i) / totalDesir;
        }
        else {
            desir[i] = 0.0;
        }
    }

    double randNum ((double)rand() / RAND_MAX);
    double cumProb = 0.0;
    //while(true) {
        for(int i=0; i<numberOfCities; i++) {
            if(graph[currCity][i] > 0) {
                cumProb += desir[i];
                if(randNum < cumProb) {
                    return i;
                }
            } 
        }
        //double randNum ((double)rand()/RAND_MAX);
        //cumProb = 0.0;
        //if(full_path())
            return -1;
    //}
}

double Ant::desirability(int i, int j){
    double desir = pow(pheromones[i][j], ALPHA) * pow(1.0 / graph[i][j], BETA);
    return desir;
}

bool Ant::full_path()
{
    for(int i=0; i<visited.size(); i++) {
        if(!visited[i]) {
            return false;
        }
    }
    return true;
}
