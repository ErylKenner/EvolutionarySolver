#ifndef NN
#define NN

#include "Matrix.h"
#include <iostream>
#include <vector>
#include <math.h>


class NeuralNet {
public:
    NeuralNet(const Matrix layerSizes);
    
    Matrix forward(const Matrix input) const;
    void printWeights() const;
    
    std::vector<Matrix> getWeights() const;
    void setWeights(const std::vector<Matrix> weights);

    void addToFitness(const double a);
    double getFitness() const;
    void resetFitness();
private:
    Matrix layerSizes_;
    std::vector<Matrix> weights_;
    std::vector<Matrix> biases_;
    double fitness_;

    double sigmoid(double x) const;
    Matrix applyNonlinearity(Matrix a) const;
};

#endif
