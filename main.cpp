#include <iostream>
#include <fstream>

#include "GraphMatrix.h"

using namespace std;

constexpr auto config = "config.ini";

void configInit(GraphMatrix* gm) {
	printf("File processing...\n");
	int repeat_test, optimal_solution;
	int method;
	double alpha;
	double beta;
	int iterations;
	string path;
	ifstream file;
	string file_name;
	string output_name;
	
	file.open(config);
	if (file.is_open()) {
		file.seekg(-1, ios_base::end);
		if (file.peek() == 'v') {
			file.seekg(-1, ios_base::cur);
			int i = file.tellg();
			for (i; i > 0; i--) {
				if (file.peek() == '\n') {
					file.get();
					break;
				}
				file.seekg(i, ios_base::beg);
			}
		}
		getline(file,output_name);
		file.clear();
		file.seekg(0);

		ofstream ofs;
		ofs.open(output_name, ofstream::out | ofstream::trunc); // czyszczenie wyniki.csv
		ofs.close();

		while ((file >> file_name) && file_name.substr(file_name.find_last_of(".")) != ".csv") {
			file >> repeat_test;
			file >> method;
			file >> alpha;
			file >> beta;
			file >> iterations;
			file >> optimal_solution;
			//getline(file, path);
			if(!(gm->readFromFile(file_name))){
				printf("COULDN'T FIND FILE! WRONG FILE NAME? CURRENT FILE: %s", file_name);	
				break;
			}
			else {
				printf("CURRENT FILE: %s\n", file_name);
			}

			//gm->testbench(1, fileName + " " + to_string(loops) + " " + to_string(optimal) + path + ";", loops, path_method, temperature, temperature_final, alpha, epoch, neighbourhood_type, cooling_method, optimal, outputName);
			
			gm->testbench(repeat_test, method, alpha, beta, iterations, optimal_solution, file_name, output_name);
		}
		file.close();
	}
	else {
		printf("NO CONFIG!");
	}

}

int main() {
	GraphMatrix* gm = new GraphMatrix();
	configInit(gm); // inicjalizacja config.ini
}