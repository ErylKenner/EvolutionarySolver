#ifndef TTT_H
#define TTT_H

#include <Eigen/Dense>
using namespace Eigen;

#include "NeuralNet.h"
#include "Player.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <type_traits>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;

enum States { empty = 0, playerX = 1, playerO = 2, invalid = 3 };



template <class T1, class T2>
class TicTacToe {
public:
	TicTacToe(playerContainer<T1>& player1, playerContainer<T2>& player2,
		bool verbose = false);
	void playGame();

	static const int NUM_INPUTS = 9;
	static const int NUM_OUTPUTS = 9;
private:
	bool takeTurn(const States state, const int turns);
	bool isEmpty() const;
	bool isFull() const;

	RowVectorXd toRowVector() const;
	RowVectorXd toPlayerPerspective(const States state) const;

	States getBoardAtPosition(const int position) const;
	void setBoardAtPosition(const int position, const States state);

	inline RowVectorXi bestMoves(const RowVectorXd& input) const;
	void printBoard(RowVectorXd moves, bool printProbabilities) const;
	bool hasWon() const;
	void populateMoves(const States state, RowVectorXd &moves);

	uint32_t m_board;

	playerContainer<T1>& m_player1;
	playerContainer<T2>& m_player2;

	bool m_verbose;


};




template <class T1, class T2>
TicTacToe<T1, T2>::TicTacToe(playerContainer<T1>& player1,
	playerContainer<T2>& player2, bool verbose)
	: m_player1(player1)
	, m_player2(player2)
	, m_verbose(verbose) {
	m_board = (uint32_t)0;
}

/* Plays until a player wins or the board is full.
 * Player's fitness is updated within takeTurn().
 */
template <class T1, class T2>
void TicTacToe<T1, T2>::playGame() {
	int turns = 0;
	while (true) {
		turns++;
		if (takeTurn(States::playerX, turns)) {
			break;
		}
		turns++;
		if (takeTurn(States::playerO, turns)) {
			break;
		}
	}
}

//Helper method to determine if the board is empty
template <class T1, class T2>
bool TicTacToe<T1, T2>::isEmpty() const {
	return m_board == (uint32_t)0;
}

//Helper method to determine if the board is full
template <class T1, class T2>
bool TicTacToe<T1, T2>::isFull() const {
	for (int i = 0; i < 9; ++i) {
		if (getBoardAtPosition(i) == States::empty) {
			return false;
		}
	}
	return true;
}

//Returns a vector of the preferred moves starting with most preferred
template <class T1, class T2>
inline RowVectorXi TicTacToe<T1, T2>::bestMoves(const RowVectorXd& input) const {
	Matrix<int, 1, 9> ret;
	vector< pair<double, unsigned int> > inputPair;
	inputPair.reserve(9);

	//Populate inputPair
	for (unsigned int i = 0; i < 9; ++i) {
		inputPair.emplace_back(make_pair(input(i), i));
	}

	sort(inputPair.begin(), inputPair.end());

	//Populate ret
	for (unsigned int i = 0; i < 9; ++i) {
		ret(8 - i) = inputPair[i].second;
	}

	return ret;
}

//Prints the current board to the console
template <class T1, class T2>
void TicTacToe<T1, T2>::printBoard(RowVectorXd moves, bool printProbabilities) const {
	cout << "+---+---+---+" << endl;
	for (int i = 0; i < 3; ++i) {
		cout << "|";
		for (int j = 0; j < 3; ++j) {
			States cur = getBoardAtPosition(3 * i + j);
			if (cur == States::playerX) {
				cout << " X |";
			}
			else if (cur == States::playerO) {
				cout << " O |";
			}
			else if (printProbabilities) {
				cout << std::setfill(' ') << std::setw(3) << std::setprecision(0) << (int)(100 * moves(3 * i + j)) << "|";
			}
			else {
				cout << "   |";
			}
		}
		cout << endl << "+---+---+---+" << endl;
	}
	cout << endl;
}

template <class T1, class T2>
RowVectorXd TicTacToe<T1, T2>::toRowVector() const {
	RowVectorXd temp(NUM_OUTPUTS);

	for (int i = 0; i < 9; ++i) {
		temp(i) = (double)getBoardAtPosition(i);
	}
	return temp;
}

/* Copy of the board with:
	- player's own squares =  1,
	- opponent's squares   = -1,
	- empty squares        =  0
 */
template <class T1, class T2>
RowVectorXd TicTacToe<T1, T2>::toPlayerPerspective(const States state) const {
	RowVectorXd temp = toRowVector();

	for (unsigned int i = 0; i < 9; ++i) {
		States cur = static_cast<States>((int)temp(i));
		if (cur == States::empty) {
			temp(i) = 0.0;
		}
		else if (cur == state) {
			temp(i) = 1.0;
		}
		else {
			temp(i) = -1.0;
		}
	}
	return temp;
}

/* 'position' is an integer between [0,8], but the board is a 3x3 matrix
 * This method returns the board position at that position.
 */
template <class T1, class T2>
States TicTacToe<T1, T2>::getBoardAtPosition(const int position) const {
	if (position < 0 || position > 8) {
		cerr << "Invalid position in getBoardAtPosition" << endl;
		exit(1);
	}
	uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
	uint32_t ret = m_board >> shiftAmount;
	ret &= (uint32_t)3;
	if (ret == (uint32_t)0) {
		return States::empty;
	}
	else if (ret == (uint32_t)1) {
		return States::playerX;
	}
	else if (ret == (uint32_t)2) {
		return States::playerO;
	}
	else {
		return States::invalid;
	}
}

/* This method sets the board square given by 'position' to a given value.
 */
