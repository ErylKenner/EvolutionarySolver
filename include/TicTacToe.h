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



template <class P1, class P2>
class TicTacToe {
public:
    TicTacToe(playerContainer<P1>& player1, playerContainer<P2>& player2,
              bool verbose = false);
    void playGame();

    static const int NUM_INPUTS = 9;
    static const int NUM_OUTPUTS = 9;
private:
    bool takeTurn(const States state, const int turn);

    bool isEmpty() const;
    bool hasTied() const;
    bool hasWon() const;

    RowVectorXd toRowVector() const;
    RowVectorXd toPlayerPerspective(const States state) const;

    States getBoardAtPosition(const int position) const;
    void setBoardAtPosition(const int position, const States state);

    inline RowVectorXi bestMoves(const RowVectorXd& input) const;
    void printBoard(RowVectorXd moves, bool printProbabilities) const;
    void populateMoves(const States state, RowVectorXd &moves, const int turn);
    double minimax(const States state, const int turn);

    double winReward(const int turn) const;
    double tieReward(const int turn) const;

    uint32_t m_board;

    playerContainer<P1>& m_player1;
    playerContainer<P2>& m_player2;

    bool m_verbose;


};




template <class P1, class P2>
TicTacToe<P1, P2>::TicTacToe(playerContainer<P1>& player1,
                             playerContainer<P2>& player2, bool verbose)
    : m_player1(player1)
    , m_player2(player2)
    , m_verbose(verbose) {
    m_board = (uint32_t)0;
}

template <class P1, class P2>
void TicTacToe<P1, P2>::playGame() {
    int turn = 0;
    while (true) {
        if (takeTurn(States::playerX, turn)) {
            break;
        }
        turn++;
        if (takeTurn(States::playerO, turn)) {
            break;
        }
        turn++;
    }
}

template <class P1, class P2>
bool TicTacToe<P1, P2>::isEmpty() const {
    return m_board == (uint32_t)0;
}

template <class P1, class P2>
bool TicTacToe<P1, P2>::hasTied() const {
    for (int i = 0; i < 9; ++i) {
        if (getBoardAtPosition(i) == States::empty) {
            return false;
        }
    }
    return true;
}

