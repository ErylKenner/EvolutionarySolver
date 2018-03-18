#ifndef GENETIC_H
#define GENETIC_H

#include <iostream>
#include <random>

#include "Matrix.h"
#include "NeuralNet.h"
#include "Player.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::default_random_engine;
using std::normal_distribution;

class Genetic {
public:
    Genetic(const float mutationRate, const float greedyPercent);

    void setPopulationSize(int populationSize);
    
    void breed(vector<playerContainer>& population);
    void mutate(vector<playerContainer>& population);

private:
    vector<Matrix> crossOver(const playerContainer parent1, const playerContainer parent2);
    playerContainer pickParent(const vector<playerContainer>& population) const;
    static float bound(float value, float min, float max);
    
    const float m_mutationRate;
    int m_populationSize;
    const float m_greedyPercent; 
};

#endif
