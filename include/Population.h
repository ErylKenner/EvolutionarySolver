
#ifndef POPULATION_H
#define POPULATION_H


#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <filesystem>
#include <Windows.h>

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

	void init(istream& is = cin, ostream& os = cout);
	time_t train(bool verbose);

	//bool load(const string path);
	//bool save(const string path);
	//bool playBest();

private:
	int m_populationSize;
	int m_iterations;
	int m_hiddenLayers;
	vector<unsigned int> m_layerSizes;
	int m_gamesToSimulate;

	vector< playerContainer<NeuralPlayer> > m_population;
	playerContainer<RandomPlayer> m_opponent;
	vector< playerContainer<NeuralPlayer> > m_hallOfFame;

	Genetic m_ga;

	string m_logFile;
	string m_playerDumpFile;

	//void playTestGame(playerContainer<NeuralPlayer> loadedPlayer);
	//playerContainer<NeuralPlayer> loadPlayerFromFile(const string path,
	//                                                 string name = "");
	//string savePlayerToFile(const NeuralPlayer best, const string path) const;




	void roundRobin();
	void playGames();

	void playHallOfFame(playerContainer<NeuralPlayer>& best,
		double &winPercent, double &lossPercent, double &tiePercent);

	void printSummary(const int generation, const double HOF_win_percent,
		const double HOF_loss_percent, const double HOF_tie_percent) const;

	void logTraining(const int generation, const double HOF_win_percent,
		const double HOF_loss_percent, const double HOF_tie_percent) const;

	void printPopulationFrom(const unsigned int start,
		const unsigned int end) const;
};


template <template <class, class> class Game>
Population<Game>::Population() : m_ga(0.05f, 0.02f), m_opponent(Game<NeuralPlayer, RandomPlayer>::NUM_OUTPUTS) {

}

