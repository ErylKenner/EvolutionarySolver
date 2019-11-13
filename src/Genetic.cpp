#include "Genetic.h"

// mutationRate is in the range [0, 1], greedyPercent is in the range [0, 1]
/*Genetic::Genetic(const float mutationRate, const float greedyPercent)
    : m_mutationRate(clamp(mutationRate, 0.0f, 1.0f))
    , m_greedyPercent(clamp(greedyPercent, 0.0f, 1.0f)) {
    m_populationSize = 0;
}

void Genetic::setPopulationSize(int populationSize) {
    m_populationSize = populationSize;
}*/

// Make new players based on how successful the current ones are
void Genetic::Breed(vector<Player *> *population, float greedyPercent) {
  unsigned int populationSize = population->size();
  vector<NeuralPlayer> newPop;
  newPop.reserve(populationSize);

  // Copy the players which are being kept from greedyPercent
  int numToKeep = (int)(greedyPercent * populationSize + 0.5f);
  for (int i = 0; i < numToKeep; ++i) {
    NeuralPlayer *temp =
        static_cast<NeuralPlayer *>((*population)[populationSize - 1 - i]);
    newPop.emplace_back(*temp);
  }

  // Iterates over the remaining child elements
  for (unsigned int i = numToKeep; i < populationSize; ++i) {
    NeuralPlayer *parent1 = Genetic::pickParent(population);
    NeuralPlayer *parent2 = Genetic::pickParent(population);
    vector<MatrixXd> newWeights = Genetic::crossOver(
        parent1->neural.getWeights(), parent2->neural.getWeights());

    NeuralPlayer *temp =
        static_cast<NeuralPlayer *>((*population)[populationSize - 1 - i]);
    NeuralPlayer temp2 = *temp;
    temp2.neural.setWeights(newWeights);
    newPop.emplace_back(temp2);
  }

  // Copies new weights to population
  for (unsigned int i = 0; i < populationSize; ++i) {
    NeuralPlayer *temp = static_cast<NeuralPlayer *>((*population)[i]);
    temp->neural.setWeights(newPop[i].neural.getWeights());
  }
}

void Genetic::Mutate(vector<Player *> *population, float greedyPercent,
                     float mutationRate) {
  unsigned int populationSize = population->size();
  // Intialize random object for gaussian distribution (mean=0)
  std::random_device rd{};
  std::mt19937 gen{rd()};
  // 99.8% chance of value being in the range [-interval, interval]
  const double interval = 0.08;
  std::normal_distribution<double> distribution(0, interval * 0.324675);

  int numToKeep = (int)(greedyPercent * populationSize + 0.5f);
  for (unsigned int i = numToKeep; i < populationSize; ++i) {
    NeuralPlayer *temp = static_cast<NeuralPlayer *>((*population)[i]);
    vector<MatrixXd> &weights = temp->neural.getWeights();
    for (size_t layer = 0; layer < weights.size(); ++layer) {
      // Randomly mutate each element
      for (int col = 0; col < weights[layer].cols(); ++col) {
        for (int row = 0; row < weights[layer].rows(); ++row) {
          weights[layer](row, col) += distribution(gen);
        }
      }
    }
  }
}

NeuralPlayer *Genetic::pickParent(vector<Player *> *population) {
  unsigned int populationSize = population->size();
  double best = (*population)[populationSize - 1]->fitness;
  double total = 0;
  for (unsigned int i = 0; i < populationSize; ++i) {
    total += (*population)[i]->fitness;
  }

  int threshold = rand() % (int)total;
  double sum = 0;
  for (int i = populationSize - 1; i >= 0; --i) {
    sum += (*population)[i]->fitness;
    if (sum >= threshold) {
      return static_cast<NeuralPlayer *>((*population)[i]);
    }
  }
  // Default, return the highest fitness player
  return static_cast<NeuralPlayer *>((*population)[populationSize - 1]);
}

vector<MatrixXd> Genetic::crossOver(vector<MatrixXd> &parent1,
                                    vector<MatrixXd> &parent2) {
  vector<MatrixXd> newWeights = parent1;

  /*int totalParams = 0;
  for (size_t i = 0; i < weights1.size(); ++i) {
    totalParams += weights1[i].rows() * weights1[i].cols();
  }

  int crossoverPoint = rand() % totalParams;

  int total = 0;
  for (size_t i = 0; i < weights1.size(); ++i) {
    int rows = weights1[i].rows();
    int cols = weights1[i].cols();
    if (total >= crossoverPoint - rows * cols) {
      for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
          if (total + row * cols + col >= crossoverPoint) {
            weights1[i](row, col) = weights2[i](row, col);
          }
        }
      }
    }
    total += rows * cols;
  }*/
  for (size_t layer = 0; layer < parent1.size(); ++layer) {
    for (int col = 0; col < parent1[layer].cols(); ++col) {
      for (int row = 0; row < parent1[layer].rows(); ++row) {
        if (rand() % 2) {
          newWeights[layer](row, col) = parent1[layer](row, col);
        } else {
          newWeights[layer](row, col) = parent2[layer](row, col);
        }
      }
    }
  }

  return newWeights;
}

/*(void Genetic::setMutationRate(const float mutationRate) {
  m_mutationRate = clamp(mutationRate, 0.0f, 1.0f);
}

void Genetic::setGreedyPercent(const float greedyPercent) {
  m_greedyPercent = clamp(greedyPercent, 0.0f, 1.0f);
}*/
