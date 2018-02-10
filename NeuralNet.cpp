

#include "NeuralNet.h"


//Constructor
NeuralNet::NeuralNet(const Matrix layerSizes)
    : layerSizes_(layerSizes.numRows(), layerSizes.numCols())
    , weights_(1, Matrix(layerSizes(0, 1), layerSizes(0,0)) )
    , biases_(1, Matrix(layerSizes(0, 1), 1) ){
    
    layerSizes_ = layerSizes;
    weights_[0].initialize(1);
    biases_[0].initialize(1);
    for(int i = 1; i < layerSizes.numCols() - 1; ++i){
        Matrix *temp = new Matrix(layerSizes(0, i+1), layerSizes(0, i));
        Matrix *temp2 = new Matrix(layerSizes(0, i+1), 1);
        
        temp->initialize(1);
        temp2->initialize(1);
        
        weights_.push_back(*temp);
        biases_.push_back(*temp2);
        
        delete temp;
        delete temp2;
    }
}

void NeuralNet::printWeights(){
    std::cout << "Current weights:" << std::endl;
    for(int i = 0; i < weights_.size(); ++i){
        std::cout << "========================" << std::endl;
        weights_[i].printData();
    }
    std::cout << "========================" << std::endl;
}

Matrix NeuralNet::forward(){
    
}

std::vector<Matrix> NeuralNet::getWeights() const{
    return weights_;
}

void NeuralNet::setWeights(const std::vector<Matrix> weights){
    if (weights.size() == 0 || weights.size() != weights_.size()){
        throw std::out_of_range("Weights have different sizes");
    }
    for(int i = 0; i < weights_.size(); ++i){
        weights_[i] = weights[i];
    }
}

Matrix NeuralNet::applyNonlinearity(Matrix a, double (*nonlinear)(double)){
    for(int row = 0; row < a.numRows(); ++row){
        for(int col = 0; col < a.numCols(); ++col){
            a(row, col) = nonlinear(a(row, col));
        }
    }
}








