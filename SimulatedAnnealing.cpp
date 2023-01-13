#include "SimulatedAnnealing.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0,1);

SimulatedAnnealing::SimulatedAnnealing(std::vector<std::vector<int>> adjMatrix, int v)
{
    this->adjMatrix = adjMatrix;
    this-> v = v;
}

std::vector<int> SimulatedAnnealing::gen_first_path(){
    std::vector<int> path;
    std::vector<bool> visited(v,false);

    // Simple Hamilton Cycle Construct
    for(int i=1; i<v; i++){
        path.push_back(i);
    }

    return path;
}

std::vector<int> SimulatedAnnealing::gen_random_path(){
    std::vector<int> path;
    std::vector<bool> visited(v,false);
    visited[0] = true;

    int vj = (int)(dis(gen)*10000)%v;

    for(int i=0; i<v-1; i++){
        while(visited[vj])
        {
            vj = (int)(dis(gen)*10000)%v;
        }
        path.push_back(vj);
        visited[vj] = true;
    }
    return path;
}

std::vector<int> SimulatedAnnealing::gen_greedy_path(){
    std::vector<int> path;
    std::vector<bool> visited(v,false);
    visited[0] = true;

    int greedy_choose = INT_MAX;
    int greedy_index = 0;

    for(int i=0; i<v; i++){
        greedy_choose = INT_MAX;
        greedy_index = 0;
        for(int j=1; j<v; j++){
            if(i!=j && adjMatrix[i][j] < greedy_choose && !visited[j]){
                greedy_choose = adjMatrix[i][j];
                greedy_index = j;
            }
        }
        if(greedy_index != 0){
            visited[greedy_index] = true;
            path.push_back(greedy_index);
        }
    }
    return path;
}

int SimulatedAnnealing::longest_edge(){
    int max_edge = 0;
    for(int i=0; i<v; i++) {
        for(int j=0; j<v; j++){
            if(i!=j && adjMatrix[i][j] > 0){
                if(adjMatrix[i][j] > max_edge){
                    max_edge = adjMatrix[i][j];
                }
            }
        }
    }
    return max_edge;
}

void SimulatedAnnealing::geometric_cooling(double *temperature, double alpha, int k)
{
    *temperature = pow(alpha,k) * *temperature;
}

void SimulatedAnnealing::boltzmann_cooling(double *temperature, double alpha, double b, int k)
{
    *temperature = (*temperature)/(alpha + 100*b * log10(double(k/10000)));
}

void SimulatedAnnealing::cauchy_cooling(double *temperature, double alpha, double b, int k)
{
    *temperature = *temperature/(alpha + b * k);
}

// Boltzmann or Cauchy cooling
void cities_insert(std::vector<int> *path, int i, int j)
{
    if(i == 0 || j == 0 || i == (*path).size() - 1 || j == (*path).size() - 1 || i == j)
        return;
    std::vector<int>::iterator it;
    it = (*path).begin();
    int temp = (*path)[j];
    (*path).erase(it+j);
    (*path).insert(it+i, temp);
}

// dodac losowa zamiane dla wszystkich przeksztalcen
void cities_swap(std::vector<int> *path, int i, int j)
{
    if(i == 0 || j == 0 || i == (*path).size() - 1 || j == (*path).size() - 1 || i == j)
        return;
    int temp = (*path)[i];
    (*path)[i] = (*path)[j];
    (*path)[j] = temp;
}

void cities_invert(std::vector<int> *path, int i, int j)
{
    if(i == 0 || j == 0 || i == (*path).size() - 1 || j == (*path).size() - 1 || i == j)
        return;
    std::reverse((*path).begin()+i,(*path).begin()+j+1); 
}

int SimulatedAnnealing::cost_function(std::vector<int> path){
    int cost = 0;
    int idx = path[0];
    
    for(int i=1; i<path.size(); i++){
        cost += adjMatrix[idx][path[i]];
        idx = path[i];
    }

    return cost;
}

int SimulatedAnnealing::Algorithm(int path_method, double temperature, double temperature_final, double alpha, int epoch, int neighbourhood_type, int cooling_method){
    std::vector<int> path;
    int k = 0;
    int differ;

    // Initial state
    if(path_method == 1) {
        path = gen_random_path();
    }
    else if(path_method == 2) {
        path = gen_greedy_path(); 
    }
    else {
        return 0;
    }

    // Starting at 0 - and end at 0
    path.insert(path.begin(), 0);
    path.push_back(0);

    // Best path cost of initial state
    int min_cost = INT_MAX;
    int path_cost = cost_function(path);
    min_cost = path_cost;

    // Boltzmann constant (before or after temp method?)
    double b = 2;

    // Temperature method 
    if(temperature == 0){
        temperature = (longest_edge())^2 * v^2;
    } 
    else {
        // Only when temperature other than 0
        temperature = temperature * (pow(v,2));
    }

    //cities_insert(&path,1,7);
    //cities_swap(&path,5,6);
    //cities_invert(&path,0,path.size()-1); // uwzgledniac zawsze -1 przy max size
    //cities_invert(&path,4,6);

    while (temperature > temperature_final)
    {
        k++;
        for (int i=0;i<epoch;i++){
            std::vector<int> old_path = path;
            int vi = (int)(dis(gen)*10000)%path.size();
            int vj = (int)(dis(gen)*10000)%path.size();
            
            if(neighbourhood_type == 1){
                cities_swap(&path,vi,vj);
            }
            else if(neighbourhood_type == 2) {
                cities_insert(&path,vi,vj);
            }
            else if(neighbourhood_type == 3) {
                cities_invert(&path,vi,vj);
            }
            else {
                return 0;
            }

            path_cost = cost_function(path);

            if(path_cost < min_cost)
            {
                min_cost = path_cost;                
            }
            else
            {
                double s = dis(gen);
                differ = path_cost - min_cost;
                if(s < exp((-1*differ)/temperature))
                {
                    min_cost = path_cost;
                }
                else 
                {
                    path = old_path;
                }
            }
        }
        // Select cooling method
        if(cooling_method == 1){
            geometric_cooling(&temperature, alpha, b);
        }
        else if(cooling_method == 2){
            boltzmann_cooling(&temperature, alpha, b, k);
        }
        else if(cooling_method == 3){
            cauchy_cooling(&temperature, alpha, b, k);
        }
    }
    return min_cost;
}