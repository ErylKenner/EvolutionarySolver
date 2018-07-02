#ifndef NN_H
#define NN_H

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::string;

enum Activations { sigmoid, relu };

class NeuralNet {


public:
    NeuralNet();
    NeuralNet(const vector<unsigned int>& layerSizes);
    NeuralNet(const NeuralNet& nn);

    RowVectorXd forward(const RowVectorXd& input) const;

    void printWeights() const;

    void operator= (const NeuralNet& nn);

    vector<MatrixXd> getWeights() const;
    void setWeights(const vector<MatrixXd>& weights);

    bool saveToFile(string fileName) const;
    bool loadFromFile(string fileName);
private:
    vector<unsigned int> m_layerSizes;
    vector<MatrixXd> m_weights;

    RowVectorXd applyNonlinearity(const RowVectorXd& input,
                                  Activations activation) const;
};

#endif
