#ifndef TTT
#define TTT

#include "Matrix.h"
#include "NeuralNet.h"
#include <iostream>
#include <vector>


class TicTacToe {
public:
    TicTacToe(NeuralNet *player1, NeuralNet *player2, bool displayStates=false);
    void playGame();
    
private:
    bool isEmpty() const;
    bool isFull() const;

    Matrix flattenBoard() const;
    double getBoardPosition(const int position) const;
    void setBoardPosition(const int position, const int value);

    Matrix pickMove(const Matrix input) const;
    void printBoard() const;
    bool hasWon(const double playerValue) const;

    NeuralNet *player1_;
    NeuralNet *player2_;

    Matrix board_;
    int numSquares_;
    bool displayStates_;
};

#endif