template <template <class, class> class Game>
void Population<Game>::init(istream& is, ostream& os) {

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

	//Get number of games to simulate
	os << "Games to simulate per player: ";
	is >> m_gamesToSimulate;
	if (m_gamesToSimulate < 1 || cin.fail()) {
		cin.clear();
		cin.ignore();
		m_gamesToSimulate = 1;
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
	m_layerSizes.push_back(1);

	//Instantiate the Players
	for (int i = 0; i < m_populationSize; ++i) {
		NeuralPlayer temp(m_layerSizes);
		playerContainer<NeuralPlayer> tempContainer(temp);
		m_population.push_back(tempContainer);
	}

	m_ga.setPopulationSize(m_populationSize);
	m_hallOfFame.reserve(m_iterations);

	//Log and dump file paths
	int highestLog = 0;
	int highestDump = 0;
	//string path = "C:/Users/erylk/Documents/Programming Projects/TicTacToeMachineLearning/Log";
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	string path = string(buffer).substr(0, pos);
	path += string("\\..\\..\\Log");

	for (const auto &entry : std::filesystem::directory_iterator(path)) {
		string cur = entry.path().string();
		cur.erase(0, path.length() + 1);

		if (cur.find("TrainingLog") != string::npos && cur.find(".log") != string::npos) {
			cur.erase(0, 11);
			cur.erase(cur.length() - 4, cur.length());
			highestLog = std::stoi(cur);
		}
		if (cur.find("PlayerDump") != string::npos && cur.find(".log") != string::npos) {
			cur.erase(0, 10);
			cur.erase(cur.length() - 4, cur.length());
			highestDump = std::stoi(cur);
		}
	}
	m_logFile = path + string("\\TrainingLog") + std::to_string(highestLog + 1) + string(".log");
	m_playerDumpFile = path + string("\\PlayerDump") + std::to_string(highestDump + 1) + string(".log");
	/*
	m_logFile.open(logFile.c_str());
	if (!m_logFile.is_open()) {
		cout << "Error opening log file at: " << logFile << endl;
	}

	m_playerDumpFile.open(dumpFile.c_str());
	if (!m_playerDumpFile.is_open()) {
		cout << "Error opening player dump file at: " << dumpFile << endl;
	}*/

	cout << endl << endl;
}

template <template <class, class> class Game>
time_t Population<Game>::train(bool verbose) {
	time_t startTime = time(NULL);
	int trainingStage = 1;
	cout << "STAGE 1: RANDOM PLAYERS" << endl;
	for (int generation = 0; generation < m_iterations; ++generation) {
		switch (trainingStage) {
		case 1:
			playGames();
			break;
		case 2:
			roundRobin();
			playGames();
			break;
		case 3:
			roundRobin();
		default:
			break;
		}


		//Sorts the players by fitness (ascending)
		sort(m_population.begin(), m_population.end(),
			comparePlayerContainer<NeuralPlayer>);

		m_hallOfFame.emplace_back(m_population[m_populationSize - 1]);

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

		if (generation > 1) {
			//Print epoch summary
			printSummary(generation, HOF_win_percent, HOF_loss_percent,
				HOF_tie_percent);

			//Log training
			logTraining(generation, HOF_win_percent, HOF_loss_percent,
				HOF_tie_percent);
		}

		//Stage selection
		double highestFit = m_population.back().player.getFitness();
		double medFit = m_population[m_populationSize / 2].player.getFitness();
		double roundRobinFit = 2 * (m_populationSize - 1);
		switch (trainingStage) {
		case 1:
			if ((highestFit >= 1.2 * m_gamesToSimulate
				&& medFit >= 0.95 * m_gamesToSimulate)
				|| generation == m_iterations - 1) {
				trainingStage = 2;
				generation = 0;
				m_ga.setMutationRate((float)0.03);
				m_ga.setGreedyPercent((float)0.05);
				cout << "MOVING TO STAGE 2: ROUND ROBIN & RANDOM PLAYERS"
					<< endl;
			}
			break;
		case 2:
			if ((highestFit >= 1.2 * m_gamesToSimulate + 1.1 * roundRobinFit
				&& medFit >= 0.95 * m_gamesToSimulate + 0.8 * roundRobinFit)
				|| generation == m_iterations - 1) {
				trainingStage = 3;
				generation = 0;
				m_ga.setMutationRate((float)0.01);
				m_ga.setGreedyPercent((float)0.08);
				cout << "MOVING TO STAGE 3: ROUND ROBIN" << endl;
			}
			break;
		default:
			break;
		}


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

/*
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
*/

/*
template <template <class, class> class Game>
playerContainer<NeuralPlayer> Population<Game>::loadPlayerFromFile(
	const string path, string name) {
	NeuralPlayer tempLoadedPlayer;

	if (name == "") {
		string fileName;
		while (true) {
			cout << "Player datafile name (located in '" << path << "'): " << endl;
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
*/

/*
template <template <class, class> class Game>
void Population<Game>::playTestGame(playerContainer<NeuralPlayer> loadedPlayer) {
	ManualPlayer tempHuman(cin, cout, 3, 3);
	playerContainer<ManualPlayer> human(tempHuman);

	//Play each other
	Game<ManualPlayer, NeuralPlayer> testGame1(human, loadedPlayer, true);
	testGame1.playGame();

	Game<NeuralPlayer, ManualPlayer> testGame2(loadedPlayer, human, true);
	testGame2.playGame();
}
*/

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
void Population<Game>::playGames() {
	for (int i = 0; i < m_populationSize; ++i) {
		for (int j = 0; j < 0.5 * m_gamesToSimulate; ++j) {
			//Game 1
			Game<NeuralPlayer, RandomPlayer> game1(m_population[i],
				m_opponent, false);
			game1.playGame();

			//Game 2 (play 2 games so both players can start first)
			Game<RandomPlayer, NeuralPlayer> game2(m_opponent,
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
		}
		else if (cur >= 0.5) {
			numTies++;
		}
		else {
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
		}
		else if (cur >= 0.5) {
			numTies++;
		}
		else {
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

	cout << std::setiosflags(std::ios::fixed) << std::setprecision(2);
	cout << "Gen: " << generation << "   ";
	cout << "Min: " << minPlayer.player.getFitness() << ", "
		<< "Median: " << medPlyr.player.getFitness() << ", "
		<< "Max: " << maxPlayer.player.getFitness() << "   ";
	cout << std::setprecision(2);
	cout << "HOF:  W: " << HOF_win_percent <<
		", L: " << HOF_loss_percent <<
		", T: " << HOF_tie_percent;
	cout << endl;
}

template <template <class, class> class Game>
void Population<Game>::logTraining(const int generation,
	const double HOF_win_percent,
	const double HOF_loss_percent,
	const double HOF_tie_percent) const {
	playerContainer<NeuralPlayer> maxPlayer = m_population.back();
	playerContainer<NeuralPlayer> minPlayer = m_population.front();
	playerContainer<NeuralPlayer> medPlyr = m_population[m_populationSize / 2];

	ofstream logFile;
	logFile.open(m_logFile.c_str(), std::ios_base::app);
	logFile << generation << " ";
	logFile << minPlayer.player.getFitness() << " " << minPlayer.index << " ";
	logFile << medPlyr.player.getFitness() << " " << medPlyr.index << " ";
	logFile << maxPlayer.player.getFitness() << " " << maxPlayer.index << " ";
	logFile << HOF_win_percent << " " << HOF_loss_percent << " " << HOF_tie_percent << " ";
	logFile << "\n";
	logFile.close();
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

/*
template <template <class, class> class Game>
bool Population<Game>::load(const string path) {
	char loadPlayer;
	cout << "Do you want to load a trained player? (y/n): ";
	cin >> loadPlayer;

	if (loadPlayer == 'y' || loadPlayer == 'Y') {
		playTestGame(loadPlayerFromFile(path));
		return true;
	}
	return false;
}
*/

/*
template <template <class, class> class Game>
bool Population<Game>::save(const string path) {
	char savePlayer;
	cout << "Do you want to save the best player to a file? (y/n): ";
	cin >> savePlayer;

	if (savePlayer == 'y' || savePlayer == 'Y') {
		savePlayerToFile(m_population.back().player, path);
		return true;
	}
	return false;
}
*/

/*
template <template <class, class> class Game>
bool Population<Game>::playBest() {
	char playBest;
	cout << "Do you want to play against the best player? (y/n): ";
	cin >> playBest;

	if (playBest == 'y' || playBest == 'Y') {
		playTestGame(m_population.back());
		return true;
	}
	return false;
}
*/


#endif

