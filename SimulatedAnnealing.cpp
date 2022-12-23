#include "SimulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing(std::vector<std::vector<int>> adjMatrix, int v)
{
    this->adjMatrix = adjMatrix;
    this-> v = v;
}

std::vector<int> SimulatedAnnealing::gen_first_path(){
    std::vector<int> path;
    std::vector<bool> visited(v,false);
    int min = INT_MAX;
    int min_index = 0;

    // Simple Hamilton Cycle Construct
    for(int i=1; i<v; i++){
        path.push_back(i);
    }
    return path;
}

void SimulatedAnnealing::geometric_cooling(double *temperature, double alpha)
{
    *temperature *= alpha;
}

// Boltzmann or Cauchy cooling

void cities_insert(std::vector<int> *path, int i, int j)
{
    if(i == 0 || j == 0 || i == (*path).size() - 1 || j == (*path).size() - 1)
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
    if(i == 0 || j == 0 || i == (*path).size() - 1 || j == (*path).size() - 1)
        return;
    int temp = (*path)[i];
    (*path)[i] = (*path)[j];
    (*path)[j] = temp;
}

void cities_invert(std::vector<int> *path, int i, int j)
{
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

void SimulatedAnnealing::Algorithm(){
    std::vector<int> s;
    double t = 1000;
    double t_final = 0.001;

    int num_iter = 100000;

    std::vector<int> path = gen_first_path(); // initial guess
    path.insert(path.begin(), 0);
    path.push_back(0);

    int min_cost = INT_MAX;
    int path_cost = cost_function(path);
    min_cost = path_cost;

    double alpha = 0.99;

    // Generating random numbers
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);

    //cities_insert(&path,1,7);
    //cities_swap(&path,5,6);
    //cities_invert(&path,0,path.size()-1); // uwzgledniac zawsze -1 przy max size
    //cities_invert(&path,4,6);
    while (t > t_final)
    {
        for (int i=0;i<num_iter;i++){
            std::vector<int> old_path = path;
            cities_swap(&path,(int)(dis(gen)*10000)%path.size(),(int)(dis(gen)*10000)%path.size());

            path_cost = cost_function(path);

            if(path_cost < min_cost)
            {
                min_cost = path_cost;                
            }
            else
            {
                double s = dis(gen);
                if(s < exp(-1*(path_cost - min_cost)/t))
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
        geometric_cooling(&t, alpha);
        // boltzmann cooling
    }
    //printf("%i",min_cost);
}