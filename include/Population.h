
#ifndef POPULATION_H
#define POPULATION_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include <Eigen/Dense>
using namespace Eigen;

#include "Genetic.h"
#include "NeuralNet.h"
#include "Player.h"
#include "TicTacToe.h"
//#include "UltimateTTT.h"

using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

template <class Game>
class Population {
 public:
  Population();
  ~Population();

  void Init(istream &is = cin, ostream &os = cout);
  time_t Train(bool verbose);

  Player *LoadPlayer(const string path);
  bool SavePlayer(const string path);
  bool PlayBest();

 private:
  int m_populationSize;
  int m_iterations;
  int m_gamesToSimulate;

  vector<Player *> m_population;
  vector<Player *> m_hallOfFame;

  //--------------------------------FUNCTIONS--------------------------------

  void playTestGame(Player *loadedPlayer);
  Player *loadPlayerFromFile(const string path, string name);
  string savePlayerToFile(const Player *best, const string path) const;

  void roundRobin();
  void playGames(Player *opponent);

  void playHallOfFame(Player *player, double *winPercent, double *lossPercent,
                      double *tiePercent);
  void printSummary(const int generation, const double HOF_win_percent,
                    const double HOF_loss_percent,
                    const double HOF_tie_percent) const;
  void printPopulationFrom(const unsigned int start,
                           const unsigned int end) const;
};

template <class Game>
Population<Game>::Population() {}

template <class Game>
Population<Game>::~Population() {
  for (unsigned int i = 0; i < m_population.size(); ++i) {
    delete m_population[i];
  }
  for (unsigned int i = 0; i < m_hallOfFame.size(); ++i) {
    delete m_hallOfFame[i];
  }
}

template <class Game>
void Population<Game>::Init(istream &is, ostream &os) {
  // Get population size
  os << "Population size: ";
  is >> m_populationSize;
  if (m_populationSize < 2 || cin.fail()) {
    cin.clear();
    cin.ignore();
    m_populationSize = 2;
  }

  // Get number of iterations
  os << "Iterations: ";
  is >> m_iterations;
  if (m_iterations < 1 || cin.fail()) {
    cin.clear();
    cin.ignore();
    m_iterations = 1;
  }

  // Get number of games to simulate
  os << "Games to simulate per player: ";
  is >> m_gamesToSimulate;
  if (m_gamesToSimulate < 1 || cin.fail()) {
    cin.clear();
    cin.ignore();
    m_gamesToSimulate = 1;
  }

  // Get number of layers
  int hiddenLayers;
  os << "Number of hidden layers: ";
  is >> hiddenLayers;
  if (cin.fail()) {
    cin.clear();
    cin.ignore();
    hiddenLayers = 0;
  }

  // Populate m_layerSizes
  vector<unsigned int> m_layerSizes;
  m_layerSizes.push_back(Game::NUM_OUTPUTS);
  for (int i = 0; i < hiddenLayers; ++i) {
    os << "Number in hidden layer " << i + 1 << ": ";
    unsigned int layerSize;
    is >> layerSize;
    if (layerSize < 1 || cin.fail()) {
      cin.clear();
      cin.ignore();
      layerSize = 1;
    }
    m_layerSizes.push_back(layerSize);
  }
  m_layerSizes.push_back(1);

  // Instantiate the Players
  m_population.reserve(m_populationSize);
  for (int i = 0; i < m_populationSize; ++i) {
    m_population.emplace_back(new NeuralPlayer(m_layerSizes));
  }

  m_hallOfFame.reserve(m_iterations);
  os << endl << endl;
}

