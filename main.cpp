

#include "main.h"



int main(){
    int populationSize;
    int iterations;
    int hiddenLayers;
    vector<unsigned int> layerSizes;
    vector<playerContainer> population;
    Genetic ga(0.03f, 0.1f);
    srand( (unsigned int)time(NULL) );
    
    //Genetic ga(0.0f, 1.0f);
    //srand(1);
    
    init(cin, cout, populationSize, iterations, hiddenLayers, layerSizes, population, ga);
    
    //Training loop
    for(int generation = 0; generation < iterations; ++generation){
        bool verbose = false/*(generation % 50) == 0*/;
        
        //Play games with every permutaiton of players
        roundRobin(population, populationSize);
        
        //Sorts the players by fitness (ascending)
        sort(population.begin(), population.end(), comparePlayerContainer);
        
        //printPopulationFrom(0, 10, population);
        //printPopulationFrom(populationSize - 10, populationSize, population);
        
        //Print epoch summary
        printSummary(generation, population, populationSize);
        
        //Print board
        if(verbose){
            (population.back()).player->neural.printWeights();
        }
        
        //Make new players based on how successful the current ones are
        ga.breed(population);
        
        //Each weight has a small chance to change by some random value
        ga.mutate(population);
        
        //Reset fitness values for next generation
        for(int i = 0; i < populationSize; ++i){
            population[i].player->resetFitness();
        }
    }
    (population.back()).player->neural.printWeights();
   
    
    return 0;
}



void init(istream& is, ostream& os, int& populationSize, int& iterations, int& hiddenLayers,
    vector<unsigned int>& layerSizes, vector<playerContainer>& population, Genetic& ga){
    
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
    
    ga.setPopulationSize(populationSize);
}

//Play games with every permutaiton of players
void roundRobin(vector<playerContainer>& population, int populationSize){
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
}

//Print epoch summary
void printSummary(int generation, vector<playerContainer>& population, int populationSize){
    double maxVal = population.back().player->getFitness();
    double minVal = population.front().player->getFitness();
    int maxPossible = 2 * (populationSize - 1);
    
    
    printf("Gen: %3d,   Max fitness: %-6.1f [i=%-3d],   Min fitness: %-6.1f [i=%-3d],   Highest possible: %4d"
        , generation, maxVal, (population.back()).index, minVal, (population.front()).index, maxPossible);
    
    cout << endl;
}

void printPopulationFrom(unsigned int start, unsigned int end, vector<playerContainer>& population){
    if (start > end){
        cerr << "Error: printPopulationFrom start is greater than end" << endl;
        exit(1);
    }
    if (end > population.size()){
        cerr << "Error: printPopulationFrom end is larger than populationSize" << endl;
        exit(1);
    }
    for(unsigned int i = start; i < end; ++i){
        cout << "Population[" << population[i].index << "] fitness: ";
        printf("%5.1f\n", population[i].player->getFitness());
    }
}



    



