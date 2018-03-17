#ifndef NN_H
#define NN_H

#include "Matrix.h"
#include <iostream>
#include <vector>
#include <math.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

class NeuralNet {
public:
    NeuralNet(const vector<unsigned int>& layerSizes);
    NeuralNet(const NeuralNet& nn);
    
    Matrix forward(const Matrix& input) const;
    
    void printWeights() const;
    
    void operator= (const NeuralNet& nn);
    
    vector<Matrix> getWeights() const;
    void setWeights(const vector<Matrix>& weights, const vector<Matrix>& biases);
private:
    vector<unsigned int> m_layerSizes;
    vector<Matrix> m_weights;
    vector<Matrix> m_biases;

    Matrix applyNonlinearity(const Matrix& input, double(*funct)(double)) const;
    static double sigmoid(const double x);
};

#endif
