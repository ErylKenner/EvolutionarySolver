#ifndef ULT_H
#define ULT_H

#include "NeuralNet.h"
#include "Player.h"
#include "TicTacToe.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;


template <class T1, class T2>
class UltimateTTT {
public:
    UltimateTTT(playerContainer<T1>& player1, playerContainer<T2>& player2,
                bool verbose = false);
    void playGame();

    //TODO: put this back into private
    void printBoard() const;

    const static int NUM_INPUTS = 9;
    const static int NUM_OUTPUTS = 90;
private:
    bool takeTurn(const States state, const int turns);
    bool isEmpty() const;
    bool isFull() const;
    bool subBoardTied(const int subBoard) const;

    Matrix toMatrix() const;
    Matrix toPlayerPerspective(const States state) const;

    States getBoardAtPos(const int subBoard, const int position) const;
    States getBoardAtPos(const int i) const;
    void setBoardAtPos(const int subBoard, const int position,
                       const States state);
    void setBoardAtPos(const int i, const States state);

    vector<unsigned int> bestMoves(const vector<double>& input) const;
    //void printBoard() const;
    bool boardHasWon(const uint32_t board) const;

    vector<uint32_t> m_board;
    uint32_t m_metaBoard;
    int activeBoard;

    playerContainer<T1>& m_player1;
    playerContainer<T2>& m_player2;

    bool dontRevisitSquares;
    bool m_verbose;


};




template <class T1, class T2>
UltimateTTT<T1, T2>::UltimateTTT(playerContainer<T1>& player1,
                                 playerContainer<T2>& player2, bool verbose)
    : m_board(9, (uint32_t)0)
    , activeBoard(4)
    , m_player1(player1)
    , m_player2(player2)
    , dontRevisitSquares(true)
    , m_verbose(verbose) {

    m_metaBoard = (uint32_t)0;
}

/* Plays until a player wins or the board is full.
 * Player's fitness is updated within takeTurn().
 */
