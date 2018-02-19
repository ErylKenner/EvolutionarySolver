#ifndef TTT
#define TTT

#include "Matrix.h"
#include "NeuralNet.h"
#include <iostream>
#include <vector>

#define NUM_INPUTS 9
#define NUM_OUTPUTS 9
#define NUM_ROWS 3
#define NUM_COLS 3

class TicTacToe {
public:
    TicTacToe(NeuralNet& player1, NeuralNet& player2, bool displayStates=false);
    void playGame();
    
private:
    bool takeTurn(int squareIdentity);
    bool isEmpty() const;
    bool isFull() const;

    Matrix flattenBoard() const;
    double getBoardAtPosition(const int position) const;
    void setBoardAtPosition(const int position, const int value);

    Matrix bestMoves(const Matrix& input) const;
    void printBoard() const;
    bool hasWon(const double playerValue) const;

    Matrix m_board;

    NeuralNet& m_player1;
    NeuralNet& m_player2;

    const bool m_verbose;
};

#endif
