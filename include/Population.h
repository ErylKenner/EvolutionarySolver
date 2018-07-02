
#ifndef POPULATION_H
#define POPULATION_H


#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>

#include <Eigen/Dense>
using namespace Eigen;

#include "NeuralNet.h"
#include "Player.h"
#include "TicTacToe.h"
#include "Genetic.h"
//#include "UltimateTTT.h"

using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;
using std::vector;
using std::string;

template <template <class, class> class Game>
class Population {
public:
    Population<Game>();

    void init(unsigned int seed = 1, istream& is = cin, ostream& os = cout);
    time_t train(bool verbose);
    string saveBest(string path);
    void loadBest(string path, string name = "");

private:
    int m_populationSize;
    int m_iterations;
    int m_hiddenLayers;
    vector<unsigned int> m_layerSizes;

    vector< playerContainer<NeuralPlayer> > m_population;
    vector< playerContainer<NeuralPlayer> > m_hallOfFame;

    Genetic m_ga;

    string savePlayerToFile(const NeuralPlayer best, const string path) const;

    playerContainer<NeuralPlayer> loadPlayerFromFile(const string path,
                                                     string name = "");

    void roundRobin();

    void playHallOfFame(playerContainer<NeuralPlayer>& best,
                        double &winPercent, double &lossPercent, double &tiePercent);

    void printSummary(const int generation, const double HOF_win_percent,
                      const double HOF_loss_percent, const double HOF_tie_percent) const;

    void printPopulationFrom(const unsigned int start,
                             const unsigned int end) const;
};



template <template <class, class> class Game>
string Population<Game>::savePlayerToFile(const NeuralPlayer best,
                                          const string path) const {
    string fileName;

    while (true) {
        cout << "Player datafile name (saved to '" << path << "'): " << endl;
        cin >> fileName;

        if (best.neural.saveToFile(path + fileName)) {
            break;
        }
        cout << "Invalid file name or path. Please try again." << endl;
    }
    return fileName;
}

template <template <class, class> class Game>
playerContainer<NeuralPlayer> Population<Game>::loadPlayerFromFile(
    const string path, string name) {
    NeuralPlayer tempLoadedPlayer;

    if (name == "") {
        string fileName;
        while (true) {
            cout << "Player datafile name (located in '" << path << "\
            '): " << endl;
            cin >> fileName;

            if (tempLoadedPlayer.neural.loadFromFile(path + fileName)) {
                break;
            }
            cout << "Invalid file name or path. Please try again." << endl;
        }
    } else {
        tempLoadedPlayer.neural.loadFromFile(path + name);
    }

    playerContainer<NeuralPlayer> loadedPlayer(tempLoadedPlayer);
    return loadedPlayer;
}


template <template <class, class> class Game>
void Population<Game>::loadBest(string path, string name) {
    playerContainer<NeuralPlayer> loadedPlayer;

    if (name == "") {
        loadedPlayer = m_population.back();
    } else {
        loadedPlayer = loadPlayerFromFile(path, name);
    }

    loadedPlayer.player.neural.printWeights();

    //Set up a human-input player
    ManualPlayer tempHuman(cin, cout, 3, 3);
    playerContainer<ManualPlayer> human(tempHuman);

    //Play each other
    Game<ManualPlayer, NeuralPlayer> testGame1(human, loadedPlayer, true);
    testGame1.playGame();

    Game<NeuralPlayer, ManualPlayer> testGame2(loadedPlayer, human, true);
    testGame2.playGame();
}

template <template <class, class> class Game>
string Population<Game>::saveBest(string path) {
    NeuralPlayer best = m_population.back().player;
    best.neural.printWeights();
    string name = savePlayerToFile(best, path);
    return name;
}


template <template <class, class> class Game>
time_t Population<Game>::train(bool verbose) {
    time_t startTime = time(NULL);
    for (int generation = 0; generation < m_iterations; ++generation) {
        //Play games with every permutaiton of players
        roundRobin();

        //Sorts the players by fitness (ascending)
        sort(m_population.begin(), m_population.end(),
             comparePlayerContainer<NeuralPlayer>);

        m_hallOfFame.push_back(m_population[m_populationSize - 1]);

        //Print board
        if (verbose) {
            NeuralPlayer best = m_population.back().player;
            best.neural.printWeights();
        }

        //Test the current best vs. the best from each previous generation
        double HOF_win_percent;
        double HOF_loss_percent;
        double HOF_tie_percent;
        playHallOfFame(m_population.back(), HOF_win_percent, HOF_loss_percent,
                       HOF_tie_percent);

        //Print epoch summary
        printSummary(generation, HOF_win_percent, HOF_loss_percent,
                     HOF_tie_percent);

        //Make new players based on how successful the current ones are
        m_ga.breed(m_population);

        //Each weight has a small chance to change by some random value
        m_ga.mutate(m_population);

        //Reset fitness values for next generation
        for (int i = 0; i < m_populationSize; ++i) {
            m_population[i].player.resetFitness();
        }
    }
    return time(NULL) - startTime;
}


template <template <class, class> class Game>
Population<Game>::Population() : m_ga(0.1f, 0.05f) {

}

