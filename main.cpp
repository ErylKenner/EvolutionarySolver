

#include "main.h"

//TODO:Are the GA and NN classes working correctly with the new inheritance system?

int main(){
    int populationSize;
    int iterations;
    int hiddenLayers;
    vector<unsigned int> layerSizes;
    vector<playerContainer<NeuralPlayer> > population;
    //Genetic ga(0.03f, 0.1f);
    //srand( (unsigned int)time(NULL) );
    
    Genetic ga(0.0f, 1.0f);
    srand(1);
    
    init(cin, cout, populationSize, iterations, hiddenLayers, layerSizes, population, ga);
    
    //Training loop
    for(int generation = 0; generation < iterations; ++generation){
        bool verbose = false/*(generation % 50) == 0*/;
        
        //Play games with every permutaiton of players
        roundRobin<NeuralPlayer>(population, populationSize);
        
        //Sorts the players by fitness (ascending)
        sort(population.begin(), population.end(), comparePlayerContainer<NeuralPlayer>);
        
        //printPopulationFrom(0, 10, population);
        //printPopulationFrom(populationSize - 10, populationSize, population);
        
        //Print epoch summary
        printSummary(generation, population, populationSize);
        
        //Print board
        if(verbose){
            NeuralPlayer best = population.back().player;
            best.neural.printWeights();
        }
        
        //Make new players based on how successful the current ones are
        ga.breed(population);
        
        //Each weight has a small chance to change by some random value
        ga.mutate(population);
        
        //Reset fitness values for next generation
        for(int i = 0; i < populationSize; ++i){
            population[i].player.resetFitness();
        }
    }
    
    //Print the best one
    NeuralPlayer best = population.back().player;
    best.neural.printWeights();
    
    
    ManualPlayer *human = new ManualPlayer(cin, cout);
    TicTacToe<NeuralPlayer, ManualPlayer> testGame(population.back().player, *human, true);
    testGame.playGame();
    
    return 0;
}



void init(istream& is, ostream& os, int& populationSize, int& iterations, int& hiddenLayers,
    vector<unsigned int>& layerSizes, vector<playerContainer<NeuralPlayer> >& population, Genetic& ga){
    
    //Get population size
    os << "Population size: ";
    is >> populationSize;
    if(populationSize < 2){
        populationSize = 2;
    }
    
    //Get number of iterations
    os << "Iterations: ";
    is >> iterations;
    if(iterations < 1){
        iterations = 1;
    }
    
    //Get number of layers
    os << "Number of hidden layers: ";
    is >> hiddenLayers;
    
    //Populate layerSizes
    layerSizes.push_back(NUM_INPUTS);
    for(int i = 1; i <= hiddenLayers; ++i){
        os << "Number in hidden layer " << i << ": ";
        unsigned int temp;
        is >> temp;
        if(temp < 1){
            temp = 1;
        }
        layerSizes.push_back(temp);
    }
    layerSizes.push_back(NUM_OUTPUTS);
    
    //Instantiate the Players
    for(int i = 0; i < populationSize; ++i){
        NeuralPlayer temp(layerSizes);
        playerContainer<NeuralPlayer> tempContainer(temp);
        population.push_back(tempContainer);
    }
    
    ga.setPopulationSize(populationSize);
}





    



