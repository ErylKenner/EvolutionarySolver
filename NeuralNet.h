#ifndef NN_H
#define NN_H

#include "Matrix.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::ofstream;
using std::ifstream;

class NeuralNet {
public:
    NeuralNet();
    NeuralNet(const vector<unsigned int>& layerSizes);
    NeuralNet(const NeuralNet& nn);
    
    Matrix forward(const Matrix& input) const;
    
    void printWeights() const;
    
    void operator= (const NeuralNet& nn);
    
    vector<Matrix> getWeights() const;
    void setWeights(const vector<Matrix>& weights);
    
    void saveToFile(const char fileName[]) const;
    void loadFromFile(const char fileName[]);
private:
    vector<unsigned int> m_layerSizes;
    vector<Matrix> m_weights;

    Matrix applyNonlinearity(const Matrix& input, double(*funct)(double)) const;
    static double sigmoid(const double x);
};

#endif
