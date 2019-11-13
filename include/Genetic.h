#ifndef GENETIC_H
#define GENETIC_H

#include <Eigen/Dense>
#include <random>
using namespace Eigen;
#include "NeuralNet.h"
#include "Player.h"

class Genetic {
 public:
  static void Breed(std::vector<Player *> *population, float greedyPercent);
  static void Mutate(std::vector<Player *> *population, float greedyPercent,
                     float mutationRate);

 private:
  static std::vector<MatrixXd> crossOver(std::vector<MatrixXd> &parent1,
                                         std::vector<MatrixXd> &parent2);
  static NeuralPlayer *pickParent(std::vector<Player *> *population);
};

#endif
