#ifndef TTT_H
#define TTT_H

#include "Matrix.h"
#include "NeuralNet.h"
#include "Player.h"

#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;

#define NUM_INPUTS 9
#define NUM_OUTPUTS 9
#define NUM_ROWS 3
#define NUM_COLS 3

template <class T1, class T2>
class TicTacToe {
public:
    TicTacToe(playerContainer<T1>& player1, playerContainer<T2>& player2, 
        bool verbose=false);
    
    void playGame();
    
private:
    bool takeTurn(int squareIdentity);
    bool isEmpty() const;
    bool isFull() const;

    Matrix flattenBoard() const;
    double getBoardAtPosition(const int position) const;
    void setBoardAtPosition(const int position, const int value);

    vector<unsigned int> bestMoves(const vector<double>& input) const;
    void printBoard() const;
    bool hasWon(const int playerValue) const;

    Matrix m_board;

    playerContainer<T1>& m_player1;
    playerContainer<T2>& m_player2;

    bool m_verbose;
};

//TODO: Rewrite with bitwise representation.
template <class T1, class T2>
TicTacToe<T1, T2>::TicTacToe(playerContainer<T1>& player1, playerContainer<T2>& player2, bool verbose)
    : m_board(NUM_ROWS, NUM_COLS)
    , m_player1(player1)
    , m_player2(player2)
    , m_verbose(verbose){

    m_board.initialize(0);
}

/* Plays until a player wins or the board is full.
 * Player's fitness is updated within takeTurn().
 */
template <class T1, class T2>
void TicTacToe<T1, T2>::playGame(){
    while(true){
        if(takeTurn(1)){
            break;
        }
        if(takeTurn(-1)){
            break;
        }
    }
}

//Helper method to determine if the board is empty
template <class T1, class T2>
bool TicTacToe<T1, T2>::isEmpty() const{
    bool empty = true;
    for(unsigned int row = 0; row < NUM_ROWS; ++row){
        for(unsigned int col = 0; col < NUM_COLS; ++col){
            if((int)m_board(row, col) != 0){
                empty = false;
                break;
            }
        }
    }
    return empty;
}

//Helper method to determine if the board is full
template <class T1, class T2>
bool TicTacToe<T1, T2>::isFull() const{
    bool full = true;
    for(unsigned int row = 0; row < NUM_ROWS; ++row){
        for(unsigned int col = 0; col < NUM_COLS; ++col){
            if((int)m_board(row, col) == 0){
                full = false;
                break;
            }
        }
    }
    return full;
}

//Returns a vector of the preferred moves starting with most preffered
template <class T1, class T2>
vector<unsigned int> TicTacToe<T1, T2>::bestMoves(const vector<double>& input) const{
    vector<unsigned int> temp;
    vector< pair<double, unsigned int> > inputPair;
    
    //Populate inputPair
    for(unsigned int i = 0; i < NUM_OUTPUTS; ++i){
        inputPair.push_back( make_pair(input[i], i) );
    }
    
    sort(inputPair.begin(), inputPair.end());
    
    //Populate temp
    for(unsigned int i = 0; i < NUM_OUTPUTS; ++i){
        temp.push_back(inputPair[i].second);
    }
    
    //Reverse temp
    reverse(temp.begin(), temp.end());
    return temp;
}

//Prints the current board to the console
template <class T1, class T2>
void TicTacToe<T1, T2>::printBoard() const{
    cout << "Board:" << endl;
    for (int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            printf("%2d  ", (int)m_board(i, j));
        }
        cout << endl;
    }
}

/* The board is stored as a 3x3 matrix.
 * This helper method converts that into a 1D column matrix
 * representation so it can be passed into other methods.
 */
template <class T1, class T2>
Matrix TicTacToe<T1, T2>::flattenBoard() const{
    Matrix temp(1, NUM_OUTPUTS);
    for(int row = 0; row < NUM_ROWS; ++row){
        for(int col = 0; col < NUM_COLS; ++col){
            temp(0, NUM_COLS * row + col) = m_board(row, col);
        }
    }
    return temp;
}

/* 'position' is an integer between [0,8], but the board is a 3x3 matrix
 * This method returns the board position at that position. 
 */
template <class T1, class T2>
double TicTacToe<T1, T2>::getBoardAtPosition(const int position) const{
    return m_board(position / NUM_COLS, position % NUM_COLS);
}

/* This method sets the board square given by 'position' to a given value. 
 */
template <class T1, class T2>
void TicTacToe<T1, T2>::setBoardAtPosition(const int position, const int value){
    m_board(position / NUM_COLS, position % NUM_COLS) = value;
}

//Helper function to determine if a player has won
template <class T1, class T2>
bool TicTacToe<T1, T2>::hasWon(const int val) const{
    //Check rows
    for(int row = 0; row < NUM_ROWS; ++row){
        if((int)m_board(row, 0) == val && (int)m_board(row, 1) == val &&
            (int)m_board(row, 2) == val){
            return true;
        }
    }
    //Check columns
    for(int col = 0; col < NUM_COLS; ++col){
        if((int)m_board(0, col) == val && (int)m_board(1, col) == val &&
            (int)m_board(2, col) == val){
            return true;
        }
    }
    //Check diagonals
    if((int)m_board(0, 0) == val && (int)m_board(1, 1) == val &&
        (int)m_board(2, 2) == val){
        return true;
    }
    if((int)m_board(0, 2) == val && (int)m_board(1, 1) == val &&
        (int)m_board(2, 0) == val){
        return true;
    }
    return false;
}

//helper function to handle the steps required to take a turn
template <class T1, class T2>
bool TicTacToe<T1, T2>::takeTurn(int squareIdentity){
    bool player1;

    //Set up player specifics depending on player1 or player2
    if(squareIdentity == 1){
        player1 = true;
    } else{
        player1 = false;
        squareIdentity = -1;
    }

    //moves holds the list of desired moves in order of preference
    vector<unsigned int> moves;
    
    //Diagnostics
    if(m_verbose){
        printBoard();
        cout << endl;
    }
    
    if(player1){
        moves = bestMoves( m_player1.player.getMove(flattenBoard(), squareIdentity) );
    } else{
        moves = bestMoves( m_player2.player.getMove(flattenBoard(), squareIdentity) );
    }

    //Make the best move from available squares
    for(int i = 0; i < NUM_OUTPUTS; ++i){
        if((int)getBoardAtPosition(moves[i]) == 0){
            setBoardAtPosition(moves[i], squareIdentity);
            break;
        }
    }

    //Check if the move played was a winning move
    if(hasWon(squareIdentity)){
        if(player1){
            m_player1.player.addToFitness(1.0);
        } else{
            m_player2.player.addToFitness(1.0);
        }
        
        if(m_verbose){
            int player;
            if(player1){
                player = 1;
            } else{
                player = 2;
            }
            printBoard();
            cout << "Player " << player << " has won the game!" << endl;
            cout << "========================" << endl;
        }
        return true;
    }

    //Check if the board is now full
    if(isFull()){
        m_player1.player.addToFitness(0.5);
        m_player2.player.addToFitness(0.5);
        if(m_verbose){
            printBoard();
            cout << "Tie game" << endl;
            cout << "========================" << endl;
        }
        return true;
    }

    //If the game is not over, return false
    return false;
}

#endif
