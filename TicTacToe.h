#ifndef TTT
#define TTT

#include "Matrix.h"
#include "NeuralNet.h"
#include "Player.h"

#include <iostream>
#include <vector>
#include <algorithm>

#define NUM_INPUTS 9
#define NUM_OUTPUTS 9
#define NUM_ROWS 3
#define NUM_COLS 3

class TicTacToe {
public:
    TicTacToe(Player *player1, Player *player2, bool verbose=false);
    void playGame();
    
private:
    bool takeTurn(int squareIdentity);
    bool isEmpty() const;
    bool isFull() const;

    Matrix flattenBoard() const;
    double getBoardAtPosition(const int position) const;
    void setBoardAtPosition(const int position, const int value);

    std::vector<unsigned int> bestMoves(const std::vector<double>& input) const;
    void printBoard() const;
    bool hasWon(const int playerValue) const;

    Matrix m_board;

    Player *m_player1;
    Player *m_player2;

    bool m_verbose;
};

#endif
