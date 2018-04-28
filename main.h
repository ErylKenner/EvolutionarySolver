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
void printSummary(int generation, vector<playerContainer<T> >& population, int populationSize){
    double maxVal = population.back().player.getFitness();
    double minVal = population.front().player.getFitness();
    int maxPossible = 2 * (populationSize - 1);
    
    
    printf("Gen: %3d,   Max fitness: %-6.1f [i=%-3d],   Min fitness: %-6.1f [i=%-3d],   Max possible: %4d"
        , generation, maxVal, (population.back()).index, minVal, (population.front()).index, maxPossible);
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
