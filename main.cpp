

#include "main.h"



int main(){
    int populationSize;
    int iterations;
    int hiddenLayers;
    vector<unsigned int> layerSizes;
    vector<playerContainer> population;
    //Genetic ga(0.03);
    
    //srand(time(NULL));
    srand(1);
    init(cin, cout, populationSize, iterations, hiddenLayers, layerSizes, population);
    
    
    for(int generation = 0; generation < iterations; ++generation){
        bool verbose = (generation % 50) == 0;
        
        //Play games with every permutaiton of players
        for(int i = 0; i < populationSize - 1; ++i){
            for(int j = i + 1; j < populationSize; ++j){
                //Game 1
                //cout << "Game between [" << population[i].second << "] and [" << population[j].second << "]" << endl;
                TicTacToe game1(population[i].player, population[j].player, false);
                game1.playGame();
                
                //Game 2 (play 2 games so both players can start first)
                //cout << "Game between [" << population[j].second << "] and [" << population[i].second << "]" << endl;
                TicTacToe game2(population[j].player, population[i].player, false);
                game2.playGame();
            }   
        }
        
        //Sorts the players by fitness (ascending)
        sort(population.begin(), population.end(), comparePlayerContainer);
        
        //Print fitness values
        /*for(int i = 0; i < populationSize; ++i){
            cout << "Population[" << population[i].index << "] fitness: " << population[i].player->getFitness() << endl;
        }*/
        
        //Print the max fitness
        double maxVal = population.back().player->getFitness();
        double minVal = population.front().player->getFitness();
        int maxPossible = 2 * (populationSize - 1);
        
        printf("Max fitness: %4.2f [%d], Min fitness: %4.2f [%d], Highest possible: %4d",
            maxVal, (population.back()).index, minVal, (population.front()).index, maxPossible);
        cout << endl;
        
        //Print board
        if(verbose){
            (population.back()).player->neural.printWeights();
        }
        
        //ga.breed(population);
        //ga.mutate(population);
        
        //Reset fitness values for next generation
        for(int i = 0; i < populationSize; ++i){
            population[i].player->resetFitness();
        }
    }
   
    
    return 0;
}



void init(istream& is, ostream& os, int& populationSize, int& iterations, int& hiddenLayers,
    vector<unsigned int>& layerSizes, vector<playerContainer>& population){
    
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
        layerSizes.push_back(temp);
    }
    layerSizes.push_back(NUM_OUTPUTS);
    
    //Instantiate the Players
    for(int i = 0; i < populationSize; ++i){
        Player *temp = new Player(layerSizes);
        //playerContainer tempContainer(temp);
        population.push_back(playerContainer(temp));
    }
}






    



