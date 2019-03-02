#ifndef NN_H
#define NN_H

#include <Eigen/Dense>
using namespace Eigen;

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::string;
using std::max;
using std::exp;
using std::log;

enum Activations { sigmoid, relu, softmax };

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

	inline RowVectorXd applyNonlinearity(const RowVectorXd& input,
		Activations activation) const;
	static inline double relu(double x);
	static inline double sigmoid(double x);
};

#endif
