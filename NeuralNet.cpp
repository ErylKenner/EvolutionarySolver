
#include "NeuralNet.h"



//Constructor takes in the structure of the network as a matrix
NeuralNet::NeuralNet(const vector<unsigned int>& layerSizes)
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
}

NeuralNet::NeuralNet(const NeuralNet& nn)
    : m_layerSizes(nn.m_layerSizes)
    , m_weights(nn.m_weights)
    , m_biases(nn.m_biases){
    
}

void NeuralNet::operator= (const NeuralNet& nn){
    m_layerSizes = nn.m_layerSizes;
    m_weights = nn.m_weights;
    m_biases = nn.m_biases;
}

//Prints the current weights to the console
void NeuralNet::printWeights() const{
    cout << "Current weights:" << endl;
    for(unsigned int i = 0; i < m_weights.size(); ++i){
        cout << "================================================" << endl;
        m_weights[i].printData();
    }
    cout << "================================================" << endl;
}

//Performs forward propagation using m_weights and 'input'
Matrix NeuralNet::forward(const Matrix& input) const{
	//Stores the previous layer's output
    vector<Matrix> layers;
    layers.push_back(input);

    for(unsigned int lay = 0; lay < m_weights.size(); ++lay){
    	//Cur = f(layers * weights + bias) .... where f(x) is nonlinearity funtion
        layers.push_back( applyNonlinearity(layers.back() * m_weights[lay] + m_biases[lay], sigmoid) );
    }
    return layers.back();
}

vector<Matrix> NeuralNet::getWeights() const{
    return m_weights;
}

//Sets the internal weights
void NeuralNet::setWeights(const vector<Matrix>& weights, const vector<Matrix>& biases){
    if (weights.size() == 0 || weights.size() != m_weights.size()){
    	cerr << "Error: setWeights(): Weights have different sizes." << endl;
        exit(1);
    }
    if (biases.size() == 0 || biases.size() != m_biases.size()){
        cerr << "Error: setWeights(): Biases have different sizes." << endl;
        exit(1);
    }
    //Set weights
    for(unsigned int i = 0; i < m_weights.size(); ++i){
        m_weights[i] = weights[i];
    }
    //Set biases
    for(unsigned int i = 0; i < m_biases.size(); ++i){
        m_biases[i] = biases[i];
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
