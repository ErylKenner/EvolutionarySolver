#ifndef GENETIC_H
#define GENETIC_H

#include <iostream>
#include <random>

#include <Eigen/Dense>
using namespace Eigen;

#include "NeuralNet.h"
#include "Player.h"
#include "main.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

class Genetic {
public:
    Genetic(const float mutationRate, const float greedyPercent);

    void setPopulationSize(int populationSize);
    
    void breed(vector<playerContainer<NeuralPlayer> >& population);
    void mutate(vector<playerContainer<NeuralPlayer> >& population);

private:
    vector<MatrixXd> crossOver(const playerContainer<NeuralPlayer>& parent1, 
                             const playerContainer<NeuralPlayer>& parent2);
    
    playerContainer<NeuralPlayer> pickParent(
            const vector<playerContainer<NeuralPlayer> >& population) const;
    
    const float m_mutationRate;
    int m_populationSize;
    const float m_greedyPercent; 
};

#endif
