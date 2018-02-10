#ifndef NN
#define NN

#include "Matrix.h"
#include <iostream>
#include <vector>

class NeuralNet {
    public:
        NeuralNet(const Matrix layerSizes);
        Matrix forward();
        void printWeights();
        
        std::vector<Matrix> getWeights() const;
        void setWeights(const std::vector<Matrix> weights);
    private:
        Matrix layerSizes_;
        std::vector<Matrix> weights_;
        std::vector<Matrix> biases_;
        
        Matrix applyNonlinearity(Matrix a, double (*nonlinear)(double));
};

#endif
