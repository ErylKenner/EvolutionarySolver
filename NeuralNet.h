#ifndef NN
#define NN

#include "Matrix.h"
#include <iostream>
#include <vector>
#include <math.h>


class NeuralNet {
public:
    NeuralNet(const Matrix& layerSizes);
    
    Matrix forward(const Matrix& input) const;
    void printWeights() const;
    
    std::vector<Matrix> getWeights() const;
    void setWeights(const std::vector<Matrix> weights);

    void addToFitness(const double a);
    double getFitness() const;
    void resetFitness();

    static bool compFitness(const NeuralNet& player1, const NeuralNet& player2);
private:
    Matrix m_layerSizes;
    std::vector<Matrix> m_weights;
    std::vector<Matrix> m_biases;

    double m_fitness;

    Matrix applyNonlinearity(const Matrix& input, double(*funct)(double)) const;
    static double sigmoid(const double x);
};

#endif
