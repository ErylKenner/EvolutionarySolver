#ifndef GENETIC_H
#define GENETIC_H

#include <iostream>
#include <random>

#include <Eigen/Dense>
using namespace Eigen;

#include "NeuralNet.h"
#include "Player.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

class Genetic {
 public:
  // Genetic(const float mutationRate, const float greedyPercent);

  // void setPopulationSize(int populationSize);

  // void setMutationRate(const float mutationRate);
  // void setGreedyPercent(const float greedyPercent);

  static void Breed(vector<Player *> *population, float greedyPercent);
  static void Mutate(vector<Player *> *population, float greedyPercent,
                     float mutationRate);

 private:
  static vector<MatrixXd> crossOver(vector<MatrixXd> &parent1,
                                    vector<MatrixXd> &parent2);

  static NeuralPlayer *pickParent(vector<Player *> *population);

  // float m_mutationRate;
  // int m_populationSize;
  // float m_greedyPercent;
};

#endif
