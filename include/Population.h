
#ifndef POPULATION_H
#define POPULATION_H

#include <Eigen/Dense>
#include <chrono>
using namespace Eigen;
#include "Genetic.h"

struct Statistics {
  double winPercent;
  double lossPercent;
  double tiePercent;
};

class Population {
 public:
  Population();
  ~Population();
  void Init(int numActions, std::istream &is = std::cin,
            std::ostream &os = std::cout);
  bool SaveBestPlayer(std::string path);
  Player *LoadPlayerFromFile(std::string path);

  template <class Game>
  double Train(bool verbose);

  template <class Game>
  void PlayBest();

 private:
  int m_populationSize;
  int m_iterations;
  int m_gamesToSimulate;
  std::vector<Player *> m_population;
  std::vector<Player *> m_hallOfFame;

  template <class Game>
  void playTestGame(Player *loadedPlayer);

  template <class Game>
  void roundRobin();

  template <class Game>
  void playGames(Player *opponent);

  template <class Game>
  Statistics playHallOfFame(Player *player);

  void printSummary(const int generation, Statistics stats) const;
  void printPopulationFrom(const unsigned int start,
                           const unsigned int end) const;
};

//--------------------------------FUNCTIONS--------------------------------

Population::Population()
    : m_populationSize(0), m_iterations(0), m_gamesToSimulate(0) {}

Population::~Population() {
  for (unsigned int i = 0; i < m_population.size(); ++i) {
    delete m_population[i];
    m_population[i] = NULL;
  }
  for (unsigned int i = 0; i < m_hallOfFame.size(); ++i) {
    delete m_hallOfFame[i];
    m_hallOfFame[i] = NULL;
  }
}

void Population::Init(int numActions, std::istream &is, std::ostream &os) {
  // Get population size
  os << "Population size: ";
  is >> m_populationSize;
  if (m_populationSize < 2 || std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    m_populationSize = 2;
  }

  // Get number of iterations
  os << "Iterations: ";
  is >> m_iterations;
  if (m_iterations < 1 || std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    m_iterations = 1;
  }

  // Get number of games to simulate
  os << "Games to simulate per player: ";
  is >> m_gamesToSimulate;
  if (m_gamesToSimulate < 1 || std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    m_gamesToSimulate = 1;
  }

  // Get number of layers
  int hiddenLayers;
  os << "Number of hidden layers: ";
  is >> hiddenLayers;
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    hiddenLayers = 0;
  }

  // Populate m_layerSizes
  std::vector<unsigned int> m_layerSizes;
  m_layerSizes.push_back(numActions);
  for (int i = 0; i < hiddenLayers; ++i) {
    os << "Number in hidden layer " << i + 1 << ": ";
    unsigned int layerSize;
    is >> layerSize;
    if (layerSize < 1 || std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore();
      layerSize = 1;
    }
    m_layerSizes.push_back(layerSize);
  }
  m_layerSizes.push_back(1);

  // Instantiate the Players
  m_population.reserve(m_populationSize);
  for (int i = 0; i < m_populationSize; ++i) {
    m_population.push_back(new NeuralPlayer(m_layerSizes));
  }

  m_hallOfFame.reserve(m_iterations);
  os << std::endl << std::endl;
}

template <class Game>
double Population::Train(bool verbose) {
  using namespace std::chrono;
  auto startTime = steady_clock::now();

  enum class TrainingStage { PlayRandom, RoundRobin, Both };
  TrainingStage stage = TrainingStage::PlayRandom;
  float greedyPercent = 0.02f;
  float mutationRate = 0.05f;

  std::cout << "STAGE 1: RANDOM PLAYERS" << std::endl;
  for (int generation = 0; generation < m_iterations; ++generation) {
    Player *opponent = new RandomPlayer(Game::NUM_ACTIONS);
    switch (stage) {
      case TrainingStage::PlayRandom:
        playGames<Game>(opponent);
        break;
      case TrainingStage::Both:
        roundRobin<Game>();
        playGames<Game>(opponent);
        break;
      case TrainingStage::RoundRobin:
        roundRobin<Game>();
      default:
        break;
    }
    delete opponent;

    sort(m_population.begin(), m_population.end(), Player::ComparePlayer);
    NeuralPlayer *curBest = static_cast<NeuralPlayer *>(m_population.back());
    if (curBest == NULL) {
      throw new std::bad_cast();
    }
    m_hallOfFame.push_back(new NeuralPlayer(*curBest));
    if (verbose) {
      curBest->neural.printWeights();
    }

    Statistics stats = playHallOfFame<Game>(m_population.back());
    printSummary(generation, stats);

    // Stage selection
    switch (stage) {
      case TrainingStage::PlayRandom:
        if (generation == m_iterations - 1) {
          stage = TrainingStage::Both;
          generation = 0;
          mutationRate = 0.03f;
          greedyPercent = 0.05f;
          std::cout << "MOVING TO STAGE 2: ROUND ROBIN & RANDOM PLAYERS"
                    << std::endl;
        }
        break;
      case TrainingStage::Both:
        if (generation == m_iterations - 1) {
          stage = TrainingStage::RoundRobin;
          generation = 0;
          mutationRate = 0.01f;
          greedyPercent = 0.08f;
          std::cout << "MOVING TO STAGE 3: ROUND ROBIN" << std::endl;
        }
        break;
      default:
        break;
    }

    Genetic::Breed(&m_population, greedyPercent);
    Genetic::Mutate(&m_population, greedyPercent, mutationRate);

    // Reset fitness values for next generation
    for (int i = 0; i < m_populationSize; ++i) {
      m_population[i]->fitness = 0.0f;
    }
  }
  auto endTime = steady_clock::now();
  return duration_cast<milliseconds>(endTime - startTime).count() / 1000.0;
}

