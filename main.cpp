#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "Matrix.h"
#include "NeuralNet.h"
#include "TicTacToe.h"
#include "Genetic.h"

int main(){
    srand(time(NULL));

    //Get population size
    int populationSize;
    cout << "Population size: ";
    cin >> populationSize;
    if(populationSize < 2){
        populationSize = 2;
    }
    
    //Get number of iterations
    int iterations;
    cout << "Iterations: ";
    cin >> iterations;
    if(iterations < 1){
        iterations = 1;
    }

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
        NeuralNet player(layerSizes);
        population.push_back(player);
    }

    //Instantiate genetic algorithm
    Genetic ga(0.03);

    for(int generation = 0; generation < iterations; ++generation){
        bool verbose = (generation % 50) == 0;
        //Play games
        for(int i = 0; i < populationSize; ++i){
            for(int j = i + 1; j < populationSize; ++j){
                //Game 1 (play 2 games so both players can start first)
                TicTacToe game1(population[i], population[j], false);
                game1.playGame();
                //Game 2
                TicTacToe game2(population[j], population[i], false);
                game2.playGame();
            }   
        }
        
        //Sorts the players by fitness (ascending)
        sort(population.begin(), population.end(), NeuralNet::compFitness);

        //Print the max fitness
        double maxVal = population.back().getFitness();
        double minVal = population.front().getFitness();
        printf("Max fitness: %4.2f, Min fitness: %4.2f, Highest possible: %4d"
            , maxVal, minVal, 2 * (populationSize - 1));
        cout << endl;
        //cout << "Max fitness: " << maxVal << ", Min fitness: " << minVal;
        //cout << ", Highest possible: " << 2 * (populationSize-1) << endl;

        if(verbose){
            population.back().printWeights();
        }
        
        ga.breed(population);
        ga.mutate(population);
    }


    /*
    //Print players' fitness
    for(int i = 0; i < populationSize; ++i){
        cout << "Player " << i + 1<< " fitness: " <<
            population[i].getFitness() << endl;
    }
    */
   
    
    return 0;
}