template <class Game>
time_t Population<Game>::Train(bool verbose) {
  time_t startTime = time(NULL);
  int trainingStage = 1;
  float greedyPercent = 0.02f;
  float mutationRate = 0.05f;

  cout << "STAGE 1: RANDOM PLAYERS" << endl;
  for (int generation = 0; generation < m_iterations; ++generation) {
    Player *opponent = new RandomPlayer(9);
    switch (trainingStage) {
      case 1:
        playGames(opponent);
        break;
      case 2:
        roundRobin();
        playGames(opponent);
        break;
      case 3:
        roundRobin();
      default:
        break;
    }
    delete opponent;

    // Sorts the players by fitness (ascending)
    sort(m_population.begin(), m_population.end(), Player::ComparePlayer);

    NeuralPlayer *curBest = dynamic_cast<NeuralPlayer *>(m_population.back());

    if (curBest == NULL) {
      throw new std::bad_cast();
    }

    // Copy best player and add them to the hall of fame
    m_hallOfFame.emplace_back(new NeuralPlayer(*curBest));

    if (verbose) {
      curBest->neural.printWeights();
    }

    // Test the current best vs. the best from each previous generation
    double winPercent;
    double lossPercent;
    double tiePercent;
    playHallOfFame(m_population.back(), &winPercent, &lossPercent, &tiePercent);
    printSummary(generation, winPercent, lossPercent, tiePercent);

    // Stage selection
    double highestFit = m_population[m_populationSize - 1]->fitness;
    double medFit = m_population[m_populationSize / 2]->fitness;

    switch (trainingStage) {
      case 1:
        if ((highestFit >= 1.2 * m_populationSize &&
             medFit >= 0.95 * m_populationSize) ||
            generation == m_iterations - 1) {
          trainingStage = 2;
          generation = 0;
          mutationRate = 0.03f;
          greedyPercent = 0.05f;
          cout << "MOVING TO STAGE 2: ROUND ROBIN & RANDOM PLAYERS" << endl;
        }
        break;
      case 2:
        if (generation == m_iterations - 1) {
          trainingStage = 3;
          generation = 0;
          mutationRate = 0.01f;
          greedyPercent = 0.08f;
          cout << "MOVING TO STAGE 3: ROUND ROBIN" << endl;
        }
        break;
      default:
        break;
    }

    // Make new players based on how successful the current ones are
    Genetic::Breed(&m_population, greedyPercent);

    // Each weight has a small chance to change by some random value
    Genetic::Mutate(&m_population, greedyPercent, mutationRate);

    // Reset fitness values for next generation
    for (int i = 0; i < m_populationSize; ++i) {
      m_population[i]->fitness = 0.0f;
    }
  }
  return time(NULL) - startTime;
}

template <class Game>
string Population<Game>::savePlayerToFile(const Player *player,
                                          const string path) const {
  string fileName;
  while (true) {
    cout << "Player datafile name (saved to '" << path << "'): " << endl;
    cin >> fileName;

    NeuralPlayer *playerNeural = dynamic_cast<NeuralPlayer *>(player);
    if (playerNeural == NULL) {
      throw new std::bad_cast();
    }

    bool success = player->neural.saveToFile(path + fileName);
    if (!success) {
      cout << "Invalid file name or path. Please try again." << endl;
      continue;
    }
  }
  return fileName;
}

template <class Game>
Player *Population<Game>::loadPlayerFromFile(const string path, string name) {
  NeuralPlayer *temp = new NeuralPlayer();
  temp->neural.loadFromFile(path + name);
  return temp;
}

template <class Game>
void Population<Game>::playTestGame(Player *loadedPlayer) {
  ManualPlayer human(cin, cout, 3, 3);

  // Play each other
  Game testGameA(&human, loadedPlayer, true);
  testGameA.playGame();

  Game testGameB(loadedPlayer, &human, true);
  testGameB.playGame();
}

template <class Game>
void Population<Game>::roundRobin() {
  for (int i = 0; i < m_populationSize - 1; ++i) {
    for (int j = i + 1; j < m_populationSize; ++j) {
      // Game 1
      /*cout << "Game between [" << population[i].second << "] \
      and [" << population[j].second << "]" << endl;*/
      Game gameA(m_population[i], m_population[j], false);
      gameA.playGame();

      // Game 2 (play 2 games so both players can start first)
      /*cout << "Game between [" << population[j].second << "] \
      and [" << population[i].second << "]" << endl;*/
      Game gameB(m_population[j], m_population[i], false);
      gameB.playGame();
    }
  }
}

