
#include "NeuralNet.h"


NeuralNet::NeuralNet(){
    
}

//Constructor takes in the structure of the network as a matrix
NeuralNet::NeuralNet(const vector<unsigned int>& layerSizes)
    : m_layerSizes(layerSizes){

    //Create vectors for weights. Each entry is a matrix for that layer
    for(unsigned int i = 0; i < layerSizes.size() - 1; ++i){
        Matrix tempWeight(layerSizes[i] + 1, layerSizes[i+1]);
        tempWeight.initRand(-1, 1);
        m_weights.push_back(tempWeight);
    }
}

NeuralNet::NeuralNet(const NeuralNet& nn)
    : m_layerSizes(nn.m_layerSizes)
    , m_weights(nn.m_weights){
    
}

void NeuralNet::operator= (const NeuralNet& nn){
    m_layerSizes = nn.m_layerSizes;
    m_weights = nn.m_weights;
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

bool NeuralNet::saveToFile(string fileName) const{
    ofstream outputFile;
    outputFile.open(fileName.c_str());
    if(!outputFile.is_open()){
        return false;
    }
    
    //Outuput number of layer
    outputFile << m_layerSizes.size() << "\n";
    
    //Ouput layerSizes
    for(unsigned int i = 0; i < m_layerSizes.size(); ++i){
        outputFile << m_layerSizes[i] << " ";
    }
    outputFile << "\n";
    
    for(unsigned int lay = 0; lay < m_weights.size(); ++lay){
        unsigned int numRows = m_weights[lay].numRows();
        unsigned int numCols = m_weights[lay].numCols();
        
        for(unsigned int i = 0; i < numRows; ++i){
            for(unsigned int j = 0; j < numCols; ++j){
                Matrix cur = m_weights[lay];
                outputFile << cur(i, j) << " ";
            }
        }
    }
    
    outputFile << "D\n";
    outputFile.close();
    return true;
}

bool NeuralNet::loadFromFile(string fileName){
    ifstream inputFile;
    inputFile.open(fileName.c_str());
    if(!inputFile.is_open()){
        return false;
    }
    
    unsigned int numLayers;
    inputFile >> numLayers;
    
    m_layerSizes.clear();
    for(unsigned int i = 0; i < numLayers; ++i){
        unsigned int cur;
        inputFile >> cur;
        m_layerSizes.push_back(cur);
    }
    
    for(unsigned int lay = 0; lay < numLayers - 1; ++lay){
        Matrix cur(m_layerSizes[lay] + 1, m_layerSizes[lay + 1]);
        for(unsigned int i = 0; i < cur.numRows(); ++i){
            for(unsigned int j = 0; j < cur.numCols(); ++j){
                double temp;
                inputFile >> temp;
                cur(i, j) = temp;
            }
        }
        m_weights.push_back(cur);
    }
    
    char check;
    inputFile >> check;
    if(check != 'D'){
        cerr << "Error: Check at the end of input file was incorrect" << endl;
        return false;
    }
    
    
    inputFile.close();
    return true;
}

//Performs forward propagation using m_weights and 'input'
Matrix NeuralNet::forward(const Matrix& input) const{
	//Stores the previous layer's output
    vector<Matrix> layers;
    layers.push_back(input);

    for(unsigned int lay = 0; lay < m_weights.size(); ++lay){
        //Add extra col with 1.0 in it for bias
        Matrix prev = (layers.back()).addRowsCols(0, 1, 1.0f);
        
    	//Cur = f(layers * weights + bias) .... where f(x) is nonlinearity funtion
        layers.push_back( applyNonlinearity(prev * m_weights[lay], sigmoid) );
    }
    return layers.back();
}

vector<Matrix> NeuralNet::getWeights() const{
    return m_weights;
}

//Sets the internal weights
void NeuralNet::setWeights(const vector<Matrix>& weights){
    if (weights.size() == 0 || weights.size() != m_weights.size()){
    	cerr << "Error: setWeights(): Weights have different sizes." << endl;
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
