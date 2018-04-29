#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "Matrix.h"
#include "NeuralNet.h"
#include "Player.h"
#include "TicTacToe.h"
#include "Genetic.h"

using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;
using std::vector;


void init(istream& is, ostream& os, int& populationSize, int& iterations, int& hiddenLayers,
    vector<unsigned int>& layerSizes, vector<playerContainer<NeuralPlayer> >& population, Genetic& ga);

void roundRobin(vector<playerContainer<NeuralPlayer> >& population, int populationSize);

double playHallOfFame(vector<playerContainer<NeuralPlayer> >& hallOfFame, playerContainer<NeuralPlayer>& best);

//Print epoch summary
template <class T>
void printSummary(int generation, vector<playerContainer<T> >& population, 
    int populationSize, double HOF_percent){
    
    playerContainer<T> maxPlayer = population.back();
    playerContainer<T> minPlayer = population.front();
    playerContainer<T> medianPlayer = population[populationSize / 2];
    
    int maxPossible = 2 * (populationSize - 1);
    
    
    printf("Gen: %3d", generation);
    printf(",   Min: %-6.1f [i=%-3d]", 
        minPlayer.player.getFitness(), minPlayer.index);
    
    printf(",   Median: %-6.1f [i=%-3d]", 
        medianPlayer.player.getFitness(), medianPlayer.index);
    
    printf(",   Max: %-6.1f [i=%-3d]", 
        maxPlayer.player.getFitness(), maxPlayer.index);
    
    printf(",   Max possible: %4d", maxPossible);
    printf(",   Win vs HOF: %.2lf%%", HOF_percent);
    cout << endl;
}

template <class T>
void printPopulationFrom(unsigned int start, unsigned int end, vector<playerContainer<T> >& population){
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
        printf("%5.1f\n", population[i].player.getFitness());
    }
}
#endif
