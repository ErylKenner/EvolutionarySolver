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
    vector<unsigned int>& layerSizes, vector<playerContainer>& population, Genetic& ga);

void roundRobin(vector<playerContainer>& population, int populationSize);
void printPopulationFrom(unsigned int start, unsigned int end, vector<playerContainer>& population);
void printSummary(int generation, vector<playerContainer>& population, int populationSize);

#endif
