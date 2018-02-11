#include <iostream>
#include <vector>
using namespace std;

#include "Matrix.h"
#include "NeuralNet.h"
#include "TicTacToe.h"

#define NUM_INPUTS 9
#define NUM_OUTPUTS 9


int main(){
    srand(time(NULL));

    //Get population size
    int populationSize;
    cout << "Population size: ";
    cin >> populationSize;

    //Populate layerSizes
    int hiddenLayers;
    cout << "Number of hidden layers: ";
    cin >> hiddenLayers;
    
    Matrix layerSizes(1, hiddenLayers + 2);
    layerSizes(0, 0) = NUM_INPUTS;
    layerSizes(0, hiddenLayers + 1) = NUM_OUTPUTS;

    for(int i = 1; i <= hiddenLayers; ++i){
        cout << "Number in hidden layer " << i << ": ";
        cin >> layerSizes(0, i);
    }

    //Instantiate the Players
    std::vector<NeuralNet> population;
    for(int i = 0; i < populationSize; ++i){
        NeuralNet *player = new NeuralNet(layerSizes);
        population.push_back(*player);
    }

    //Play games
    for(int i = 0; i < populationSize; ++i){
        for(int j = i; j < populationSize; ++j){
            if(i == j){
                continue;
            }
            TicTacToe game1(&population[i], &population[j], false);
            game1.playGame();
            TicTacToe game2(&population[j], &population[i], false);
            game2.playGame();
        }   
    }

    //Print players' fitness
    for(int i = 0; i < populationSize; ++i){
        cout << "Player " << i + 1<< " fitness: " <<
            population[i].getFitness() << endl;
    }

    //Print the max fitness
    double maxVal = population[0].getFitness();
    for(int i = 0; i < populationSize; ++i){
        if(population[i].getFitness() > maxVal){
            maxVal = population[i].getFitness();
        }
    }
    cout << "Max fitness: " << maxVal
        << ", Highest possible: " << 2 * (populationSize - 1) << endl;
   
    
    return 0;
}
