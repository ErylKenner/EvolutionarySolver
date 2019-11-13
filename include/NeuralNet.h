#ifndef NN_H
#define NN_H

#include <Eigen/Dense>
using namespace Eigen;

#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::exp;
using std::ifstream;
using std::log;
using std::max;
using std::ofstream;
using std::string;
using std::vector;

enum Activations { sigmoid, relu, softmax };

class NeuralNet {
 public:
  NeuralNet();
  NeuralNet(const vector<unsigned int> &layerSizes);
  NeuralNet(const NeuralNet &nn);

  RowVectorXd forward(const RowVectorXd &input) const;

  void printWeights() const;

  void operator=(const NeuralNet &nn);

  vector<MatrixXd> &getWeights();
  void setWeights(const vector<MatrixXd> &weights);

  bool saveToFile(string fileName) const;
  bool loadFromFile(string fileName);

 private:
  vector<unsigned int> m_layerSizes;
  vector<MatrixXd> m_weights;

  inline RowVectorXd applyNonlinearity(const RowVectorXd &input,
                                       Activations activation) const;
  static inline double relu(double x);
  static inline double sigmoid(double x);
};

#endif
