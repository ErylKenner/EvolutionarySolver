

#include "main.h"

int main(){
    int populationSize;
    int iterations;
    int hiddenLayers;
    vector<unsigned int> layerSizes;
    vector<playerContainer<NeuralPlayer> > population;
    vector<playerContainer<NeuralPlayer> > hallOfFame;
    Genetic ga(0.03f, 0.1f);
    srand( (unsigned int)time(NULL) );
    
    //Genetic ga(0.0f, 1.0f);
    //srand(1);
    
    init(cin, cout, populationSize, iterations, hiddenLayers, layerSizes, population, ga);
    
    //--------Training loop-----------
    for(int generation = 0; generation < iterations; ++generation){
        bool verbose = false/*(generation % 50) == 0*/;
        
        //Play games with every permutaiton of players
        roundRobin(population, populationSize);
        
        //Sorts the players by fitness (ascending)
        sort(population.begin(), population.end(), comparePlayerContainer<NeuralPlayer>);
        
        hallOfFame.push_back(population.back());
        
        //printPopulationFrom(0, 10, population);
        //printPopulationFrom(0, populationSize, population);
        
        //Print board
        if(verbose){
            NeuralPlayer best = population.back().player;
            best.neural.printWeights();
        }
        
        //Test the current best vs. the best from each previous generation
        double HOF_percent = playHallOfFame(hallOfFame, population.back());
        
        //Print epoch summary
        printSummary(generation, population, populationSize, HOF_percent);
        
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
    
    ManualPlayer tempHuman(cin, cout);
    playerContainer<ManualPlayer> human(tempHuman);
    TicTacToe<NeuralPlayer, ManualPlayer> testGame(population.back(), human, true);
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

//Play games with every permutaiton of players
void roundRobin(vector<playerContainer<NeuralPlayer> >& population, int populationSize){
    for(int i = 0; i < populationSize - 1; ++i){
        for(int j = i + 1; j < populationSize; ++j){
            //Game 1
            //cout << "Game between [" << population[i].second << "] and [" << population[j].second << "]" << endl;
            TicTacToe<NeuralPlayer, NeuralPlayer> game1(population[i], population[j], false);
            game1.playGame();
            
            //Game 2 (play 2 games so both players can start first)
            //cout << "Game between [" << population[j].second << "] and [" << population[i].second << "]" << endl;
            TicTacToe<NeuralPlayer, NeuralPlayer> game2(population[j], population[i], false);
            game2.playGame();
        }   
    }
}

double playHallOfFame(vector<playerContainer<NeuralPlayer> >& hallOfFame, playerContainer<NeuralPlayer>& best){
    int numOpponents = hallOfFame.size() - 1;
    double initialFitness = best.player.getFitness();
    
    //Reset the best players' fitness to 0
    best.player.resetFitness();
    
    for(int i = 0; i < numOpponents; ++i){
        //Game 1
        TicTacToe<NeuralPlayer, NeuralPlayer> game1(hallOfFame[i], best, false);
        game1.playGame();
        
        //Game 2 (play 2 games so both players can start first)
        TicTacToe<NeuralPlayer, NeuralPlayer> game2(best, hallOfFame[i], false);
        game2.playGame();
    }
    double fractionOfWins = best.player.getFitness() / (2 * numOpponents);
    
    //Set the best player's fitness to what it was before this function
    best.player.resetFitness();
    best.player.addToFitness(initialFitness);
    
    return 100 * fractionOfWins;
}

    