//Returns a vector of the preferred moves starting with most preferred
template <class P1, class P2>
inline RowVectorXi TicTacToe<P1, P2>::bestMoves(const RowVectorXd& input) const {
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

template <class P1, class P2>
void TicTacToe<P1, P2>::printBoard(RowVectorXd moves, bool printProbabilities) const {
    cout << "+---+---+---+" << endl;
    for (int i = 0; i < 3; ++i) {
        cout << "|";
        for (int j = 0; j < 3; ++j) {
            States cur = getBoardAtPosition(3 * i + j);
            if (cur == States::playerX) {
                cout << " X |";
            } else if (cur == States::playerO) {
                cout << " O |";
            } else if (printProbabilities) {
                cout << std::setfill(' ') << std::setw(3) << std::setprecision(0) << (int)(100 * moves(3 * i + j)) << "|";
            } else {
                cout << "   |";
            }
        }
        cout << endl << "+---+---+---+" << endl;
    }
    cout << endl;
}

template <class P1, class P2>
RowVectorXd TicTacToe<P1, P2>::toRowVector() const {
    RowVectorXd temp(9);
    for (int i = 0; i < 9; ++i) {
        temp(i) = (double)getBoardAtPosition(i);
    }
    return temp;
}

/* Copy of the board with:
    - player's own squares =  1
    - opponent's squares   = -1
    - empty squares        =  0
 */
template <class P1, class P2>
RowVectorXd TicTacToe<P1, P2>::toPlayerPerspective(const States state) const {
    RowVectorXd temp = toRowVector();
    for (int i = 0; i < 9; ++i) {
        int cur = (int)temp(i);
        if (cur == (int)States::empty) {
            temp(i) = 0.0;
        } else if (cur == (int)state) {
            temp(i) = 1.0;
        } else {
            temp(i) = -1.0;
        }
    }
    return temp;
}

template <class P1, class P2>
States TicTacToe<P1, P2>::getBoardAtPosition(const int position) const {
    uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
    uint32_t ret = m_board >> shiftAmount;
    ret &= (uint32_t)3;
    if (ret == (uint32_t)0) {
        return States::empty;
    } else if (ret == (uint32_t)1) {
        return States::playerX;
    } else if (ret == (uint32_t)2) {
        return States::playerO;
    } else {
        return States::invalid;
    }
}

template <class P1, class P2>
void TicTacToe<P1, P2>::setBoardAtPosition(const int position,
                                           const States state) {
    uint32_t shiftAmount = (uint32_t)(8 - position) << 1;

    //Clear the 2-bit-wide field
    m_board &= ~((uint32_t)3 << shiftAmount);

    //Set the new state at the 2-bit-wide field
    uint32_t val = (uint32_t)state;
    m_board |= (val << shiftAmount);
}

template <class P1, class P2>
bool TicTacToe<P1, P2>::hasWon() const {
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

template <class P1, class P2>
double TicTacToe<P1, P2>::winReward(const int turn) const {
    return 1.0 + (9.0 - turn) / 10.0;
}

template <class P1, class P2>
double TicTacToe<P1, P2>::tieReward(const int turn) const {
    return 1.0;
}

template <class P1, class P2>
void TicTacToe<P1, P2>::populateMoves(const States state, RowVectorXd &moves, const int turn) {
    RowVectorXd startBoard = toPlayerPerspective(state);

    if (state == States::playerX) {
        if (std::is_same_v<P1, ManualPlayer>) {
            double index = (m_player1.player.getMove(startBoard))(0);
            moves((int)index) = 1.0;
        } else if (std::is_same_v<P1, PerfectPlayer>) {
            uint32_t currentBoard = m_board;

            for (int i = 0; i < 9; ++i) {
                if (getBoardAtPosition(i) == States::empty) {
                    setBoardAtPosition(i, States::playerX);
                    moves(i) = minimax(States::playerO, turn);
                    m_board = currentBoard;
                }
            }

            m_board = currentBoard;
        } else {
            RowVectorXd cur;
            for (int i = 0; i < 9; ++i) {
                cur = startBoard;
                if ((int)cur(i) == (int)States::empty) {
                    cur(i) = 1.0;
                    moves(i) = m_player1.player.getMove(cur)(0);
                }
            }
        }
    } else {
        if (std::is_same_v<P2, ManualPlayer>) {
            double index = (m_player2.player.getMove(startBoard))(0);
            moves((int)index) = 1.0;
        } else if (std::is_same_v<P2, PerfectPlayer>) {
            uint32_t currentBoard = m_board;

            for (int i = 0; i < 9; ++i) {
                if (getBoardAtPosition(i) == States::empty) {
                    setBoardAtPosition(i, States::playerO);
                    moves(i) = minimax(States::playerX, turn);
                    m_board = currentBoard;
                }
            }
            moves *= -1;

            m_board = currentBoard;
        } else {
            RowVectorXd cur;
            for (int i = 0; i < 9; ++i) {
                cur = startBoard;
                if ((int)cur(i) == (int)States::empty) {
                    cur(i) = 1.0;
                    moves(i) = m_player2.player.getMove(cur)(0);
                }
            }
        }
    }



}

//helper function to handle the steps required to take a turn
template <class P1, class P2>
bool TicTacToe<P1, P2>::takeTurn(const States state, const int turn) {
    //List of desired moves in order of preference
    RowVectorXd moves = RowVectorXd::Constant(9, 0.0);

    if (m_verbose && turn == 0) {
        printBoard(moves, false);
        cout << "===========================================" << endl;
    }

    populateMoves(state, moves, turn);

    if (m_verbose) {
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
        cout << "===========================================" << endl;
    }

    //Check if the move played was a winning move
    if (turn >= 4 && hasWon()) {
        if (state == States::playerX) {
            m_player1.player.addToFitness(winReward(turn));
        } else {
            m_player2.player.addToFitness(winReward(turn));
        }

        if (m_verbose) {
            cout << "===========================================" << endl;
            cout << "Player " << (state == States::playerX ? 'X' : 'O') << " has won the game!" << endl;
            cout << "===========================================" << endl;
            cout << "===========================================" << endl;
        }
        return true;
    }

    //Check if the board is now full
    if (turn == 8 && hasTied()) {
        m_player1.player.addToFitness(tieReward(turn));
        m_player2.player.addToFitness(tieReward(turn));
        if (m_verbose) {
            cout << "===========================================" << endl;
            cout << "Tie game" << endl;
            cout << "===========================================" << endl;
            cout << "===========================================" << endl;
        }
        return true;
    }

    //If the game is not over, return false
    return false;
}

template <class P1, class P2>
double TicTacToe<P1, P2>::minimax(const States state, const int turn) {
    if (turn >= 4 && hasWon()) {
        double score = 9.0 - turn;
        if (state == States::playerX) {
            return -score;
        } else {
            return score;
        }
    } else if (turn == 8 && hasTied()) {
        return 0.0;
    }

    double bestValue;
    if (state == States::playerX) {
        bestValue = -1000.0;
        uint32_t currentBoard = m_board;
        for (int i = 0; i < 9; ++i) {
            if (getBoardAtPosition(i) == States::empty) {
                setBoardAtPosition(i, States::playerX);
                bestValue = std::max(bestValue, minimax(States::playerO, turn + 1));
                m_board = currentBoard;
            }
        }
        return bestValue;
    } else {
        bestValue = 1000.0;
        uint32_t currentBoard = m_board;
        for (int i = 0; i < 9; ++i) {
            if (getBoardAtPosition(i) == States::empty) {
                setBoardAtPosition(i, States::playerO);
                bestValue = std::min(bestValue, minimax(States::playerX, turn + 1));
                m_board = currentBoard;
            }
        }
        return bestValue;
    }


}

#endif
