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

class Genetic {
public:
    Genetic(const double mutationRate);

    void breed(std::vector<Player>& population);
    void mutate(std::vector<Player>& population);

private:
    std::vector<Matrix> crossOver(Player parent1, const Player parent2);
    Player pickParent(const std::vector<Player>& population) const;
    double m_mutationRate;
};

#endif
