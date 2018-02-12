#include "NeuralNet.h"


//Constructor takes in the structure of the network as a matrix
NeuralNet::NeuralNet(const Matrix layerSizes)
    : layerSizes_(layerSizes){

    for(int i = 0; i < layerSizes.numCols() - 1; ++i){
        Matrix tempWeight(layerSizes(0, i), layerSizes(0, i+1));
        Matrix tempBias(1, layerSizes(0, i+1));
        
        tempWeight.initRand(-1, 1);
        tempBias.initRand(-1, 1);
        
        weights_.push_back(tempWeight);
        biases_.push_back(tempBias);
    }
    fitness_ = 0;
}

//Prints the current weights to the console
void NeuralNet::printWeights() const{
    std::cout << "Current weights:" << std::endl;
    for(int i = 0; i < weights_.size(); ++i){
        std::cout << "========================" << std::endl;
        weights_[i].printData();
    }
    std::cout << "========================" << std::endl;
}

//Performs forward propagation using weights_ and 'input'
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

//Sets the internal weights
void NeuralNet::setWeights(const std::vector<Matrix> weights){
    if (weights.size() == 0 || weights.size() != weights_.size()){
        throw std::out_of_range("Weights have different sizes");
    }
    for(int i = 0; i < weights_.size(); ++i){
        weights_[i] = weights[i];
    }
}

//Applies the nonlinearity function (sigmoid) elementwise
Matrix NeuralNet::applyNonlinearity(Matrix a) const{
    Matrix temp(a);
    for(int row = 0; row < a.numRows(); ++row){
        for(int col = 0; col < a.numCols(); ++col){
            temp(row, col) = sigmoid(a(row, col));
        }
    }
    return temp;
}

//Sigmoid function. Returns a double between (0, 1)
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

