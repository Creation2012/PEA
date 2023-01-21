#include <iostream>
#include <fstream>

#include "GraphMatrix.h"

using namespace std;

constexpr auto config = "config.ini";

void configInit(GraphMatrix* gm) {
	printf("File processing...\n");
	int loops, optimal;
	double temperature, temperature_final, alpha;
	int path_method, epoch, neighbourhood_type, cooling_method;

	string path;
	ifstream file;
	string fileName;
	string outputName;
	
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
		getline(file,outputName);
		file.clear();
		file.seekg(0);

		ofstream ofs;
		ofs.open(outputName, ofstream::out | ofstream::trunc); // czyszczenie wyniki.csv
		ofs.close();

		while ((file >> fileName) && fileName.substr(fileName.find_last_of(".")) != ".csv") {
			file >> loops;
			file >> path_method;
			file >> temperature;   
			file >> temperature_final;
			file >> alpha;
			file >> epoch;
			file >> neighbourhood_type;
			file >> cooling_method;
			file >> optimal;
			//getline(file, path);
			if(!(gm->readFromFile(fileName))){
				printf("COULDN'T FIND FILE! WRONG FILE NAME? CURRENT FILE: %s", fileName);	
				break;
			}
			else {
				printf("CURRENT FILE: %s\n", fileName);
			}

			gm->testbench(1, fileName + " " + to_string(loops) + " " + to_string(optimal) + path + ";", loops, path_method, temperature, temperature_final, alpha, epoch, neighbourhood_type, cooling_method, optimal, outputName);
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