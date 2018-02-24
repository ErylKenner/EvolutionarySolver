
#include "NeuralNet.h"



//Constructor takes in the structure of the network as a matrix
NeuralNet::NeuralNet(const std::vector<unsigned int>& layerSizes)
    : m_layerSizes(layerSizes){

    //Create vectors for weights and biases. Each entry is a matrix for that layer
    for(unsigned int i = 0; i < layerSizes.size() - 1; ++i){
        Matrix tempWeight(layerSizes[i], layerSizes[i+1]);
        Matrix tempBias(1, layerSizes[i+1]);
        
        tempWeight.initRand(-1, 1);
        tempBias.initRand(-1, 1);
        
        m_weights.push_back(tempWeight);
        m_biases.push_back(tempBias);
    }
    m_fitness = 0;
}

//Prints the current weights to the console
void NeuralNet::printWeights() const{
    std::cout << "Current weights:" << std::endl;
    for(unsigned int i = 0; i < m_weights.size(); ++i){
        std::cout << "========================" << std::endl;
        m_weights[i].printData();
    }
    std::cout << "========================" << std::endl;
}

//Performs forward propagation using m_weights and 'input'
Matrix NeuralNet::forward(const Matrix& input) const{
	//Stores the previous layer's output
    std::vector<Matrix> prev;
    prev.push_back(input);

    for(unsigned int lay = 0; lay < m_weights.size(); ++lay){
    	//Cur = f(prev * weights + bias) .... where f(x) is nonlinearity funtion
        prev.push_back( applyNonlinearity(prev.back() * m_weights[lay] + m_biases[lay], sigmoid) );
    }
    return prev.back();
}

std::vector<Matrix> NeuralNet::getWeights() const{
    return m_weights;
}

//Sets the internal weights
void NeuralNet::setWeights(const std::vector<Matrix> weights){
    if (weights.size() == 0 || weights.size() != m_weights.size()){
    	std::cerr << "Error: setWeights(): Weights have different sizes." << std::endl;
        exit(1);
    }
    for(unsigned int i = 0; i < m_weights.size(); ++i){
        m_weights[i] = weights[i];
    }
}

//Applies the nonlinearity function (sigmoid) elementwise
Matrix NeuralNet::applyNonlinearity(const Matrix& input, double(*callback)(double)) const{
    Matrix temp(input);
    for(unsigned int row = 0; row < input.numRows(); ++row){
        for(unsigned int col = 0; col < input.numCols(); ++col){
        	//Applies the callback to each element of input
            temp(row, col) = callback(input(row, col));
        }
    }
    return temp;
}

//Sigmoid function. Returns a double between (0, 1)
double NeuralNet::sigmoid(const double x){
    return 1 / (1 + exp(-x));
}

void NeuralNet::resetFitness(){
    m_fitness = 0.0;
}

void NeuralNet::addToFitness(const double a){
    m_fitness += a;
}

double NeuralNet::getFitness() const{
    return m_fitness;
}

bool NeuralNet::compFitness(const NeuralNet& player1, const NeuralNet& player2){
    return player1.getFitness() < player2.getFitness();
}