template <template <class, class> class Game>
void Population<Game>::init(unsigned int seed, istream& is, ostream& os) {
    srand(seed);

    //Get population size
    os << "Population size: ";
    is >> m_populationSize;
    if (m_populationSize < 2 || cin.fail()) {
        cin.clear();
        cin.ignore();
        m_populationSize = 2;
    }

    //Get number of iterations
    os << "Iterations: ";
    is >> m_iterations;
    if (m_iterations < 1 || cin.fail()) {
        cin.clear();
        cin.ignore();
        m_iterations = 1;
    }

    //Get number of layers
    os << "Number of hidden layers: ";
    is >> m_hiddenLayers;
    if (cin.fail()) {
        cin.clear();
        cin.ignore();
        m_hiddenLayers = 0;
    }

    //Populate m_layerSizes
    m_layerSizes.push_back(Game<NeuralPlayer, NeuralPlayer>::NUM_INPUTS);
    for (int i = 1; i <= m_hiddenLayers; ++i) {
        os << "Number in hidden layer " << i << ": ";
        unsigned int temp;
        is >> temp;
        if (temp < 1 || cin.fail()) {
            cin.clear();
            cin.ignore();
            temp = 1;
        }
        m_layerSizes.push_back(temp);
    }
    m_layerSizes.push_back(Game<NeuralPlayer, NeuralPlayer>::NUM_OUTPUTS);

    //Instantiate the Players
    for (int i = 0; i < m_populationSize; ++i) {
        NeuralPlayer temp(m_layerSizes);
        playerContainer<NeuralPlayer> tempContainer(temp);
        m_population.push_back(tempContainer);
    }

    m_ga.setPopulationSize(m_populationSize);
    m_hallOfFame.reserve(m_iterations);
}

template <template <class, class> class Game>
void Population<Game>::roundRobin() {
    for (int i = 0; i < m_populationSize - 1; ++i) {
        for (int j = i + 1; j < m_populationSize; ++j) {
            //Game 1
            /*cout << "Game between [" << population[i].second << "] \
            and [" << population[j].second << "]" << endl;*/
            Game<NeuralPlayer, NeuralPlayer> game1(m_population[i],
                                                   m_population[j], false);
            game1.playGame();

            //Game 2 (play 2 games so both players can start first)
            /*cout << "Game between [" << population[j].second << "] \
            and [" << population[i].second << "]" << endl;*/
            Game<NeuralPlayer, NeuralPlayer> game2(m_population[j],
                                                   m_population[i], false);
            game2.playGame();
        }
    }
}

template <template <class, class> class Game>
void Population<Game>::playHallOfFame(playerContainer<NeuralPlayer>& best,
                                      double &winPercent,
                                      double &lossPercent,
                                      double &tiePercent) {
    int numOpponents = m_hallOfFame.size() - 1;
    double initialFitness = best.player.getFitness();

    double numWins = 0.0;
    double numTies = 0.0;
    double numLoss = 0.0;


    for (int i = 0; i < numOpponents; ++i) {
        //Game 1
        best.player.resetFitness();
        Game<NeuralPlayer, NeuralPlayer> game1(m_hallOfFame[i], best, false);
        game1.playGame();

        //Check if best won or not
        double cur = best.player.getFitness();
        if (cur > 1.0) {
            numWins++;
        } else if (cur >= 0.5) {
            numTies++;
        } else {
            numLoss++;
        }

        //Game 2 (so both players can start first)
        best.player.resetFitness();
        Game<NeuralPlayer, NeuralPlayer> game2(best, m_hallOfFame[i], false);
        game2.playGame();

        //Check if best won or not
        cur = best.player.getFitness();
        if (cur > 1.0) {
            numWins++;
        } else if (cur >= 0.5) {
            numTies++;
        } else {
            numLoss++;
        }
    }

    winPercent = 100.0 * numWins / (2 * numOpponents);
    tiePercent = 100.0 * numTies / (2 * numOpponents);
    lossPercent = 100.0 * numLoss / (2 * numOpponents);

    //Set the best player's fitness to what it was before this function
    best.player.resetFitness();
    best.player.addToFitness(initialFitness);
}

template <template <class, class> class Game>
void Population<Game>::printSummary(const int generation,
                                    const double HOF_win_percent,
                                    const double HOF_loss_percent,
                                    const double HOF_tie_percent) const {
    playerContainer<NeuralPlayer> maxPlayer = m_population.back();
    playerContainer<NeuralPlayer> minPlayer = m_population.front();
    playerContainer<NeuralPlayer> medPlyr = m_population[m_populationSize / 2];

    printf("Gen: %3d", generation);
    printf(", Min: %-6.1f [i=%-3d]",
           minPlayer.player.getFitness(), minPlayer.index);

    printf(", Median: %-6.1f [i=%-3d]",
           medPlyr.player.getFitness(), medPlyr.index);

    printf(", Max: %-6.1f [i=%-3d]",
           maxPlayer.player.getFitness(), maxPlayer.index);

    printf("  HOF: ");
    printf("W: %.2lf%%, ", HOF_win_percent);
    printf("L: %.2lf%%, ", HOF_loss_percent);
    printf("T: %.2lf%%", HOF_tie_percent);
    cout << endl;
}

template <template <class, class> class Game>
void  Population<Game>::printPopulationFrom(const unsigned int start,
                                            const unsigned int end) const {
    if (start > end) {
        cerr << "Error: printPopulationFrom start is greater than end" << endl;
        exit(1);
    }
    if (end > m_population.size()) {
        cerr << "Error: printPopulationFrom end is larger than \
        populationSize" << endl;
        exit(1);
    }
    for (unsigned int i = start; i < end; ++i) {
        cout << "Population[" << m_population[i].index << "] fitness: ";
        printf("%5.1f\n", m_population[i].player.getFitness());
    }
}

#endif

