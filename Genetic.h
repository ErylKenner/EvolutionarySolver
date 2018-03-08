#ifndef GENETIC
#define GENETIC

#include <iostream>
#include <random>
#include "Matrix.h"
#include "NeuralNet.h"
#include "Player.h"

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