template <class T1, class T2>
void TicTacToe<T1, T2>::setBoardAtPosition(const int position,
	const States state) {
	uint32_t shiftAmount = (uint32_t)(8 - position) << 1;

	//Clear the 2-bit-wode field
	m_board &= ~((uint32_t)3 << shiftAmount);

	//Set the new state at the 2-bit-wide field
	uint32_t val = (uint32_t)state;
	m_board |= (val << shiftAmount);
}

//Helper function to determine if a player has won
template <class T1, class T2>
bool TicTacToe<T1, T2>::hasWon() const {
	//PlayerX
	if ((m_board & (uint32_t)65793) == (uint32_t)65793) { return true; }
	if ((m_board & (uint32_t)4368) == (uint32_t)4368) { return true; }
	if ((m_board & (uint32_t)86016) == (uint32_t)86016) { return true; }
	if ((m_board & (uint32_t)1344) == (uint32_t)1344) { return true; }
	if ((m_board & (uint32_t)21) == (uint32_t)21) { return true; }
	if ((m_board & (uint32_t)66576) == (uint32_t)66576) { return true; }
	if ((m_board & (uint32_t)16644) == (uint32_t)16644) { return true; }
	if ((m_board & (uint32_t)4161) == (uint32_t)4161) { return true; }

	//PlayerO
	if ((m_board & (uint32_t)131586) == (uint32_t)131586) { return true; }
	if ((m_board & (uint32_t)8736) == (uint32_t)8736) { return true; }
	if ((m_board & (uint32_t)172032) == (uint32_t)172032) { return true; }
	if ((m_board & (uint32_t)2688) == (uint32_t)2688) { return true; }
	if ((m_board & (uint32_t)42) == (uint32_t)42) { return true; }
	if ((m_board & (uint32_t)133152) == (uint32_t)133152) { return true; }
	if ((m_board & (uint32_t)33288) == (uint32_t)33288) { return true; }
	if ((m_board & (uint32_t)8322) == (uint32_t)8322) { return true; }

	return false;
}

template <class T1, class T2>
void TicTacToe<T1, T2>::populateMoves(const States state, RowVectorXd &moves) {
	RowVectorXd startBoard = toPlayerPerspective(state);

	if (state == States::playerX && std::is_same_v<T1, ManualPlayer>) {
		double index = (m_player1.player.getMove(startBoard))(0);
		moves((int)index) = 1.0;
	}
	else if (state == States::playerO && std::is_same_v<T2, ManualPlayer>) {
		double index = (m_player2.player.getMove(startBoard))(0);
		moves((int)index) = 1.0;
	}
	else {
		RowVectorXd cur;
		for (int i = 0; i < 9; ++i) {
			cur = startBoard;
			if ((int)cur(i) != 0) {
				continue;
			}
			cur(i) = 1.0;
			RowVectorXd temp;
			if (state == States::playerX) {
				//moves = m_player1.player.getMove(toPlayerPerspective(state));
				temp = m_player1.player.getMove(cur);
			}
			else {
				//moves = m_player2.player.getMove(toPlayerPerspective(state));
				temp = m_player2.player.getMove(cur);
			}
			moves(i) = temp(0);
		}
	}
}

//helper function to handle the steps required to take a turn
template <class T1, class T2>
bool TicTacToe<T1, T2>::takeTurn(const States state, const int turn) {
	//holds the list of desired moves in order of preference
	RowVectorXd moves = RowVectorXd::Constant(9, 0.0);


	if (m_verbose && turn == 1) {
		printBoard(moves, false);
	}

	populateMoves(state, moves);

	if (m_verbose) {
		//Array of the values of only empty spaces
		int index = 0;
		RowVectorXd startBoard = toPlayerPerspective(state);
		RowVectorXd availableMoves(9 - (turn - 1));
		for (int i = 0; i < 9; ++i) {
			if ((int)(startBoard(i)) == 0) {
				availableMoves(index++) = moves(i);
			}
		}

		//Softmax for visualizing probabilities
		double max = availableMoves.maxCoeff();
		double sum = (availableMoves.array() - max).exp().sum();
		availableMoves = (availableMoves.array() - max - log(sum)).exp().matrix();

		//Bring array of empty space values back into moves array
		index = 0;
		for (int i = 0; i < 9; ++i) {
			if ((int)(startBoard(i)) == 0) {
				moves(i) = availableMoves(index++);
			}
			else {
				moves(i) = 0.0;
			}
		}
		printBoard(moves, true);
	}

	//Make the best move from available squares
	RowVectorXi orderedMoves = bestMoves(moves);
	for (int i = 0; i < 9; ++i) {
		if (getBoardAtPosition(orderedMoves(i)) == States::empty) {
			setBoardAtPosition(orderedMoves(i), state);
			break;
		}
	}

	if (m_verbose) {
		printBoard(moves, false);
	}

	//Check if the move played was a winning move
	if (turn >= 5) {
		if (hasWon()) {
			if (state == States::playerX) {
				m_player1.player.addToFitness(1.0 + (10.0 - turn) / 10.0);
			}
			else {
				m_player2.player.addToFitness(1.0 + (10.0 - turn) / 10.0);
			}

			if (m_verbose) {
				char symbol;
				if (state == States::playerX) {
					symbol = 'X';
				}
				else {
					symbol = 'O';
				}

				cout << "Player " << symbol << " has won the game!" << endl;
				cout << "=============" << endl;
			}
			return true;
		}
	}


	//Check if the board is now full
	if (turn == 9) {
		if (isFull()) {
			m_player1.player.addToFitness(1.0);
			m_player2.player.addToFitness(1.0);
			if (m_verbose) {
				cout << "Tie game" << endl;
				cout << "=============" << endl;
			}
			return true;
		}
	}

	//If the game is not over, return false
	return false;
}

#endif
