#ifndef NN_H
#define NN_H

#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace Eigen;

enum Activations { sigmoid, relu, softmax };

class NeuralNet {
 public:
  NeuralNet();
  NeuralNet(const std::vector<unsigned int> &layerSizes);
  NeuralNet(const NeuralNet &nn);

  RowVectorXd forward(const RowVectorXd &input) const;

  void printWeights() const;

  void operator=(const NeuralNet &nn);

  std::vector<MatrixXd> &getWeights();
  void setWeights(const std::vector<MatrixXd> &weights);

  bool saveToFile(std::string fileName) const;
  bool loadFromFile(std::string fileName);

 private:
  std::vector<unsigned int> m_layerSizes;
  std::vector<MatrixXd> m_weights;

  inline RowVectorXd applyNonlinearity(const RowVectorXd &input,
                                       Activations activation) const;
  static inline double relu(double x);
  static inline double sigmoid(double x);
};

#endif
