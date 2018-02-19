#ifndef GENETIC
#define GENETIC

#include <iostream>
#include <random>
#include "NeuralNet.h"
#include "Matrix.h"


class Genetic {
public:
    Genetic(const double mutationRate);

    void breed(std::vector<NeuralNet>& population);
    void mutate(std::vector<NeuralNet>& population);

private:
    std::vector<Matrix> crossOver(NeuralNet parent1, const NeuralNet parent2);
    NeuralNet pickParent(const std::vector<NeuralNet>& population) const;
    double m_mutationRate;
};

#endif