template <class T1, class T2>
void UltimateTTT<T1, T2>::playGame() {
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
bool UltimateTTT<T1, T2>::isEmpty() const {
    for (int i = 0; i < 9; ++i) {
        if (m_board[i] != (uint32_t)0) {
            return false;
        }
    }
    return true;
}

//Helper method to determine if the board is full
template <class T1, class T2>
bool UltimateTTT<T1, T2>::isFull() const {
    for (int i = 0; i < 9; ++i) {
        if (getBoardAtPos(-1, i) == States::empty
            && !subBoardTied(i)) {
            return false;
        }
    }
    return true;
}

template <class T1, class T2>
bool UltimateTTT<T1, T2>::subBoardTied(const int subBoard) const {
    for (int i = 0; i < 9; ++i) {
        if (getBoardAtPos(subBoard, i) == States::empty) {
            return false;
        }
    }
    return true;
}

//Returns a vector of the preferred moves starting with most preferred
template <class T1, class T2>
vector<unsigned int> UltimateTTT<T1, T2>::bestMoves(
    const vector<double>& input) const {

    vector<unsigned int> ret;
    vector< pair<double, unsigned int> > inputPair;
    ret.reserve(NUM_OUTPUTS);
    inputPair.reserve(NUM_OUTPUTS);

    //Populate inputPair
    for (unsigned int i = 0; i < NUM_OUTPUTS; ++i) {
        inputPair.push_back(make_pair(input[i], i));
    }

    sort(inputPair.begin(), inputPair.end());

    //Populate temp
    for (unsigned int i = 0; i < NUM_OUTPUTS; ++i) {
        ret.push_back(inputPair[i].second);
    }

    //Reverse temp
    reverse(ret.begin(), ret.end());
    return ret;
}

//Prints the current board to the console
template <class T1, class T2>
void UltimateTTT<T1, T2>::printBoard() const {
    for (int i = 0; i < 25; ++i) {
        for (int j = 0; j < 47; ++j) {
            char cur = ' ';

            int x = (j - 3) / 4 - (j - 3) / 16;
            int y = (i - 2) / 2 - (i - 2) / 8;

            int subBoard = x / 3 + 3 * (int)(y / 3);
            int subPostion = x % 3 + 3 * (y % 3);

            bool insideMetaBox_x = (j / 4) % 4 == 1 || (j / 4) % 4 == 2;
            bool insideMetaBox_y = (i / 2) % 4 == 1 || (i / 2) % 4 == 2;
            bool metaOccupied = true; //default

            if (i != 0 && i != 24) {
                metaOccupied = getBoardAtPos(-1, subBoard) != States::empty;
            }

            if (i % 8 == 0 && (j / 16) == (activeBoard % 3) && j % 16 != 15
                && ((i / 8) == (activeBoard / 3) ||
                (i / 8) == ((activeBoard / 3) + 1))) {
                cur = '#';
            } else if ((j % 16 == 0 || j % 16 == 14)
                       && (i / 8) == (activeBoard / 3)
                       && (j / 16) == (activeBoard % 3)) {
                cur = '#';
            } else if (i % 2 == 1 && j % 4 == 1) {
                if (metaOccupied && dontRevisitSquares
                    && insideMetaBox_x && insideMetaBox_y) {
                    cur = ' ';
                } else {
                    cur = '+';
                }
            } else if (i % 2 == 1 && (j % 16 != 0) && (j % 16 != 14) &&
                (j % 16 != 15)) {
                if (metaOccupied && dontRevisitSquares && insideMetaBox_y) {
                    cur = ' ';
                } else {
                    cur = '-';
                }
            } else if (j % 4 == 1 && (i % 8 != 0)) {
                if (metaOccupied && dontRevisitSquares && insideMetaBox_x) {
                    cur = ' ';
                } else {
                    cur = '|';
                }
            } else if (i % 2 == 0 && (i % 8 != 0) && j % 4 == 3 &&
                (j % 16 != 15)) {
                States curState = States::empty;
                if (metaOccupied && dontRevisitSquares) {
                    if (subPostion == 4) {
                        curState = getBoardAtPos(-1, subBoard);
                    } else {
                        curState = States::empty;
                    }
                } else {
                    curState = getBoardAtPos(subBoard, subPostion);
                }

                if (curState == States::playerX) {
                    cur = 'X';
                } else if (curState == States::playerO) {
                    cur = 'O';
                } else {
                    cur = ' ';
                }
            }
            cout << cur;
        }
        cout << endl;
    }
}

/* The board is stored as a 3x3 matrix.
 * This helper method converts that into a 1D column matrix
 * representation so it can be passed into other methods.
 */
template <class T1, class T2>
Matrix UltimateTTT<T1, T2>::toMatrix() const {
    Matrix temp(1, NUM_OUTPUTS - 9);

    for (int i = 0; i < (NUM_OUTPUTS - 9); ++i) {
        temp(0, i) = (float)getBoardAtPos(i);
    }
    return temp;
}


/* Copy of the board with:
    - player's own squares =  1,
    - opponent's squares   = -1,
    - empty squares        =  0
 */
template <class T1, class T2>
Matrix UltimateTTT<T1, T2>::toPlayerPerspective(const States state) const {
    Matrix temp(toMatrix());

    for (unsigned int i = 0; i < temp.numRows(); ++i) {
        for (unsigned int j = 0; j < temp.numCols(); ++j) {
            States cur = static_cast<States>(temp(i, j));
            if (cur == States::empty) {
                temp(i, j) = 0.0;
            } else if (cur == state) {
                temp(i, j) = 1.0;
            } else {
                temp(i, j) = -1.0;
            }
        }
    }
    return temp;
}

/* 'position' is an integer between [0,8], but the board is a 3x3 matrix
 * This method returns the board position at that position.
 */
template <class T1, class T2>
States UltimateTTT<T1, T2>::getBoardAtPos(const int subBoard,
                                          const int position) const {
    if (position < 0 || position > 8) {
        cerr << "Invalid position in getBoardAtPos" << endl;
        exit(1);
    }

    uint32_t ret;
    if (subBoard < 0) {
        ret = m_metaBoard;
    } else {
        ret = m_board[subBoard];
    }

    uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
    ret = (uint32_t)3 & (ret >> shiftAmount);

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

template <class T1, class T2>
States UltimateTTT<T1, T2>::getBoardAtPos(const int i) const {
    if (i < 0 || i > 81) {
        cerr << "Invalid position in getBoardAtPos" << endl;
        exit(1);
    }

    int x = i % 9;
    int y = i / 9;

    int subBoard = x / 3 + 3 * (int)(y / 3);
    int position = x % 3 + 3 * (y % 3);


    uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
    uint32_t ret;
    if (subBoard < 0) {
        ret = m_metaBoard;
    } else {
        ret = m_board[subBoard];
    }

    ret = (uint32_t)3 & (ret >> shiftAmount);

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

/* This method sets the board square given by 'position' to a given value.
 */
template <class T1, class T2>
void UltimateTTT<T1, T2>::setBoardAtPos(const int subBoard, const int position,
                                        const States state) {
    if (position < 0 || position > 8) {
        cerr << "Invalid position in getBoardAtPos" << endl;
        exit(1);
    }

    uint32_t shiftAmount = (uint32_t)(8 - position) << 1;

    if (subBoard < 0) {
        //Clear the 2-bit-wide field
        m_metaBoard &= ~((uint32_t)3 << shiftAmount);

        //Set the new state at the 2-bit-wide field
        uint32_t val = (uint32_t)state;
        m_metaBoard |= (val << shiftAmount);
    } else {
        //Clear the 2-bit-wide field
        m_board[subBoard] &= ~((uint32_t)3 << shiftAmount);

        //Set the new state at the 2-bit-wide field
        uint32_t val = (uint32_t)state;
        m_board[subBoard] |= (val << shiftAmount);
    }

}

template <class T1, class T2>
void UltimateTTT<T1, T2>::setBoardAtPos(const int i, const States state) {
    if (i < 0 || i > 81) {
        cerr << "Invalid position in getBoardAtPos" << endl;
        exit(1);
    }

    int x = i % 9;
    int y = i / 9;

    int subBoard = x / 3 + 3 * (int)(y / 3);
    int position = x % 3 + 3 * (y % 3);

    uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
    //Clear the 2-bit-wode field
    m_board[subBoard] &= ~((uint32_t)3 << shiftAmount);

    //Set the new state at the 2-bit-wide field
    uint32_t val = (uint32_t)state;
    m_board[subBoard] |= (val << shiftAmount);
}


template <class T1, class T2>
bool UltimateTTT<T1, T2>::boardHasWon(const uint32_t board) const {
    //PlayerX
    if ((board & (uint32_t)65793) == (uint32_t)65793) { return true; }
    if ((board & (uint32_t)4368) == (uint32_t)4368) { return true; }
    if ((board & (uint32_t)86016) == (uint32_t)86016) { return true; }
    if ((board & (uint32_t)1344) == (uint32_t)1344) { return true; }
    if ((board & (uint32_t)21) == (uint32_t)21) { return true; }
    if ((board & (uint32_t)66576) == (uint32_t)66576) { return true; }
    if ((board & (uint32_t)16644) == (uint32_t)16644) { return true; }
    if ((board & (uint32_t)4161) == (uint32_t)4161) { return true; }

    //PlayerO
    if ((board & (uint32_t)131586) == (uint32_t)131586) { return true; }
    if ((board & (uint32_t)8736) == (uint32_t)8736) { return true; }
    if ((board & (uint32_t)172032) == (uint32_t)172032) { return true; }
    if ((board & (uint32_t)2688) == (uint32_t)2688) { return true; }
    if ((board & (uint32_t)42) == (uint32_t)42) { return true; }
    if ((board & (uint32_t)133152) == (uint32_t)133152) { return true; }
    if ((board & (uint32_t)33288) == (uint32_t)33288) { return true; }
    if ((board & (uint32_t)8322) == (uint32_t)8322) { return true; }

    return false;
}

//helper function to handle the steps required to take a turn
template <class T1, class T2>
bool UltimateTTT<T1, T2>::takeTurn(const States state, const int turn) {
    //holds the list of desired moves in order of preference
    vector<unsigned int> moves;

    //Diagnostics
    if (m_verbose) {
        printBoard();
    }

    //player 1 controls 'X' squares, player 2 controls 'O' squares
    Matrix playerPerspective = toPlayerPerspective(state);
    if (state == States::playerX) {
        moves = bestMoves(m_player1.player.getMove(playerPerspective));
    } else {
        moves = bestMoves(m_player2.player.getMove(playerPerspective));
    }

    //Make the best move from available squares
    bool foundOpenSpot = false;
    for (int i = 0; i < NUM_OUTPUTS; ++i) {
        if (moves[i] < 9) {
            if (getBoardAtPos(activeBoard, moves[i]) == States::empty
                && (getBoardAtPos(-1, activeBoard) == States::empty
                    || !dontRevisitSquares)) {
                setBoardAtPos(activeBoard, moves[i], state);
                if (getBoardAtPos(-1, activeBoard) == States::empty
                    && boardHasWon(m_board[activeBoard])) {
                    setBoardAtPos(-1, activeBoard, state);
                }
                activeBoard = moves[i];
                foundOpenSpot = true;
                break;
            }
        }
    }
    if (!foundOpenSpot) {

        for (int i = 0; i < NUM_OUTPUTS; ++i) {
            if (moves[i] < 81) {
                int subBoard = (moves[i] % 9) / 3 +
                    3 * (int)((int)(moves[i] / 9) / 3);
                if (getBoardAtPos(moves[i]) == States::empty
                    && (getBoardAtPos(-1, subBoard) == States::empty
                        || !dontRevisitSquares)) {
                    setBoardAtPos(moves[i], state);

                    if (getBoardAtPos(-1, subBoard) == States::empty
                        && boardHasWon(m_board[subBoard])) {
                        setBoardAtPos(-1, subBoard, state);
                    }
                    activeBoard = subBoard;
                    break;
                }
            }
        }

    }

    //Check if the move played was a winning move
    if (turn >= 17) {
        if (boardHasWon(m_metaBoard)) {
            if (state == States::playerX) {
                m_player1.player.addToFitness(1.0 + (10.0 - turn) / 10.0);
            } else {
                m_player2.player.addToFitness(1.0 + (10.0 - turn) / 10.0);
            }

            if (m_verbose) {
                printBoard();

                char symbol;
                if (state == States::playerX) {
                    symbol = 'X';
                } else {
                    symbol = 'O';
                }

                cout << "Player " << symbol << " has won the game!" << endl;
                cout << "=============" << endl;
            }
            return true;
        }
    }


    //Check if the board is now full
    if (turn == 81) {
        if (isFull()) {
            m_player1.player.addToFitness(1.0);
            m_player2.player.addToFitness(1.0);
            if (m_verbose) {
                printBoard();
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