template <class Game>
void Population<Game>::playGames(Player *opponent) {
  for (int i = 0; i < m_populationSize; ++i) {
    Game gameA(m_population[i], opponent, false);
    Game gameB(opponent, m_population[i], false);
    for (int j = 0; j <= m_gamesToSimulate / 2; ++j) {
      gameA.playGame();
      gameA.Reset();
      gameB.playGame();
      gameB.Reset();
    }
  }
}

template <class Game>
void Population<Game>::playHallOfFame(Player *best, double *winPercent,
                                      double *lossPercent, double *tiePercent) {
  int numOpponents = m_hallOfFame.size() - 1;
  double initialFitness = best->fitness;

  double numWins = 0.0;
  double numTies = 0.0;
  double numLoss = 0.0;

  for (int i = 0; i < numOpponents; ++i) {
    // Game 1
    best->fitness = 0.0f;
    Game gameA(m_hallOfFame[i], best, false);
    gameA.playGame();

    // Check if best won or not
    if (best->fitness > 1.0) {
      numWins++;
    } else if (best->fitness >= 0.5) {
      numTies++;
    } else {
      numLoss++;
    }

    // Game 2 (so both players can start first)
    best->fitness = 0;
    Game gameB(best, m_hallOfFame[i], false);
    gameB.playGame();

    // Check if best won or not
    if (best->fitness > 1.0) {
      numWins++;
    } else if (best->fitness >= 0.5) {
      numTies++;
    } else {
      numLoss++;
    }
  }

  *winPercent = 100.0 * numWins / (2 * numOpponents);
  *tiePercent = 100.0 * numTies / (2 * numOpponents);
  *lossPercent = 100.0 * numLoss / (2 * numOpponents);

  // Set the best player's fitness to what it was before this function
  best->fitness = initialFitness;
}

template <class Game>
void Population<Game>::printSummary(const int generation,
                                    const double HOF_win_percent,
                                    const double HOF_loss_percent,
                                    const double HOF_tie_percent) const {
  Player *minPlayer = m_population[0];
  Player *medPlyr = m_population[m_populationSize / 2];
  Player *maxPlayer = m_population[m_populationSize - 1];

  printf("Gen: %3d", generation);
  printf(", Min: %-6.1f [i=%-3d]", minPlayer->fitness, minPlayer->index);
  printf(", Median: %-6.1f [i=%-3d]", medPlyr->fitness, medPlyr->index);
  printf(", Max: %-6.1f [i=%-3d]", maxPlayer->fitness, maxPlayer->index);
  printf("  HOF: ");
  printf("W: %.2lf%%, ", HOF_win_percent);
  printf("L: %.2lf%%, ", HOF_loss_percent);
  printf("T: %.2lf%%", HOF_tie_percent);
  cout << endl;
}

template <class Game>
void Population<Game>::printPopulationFrom(const unsigned int start,
                                           const unsigned int lastIndex) const {
  if (start > lastIndex) {
    cerr << "Error: start is greater than lastIndex" << endl;
    exit(1);
  }
  if (lastIndex > m_population.size() - 1) {
    cerr << "Error: lastIndex is larger than population's last index" << endl;
    exit(1);
  }
  for (unsigned int i = start; i <= lastIndex; ++i) {
    cout << "Population[" << m_population[i]->index << "] fitness: ";
    printf("%5.1f\n", m_population[i]->fitness);
  }
}

template <class Game>
Player *Population<Game>::LoadPlayer(const string path) {
  char loadPlayer;
  cout << "Do you want to load a trained player? (y/n): ";
  cin >> loadPlayer;

  if (loadPlayer == 'y' || loadPlayer == 'Y') {
    return (path, std::to_string(""));
  }
  return NULL;
}

template <class Game>
bool Population<Game>::SavePlayer(const string path) {
  char savePlayer;
  cout << "Do you want to save the best player to a file? (y/n): ";
  cin >> savePlayer;

  if (savePlayer == 'y' || savePlayer == 'Y') {
    savePlayerToFile(m_population.back(), path);
    return true;
  }
  return false;
}

template <class Game>
bool Population<Game>::PlayBest() {
  char playBest;
  cout << "Do you want to play against the best player? (y/n): ";
  cin >> playBest;

  if (playBest == 'y' || playBest == 'Y') {
    playTestGame(m_population.back());
    return true;
  }
  return false;
}

#endif