bool Population::SaveBestPlayer(std::string path) {
  NeuralPlayer *playerNeural =
      dynamic_cast<NeuralPlayer *>(m_population.back());
  if (playerNeural == NULL) {
    throw new std::bad_cast();
  }
  return playerNeural->neural.saveToFile(path);
}

template <class Game>
void Population::playTestGame(Player *loadedPlayer) {
  ManualPlayer human(std::cin, std::cout, Game::NUM_ACTIONS);
  Game testGame1(&human, loadedPlayer, true);
  testGame1.playGame();
  Game testGame2(loadedPlayer, &human, true);
  testGame2.playGame();
}

template <class Game>
void Population::roundRobin() {
  for (int i = 0; i < m_populationSize - 1; ++i) {
    for (int j = i + 1; j < m_populationSize; ++j) {
      Game game1(m_population[i], m_population[j], false);
      game1.playGame();
      Game game2(m_population[j], m_population[i], false);
      game2.playGame();
    }
  }
}

template <class Game>
void Population::playGames(Player *opponent) {
  for (int i = 0; i < m_populationSize; ++i) {
    Game game1(m_population[i], opponent, false);
    Game game2(opponent, m_population[i], false);
    for (int j = 0; j <= m_gamesToSimulate / 2; ++j) {
      game1.playGame();
      game1.Reset();
      game2.playGame();
      game2.Reset();
    }
  }
}

template <class Game>
Statistics Population::playHallOfFame(Player *best) {
  int numOpponents = m_hallOfFame.size() - 1;
  double initialFitness = best->fitness;
  int numWins = 0;
  int numTies = 0;
  int numLoss = 0;
  for (int i = 0; i < numOpponents; ++i) {
    best->fitness = 0.0f;
    Game game1(m_hallOfFame[i], best, false);
    game1.playGame();

    // Check if best won or not
    if (best->fitness > 1.0) {
      numWins++;
    } else if (best->fitness >= 0.5) {
      numTies++;
    } else {
      numLoss++;
    }

    best->fitness = 0;
    Game game2(best, m_hallOfFame[i], false);
    game2.playGame();

    // Check if best won or not
    if (best->fitness > 1.0) {
      numWins++;
    } else if (best->fitness >= 0.5) {
      numTies++;
    } else {
      numLoss++;
    }
  }
  Statistics ret;
  ret.winPercent = 100.0 * numWins / (2 * numOpponents);
  ret.lossPercent = 100.0 * numLoss / (2 * numOpponents);
  ret.tiePercent = 100.0 * numTies / (2 * numOpponents);

  // Set the best player's fitness to what it was before this function
  best->fitness = initialFitness;
  return ret;
}

void Population::printSummary(const int generation, Statistics stats) const {
  Player *minPlayer = m_population[0];
  Player *medPlayer = m_population[m_populationSize / 2];
  Player *maxPlayer = m_population[m_populationSize - 1];

  printf("Gen: %3d", generation);
  printf(", Min: %-6.1f [i=%-3d]", minPlayer->fitness, minPlayer->index);
  printf(", Median: %-6.1f [i=%-3d]", medPlayer->fitness, medPlayer->index);
  printf(", Max: %-6.1f [i=%-3d]", maxPlayer->fitness, maxPlayer->index);
  printf("  HOF: ");
  printf("W: %.2lf%%, ", stats.winPercent);
  printf("L: %.2lf%%, ", stats.lossPercent);
  printf("T: %.2lf%%", stats.tiePercent);
  std::cout << std::endl;
}

void Population::printPopulationFrom(const unsigned int start,
                                     const unsigned int lastIndex) const {
  if (start > lastIndex) {
    std::cerr << "Error: start is greater than lastIndex" << std::endl;
    exit(1);
  }
  if (lastIndex > m_population.size() - 1) {
    std::cerr << "Error: lastIndex is larger than population's last index"
              << std::endl;
    exit(1);
  }
  for (unsigned int i = start; i <= lastIndex; ++i) {
    std::cout << "Population[" << m_population[i]->index << "] fitness: ";
    printf("%5.1f\n", m_population[i]->fitness);
  }
}

template <class Game>
void Population::PlayBest() {
  playTestGame<Game>(m_population.back());
}

Player *Population::LoadPlayerFromFile(std::string path) {
  NeuralPlayer *temp = new NeuralPlayer();
  temp->neural.loadFromFile(path);
  return temp;
}

#endif
