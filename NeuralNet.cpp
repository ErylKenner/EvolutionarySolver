#include "NeuralNet.h"


//Constructor
NeuralNet::NeuralNet(const Matrix layerSizes)
    : layerSizes_(layerSizes)
    , weights_(1, Matrix(layerSizes(0, 0), layerSizes(0,1)) )
    , biases_(1, Matrix(1, layerSizes(0, 1)) ){
    
    weights_[0].initRand(-1, 1);
    biases_[0].initRand(-1, 1);
    for(int i = 1; i < layerSizes.numCols() - 1; ++i){
        Matrix temp(layerSizes(0, i), layerSizes(0, i+1));
        Matrix temp2(1, layerSizes(0, i+1));
        
        temp.initRand(-1, 1);
        temp2.initRand(-1, 1);
        
        weights_.push_back(temp);
        biases_.push_back(temp2);
    }
    fitness_ = 0;
}

void NeuralNet::printWeights() const{
    std::cout << "Current weights:" << std::endl;
    for(int i = 0; i < weights_.size(); ++i){
        std::cout << "========================" << std::endl;
        weights_[i].printData();
    }
    std::cout << "========================" << std::endl;
}

Matrix NeuralNet::forward(Matrix input) const{
    std::vector<Matrix> prev;
    prev.push_back(input);
    for(int lay = 0; lay < weights_.size(); ++lay){
        prev.push_back( applyNonlinearity( prev.back() * weights_[lay] + biases_[lay] ) );
    }
    return prev.back();
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

Matrix NeuralNet::applyNonlinearity(Matrix a) const{
    Matrix temp(a);
    for(int row = 0; row < a.numRows(); ++row){
        for(int col = 0; col < a.numCols(); ++col){
            temp(row, col) = sigmoid(a(row, col));
        }
    }
    return temp;
}


double NeuralNet::sigmoid(double x) const{
    return 1 / (1 + exp(-x));
}

void NeuralNet::resetFitness(){
    fitness_ = 0.0;
}

void NeuralNet::addToFitness(const double a){
    fitness_ += a;
}

double NeuralNet::getFitness() const{
    return fitness_;
}

