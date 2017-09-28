// Generates the datasets for Random Forests by randomly selecting 512 instances
// from the adult data set. This produces datasets for 128 trees which is then read
// by build_random_trees.cpp to generate the rulebases for these trees

#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <functional>
using namespace std;
const string trainingDataPath = "../adult.data";
const string randomForestFilePath = "../random_forest/adultTraining";
const int trees = 128;
const int instances = 512;
int main()
{
    ifstream input(trainingDataPath);
    string line;
    vector<string> fullDataSet;
    if (input.is_open()) {
        while(getline(input, line)) {
            line.push_back('\n');
            fullDataSet.push_back(line);
        }
    }
    input.close();
    int fileNum = 0;
    mt19937::result_type seed = time(0);
    auto vector_rand = std::bind(std::uniform_int_distribution<int>(0,fullDataSet.size()-1), mt19937(seed));
    for (int i = 0; i < trees; i++) {
        string filePath = randomForestFilePath + to_string(i);
        ofstream output(filePath);
        for (int j = 0; j < instances; j++) {
            output << fullDataSet[vector_rand()];
        }
        output.close();
    }
    return 0;
}
