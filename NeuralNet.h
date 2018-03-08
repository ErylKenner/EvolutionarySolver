#ifndef NN
#define NN

#include "Matrix.h"
#include <iostream>
#include <vector>
#include <math.h>


class NeuralNet {
public:
    NeuralNet(const std::vector<unsigned int>& layerSizes);
    NeuralNet(const NeuralNet& nn);
    
    Matrix forward(const Matrix& input) const;
    
    void printWeights() const;
    
    void operator= (const NeuralNet& nn);
    
    std::vector<Matrix> getWeights() const;
    void setWeights(const std::vector<Matrix>& weights, const std::vector<Matrix>& biases);
private:
    std::vector<unsigned int> m_layerSizes;
    std::vector<Matrix> m_weights;
    std::vector<Matrix> m_biases;

    Matrix applyNonlinearity(const Matrix& input, double(*funct)(double)) const;
    static double sigmoid(const double x);
};

#endif
