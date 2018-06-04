#ifndef ULT_H
#define ULT_H

#include "Matrix.h"
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
    UltimateTTT(playerContainer<T1>& player1, playerContainer<T2>& player2, bool verbose=false);
    void playGame();
    
    void printBoard() const;
    const static int NUM_INPUTS = 9;
    const static int NUM_OUTPUTS = 81;
private:
    //bool takeTurn(const States state, const int turns);
    bool isEmpty() const;
    bool isFull() const;

    Matrix toMatrix() const;
    Matrix toPlayerPerspective(const States state) const;
    
    States getBoardAtPosition(const int subBoard, const int position) const;
    void setBoardAtPosition(const int subBoard, const int position, const States state);

    vector<unsigned int> bestMoves(const vector<double>& input) const;
    //void printBoard() const;
    //bool hasWon() const;

    vector<uint32_t> m_board;
    uint32_t m_metaBoard;
    
    playerContainer<T1>& m_player1;
    playerContainer<T2>& m_player2;

    bool m_verbose;
    
    
};




template <class T1, class T2>
UltimateTTT<T1, T2>::UltimateTTT(playerContainer<T1>& player1, playerContainer<T2>& player2, bool verbose)
    : m_board(9, (uint32_t)0)
    , m_player1(player1)
    , m_player2(player2)
    , m_verbose(verbose){
    
    m_metaBoard = (uint32_t)0;
    setBoardAtPosition(-1, 5, States::playerX);
    setBoardAtPosition(-1, 0, States::playerO);
    setBoardAtPosition(-1, 8, States::playerX);
    setBoardAtPosition(1, 7, States::playerX);
    setBoardAtPosition(6, 4, States::playerX);
}

/* Plays until a player wins or the board is full.
 * Player's fitness is updated within takeTurn().
 */
template <class T1, class T2>
void UltimateTTT<T1, T2>::playGame(){
    int turns = 0;/*
    while(true){
        turns++;/
        if(takeTurn(States::playerX, turns)){
            break;
        }
        turns++;
        if(takeTurn(States::playerO, turns)){
            break;
        }
    }*/
}

//Helper method to determine if the board is empty
template <class T1, class T2>
bool UltimateTTT<T1, T2>::isEmpty() const{
    for(int i = 0; i < 9; ++i){
        if(m_board[i] != (uint32_t)0){
            return false;
        }
    }
    return true;
}

//Helper method to determine if the board is full
template <class T1, class T2>
bool UltimateTTT<T1, T2>::isFull() const{
    for(int i = 0; i < 9; ++i){
        if(getBoardAtPosition(-1, i) == States::empty){
            return false;
        }
    }
    return true;
}

//Returns a vector of the preferred moves starting with most preferred
template <class T1, class T2>
vector<unsigned int> UltimateTTT<T1, T2>::bestMoves(const vector<double>& input) const{
    vector<unsigned int> temp;
    temp.resize(NUM_INPUTS, -1);
    
    vector< pair<double, unsigned int> > inputPair;
    
    //Populate inputPair
    for(unsigned int i = 0; i < NUM_INPUTS; ++i){
        inputPair.push_back(make_pair(input[i], i));
    }
    
    sort(inputPair.begin(), inputPair.end());
    
    //Populate temp
    for(unsigned int i = 0; i < NUM_INPUTS; ++i){
        temp[i] = inputPair[i].second;
    }
    
    //Reverse temp
    reverse(temp.begin(), temp.end());
    return temp;
}

//Prints the current board to the console
template <class T1, class T2>
void UltimateTTT<T1, T2>::printBoard() const{
    for(int metaRow = 0; metaRow < 3; ++metaRow){
        cout << "+---+---+---+   +---+---+---+   +---+---+---+" << endl;
        for(int row = 0; row < 5; ++row){
            
            for(int metaCol = 0; metaCol < 3; ++metaCol){
                States curMeta = getBoardAtPosition(-1, 3 * metaRow + metaCol);
                
                if(row == 0 || row == 2 || row == 4){
                    cout << "|";
                    for(int col = 0; col < 3; ++col){
                        
                        if(curMeta == States::empty){
                            States cur = getBoardAtPosition(3 * metaRow + metaCol, 3 * (row / 2) + col);
                            if(cur == States::playerX){
                                cout << " X |"; 
                            } else if(cur == States::playerO){
                                cout << " O |";
                            } else{
                                cout << "   |";
                            }
                        } else if(row == 2 && col == 1){
                            if(curMeta == States::playerX){
                                cout << " X  ";
                            } else if(curMeta == States::playerO){
                                cout << " O  ";
                            } else{
                                cout << " ?  ";
                            }
                        } else if(col == 2){
                            cout << "   |";
                        } else{
                            cout << "    ";
                        }
                    }
                    cout << "   ";
                } else{
                    if(curMeta == States::empty){
                        cout << "+---+---+---+   ";
                    } else{
                        cout << "+           +   ";
                    }
                }
            }
            cout << endl;
        }
        cout << "+---+---+---+   +---+---+---+   +---+---+---+" << endl;
        cout << endl;
        
    }
    toMatrix().printData();
}

/* The board is stored as a 3x3 matrix.
 * This helper method converts that into a 1D column matrix
 * representation so it can be passed into other methods.
 */
template <class T1, class T2>
Matrix UltimateTTT<T1, T2>::toMatrix() const{
    Matrix temp(1, NUM_OUTPUTS);
    
    for(int metaRow = 0; metaRow < 3; ++metaRow){
        for(int row = 0; row < 3; ++row){
            for(int metaCol = 0; metaCol < 3; ++metaCol){
                for(int col = 0; col < 3; ++col){
                    temp(0, 27 * metaRow + 9 * row + 3 * metaCol + col) = (float)getBoardAtPosition(3 * metaRow + metaCol, 3 * (row / 2) + col);
                }
            }
        }
    }
    return temp;
}


/* Makes the player's own squares a 1, opponent's squares a -1, and empty squares a 0
 */
template <class T1, class T2>
Matrix UltimateTTT<T1, T2>::toPlayerPerspective(const States state) const{
    Matrix temp(toMatrix());
    
    for(unsigned int i = 0; i < temp.numRows(); ++i){
        for(unsigned int j = 0; j < temp.numCols(); ++j){
            States cur = static_cast<States>(temp(i, j));
            if(cur == States::empty){
                temp(i, j) = 0.0;
            } else if(cur == state){
                temp(i, j) = 1.0;
            } else{
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
States UltimateTTT<T1, T2>::getBoardAtPosition(const int subBoard, const int position) const{
    if(position < 0 || position > 8){
        cerr << "Invalid position in getBoardAtPosition" << endl;
        exit(1);
    }
    if(subBoard < 0){
        uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
        uint32_t ret = m_metaBoard >> shiftAmount;
        ret &= (uint32_t)3;
        if(ret == (uint32_t)0){
            return States::empty;
        } else if(ret == (uint32_t)1){
            return States::playerX;
        } else if(ret == (uint32_t)2){
            return States::playerO;
        } else{
            return States::invalid;
        }
    } else{
        uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
        uint32_t ret = m_board[subBoard] >> shiftAmount;
        ret &= (uint32_t)3;
        if(ret == (uint32_t)0){
            return States::empty;
        } else if(ret == (uint32_t)1){
            return States::playerX;
        } else if(ret == (uint32_t)2){
            return States::playerO;
        } else{
            return States::invalid;
        }
    }
    
}

/* This method sets the board square given by 'position' to a given value. 
 */
template <class T1, class T2>
void UltimateTTT<T1, T2>::setBoardAtPosition(const int subBoard, const int position, const States state){
    uint32_t shiftAmount = (uint32_t)(8 - position) << 1;
    
    if(subBoard < 0){
        //Clear the 2-bit-wode field
        m_metaBoard &= ~((uint32_t)3 << shiftAmount);
        
        //Set the new state at the 2-bit-wide field
        uint32_t val = (uint32_t)state;
        m_metaBoard |= (val << shiftAmount);
    } else{
        //Clear the 2-bit-wode field
        m_board[position] &= ~((uint32_t)3 << shiftAmount);
        
        //Set the new state at the 2-bit-wide field
        uint32_t val = (uint32_t)state;
        m_board[position] |= (val << shiftAmount);
    }
    
}
/*
//Helper function to determine if a player has won
template <class T1, class T2>
bool UltimateTTT<T1, T2>::hasWon() const{
    //PlayerX
    if((m_board & (uint32_t)65793 ) == (uint32_t)65793){ return true;}
    if((m_board & (uint32_t)4368  ) == (uint32_t)4368 ){ return true;}
    if((m_board & (uint32_t)86016 ) == (uint32_t)86016){ return true;}
    if((m_board & (uint32_t)1344  ) == (uint32_t)1344 ){ return true;}
    if((m_board & (uint32_t)21    ) == (uint32_t)21   ){ return true;}
    if((m_board & (uint32_t)66576 ) == (uint32_t)66576){ return true;}
    if((m_board & (uint32_t)16644 ) == (uint32_t)16644){ return true;}
    if((m_board & (uint32_t)4161  ) == (uint32_t)4161 ){ return true;}
    
    //PlayerO
    if((m_board & (uint32_t)131586) == (uint32_t)131586){ return true;}
    if((m_board & (uint32_t)8736  ) == (uint32_t)8736  ){ return true;}
    if((m_board & (uint32_t)172032) == (uint32_t)172032){ return true;}
    if((m_board & (uint32_t)2688  ) == (uint32_t)2688  ){ return true;}
    if((m_board & (uint32_t)42    ) == (uint32_t)42    ){ return true;}
    if((m_board & (uint32_t)133152) == (uint32_t)133152){ return true;}
    if((m_board & (uint32_t)33288 ) == (uint32_t)33288 ){ return true;}
    if((m_board & (uint32_t)8322  ) == (uint32_t)8322  ){ return true;}
    
    return false;
}

//helper function to handle the steps required to take a turn
template <class T1, class T2>
bool UltimateTTT<T1, T2>::takeTurn(const States state, const int turn){
    //holds the list of desired moves in order of preference
    vector<unsigned int> moves;
    
    //Diagnostics
    if(m_verbose){
        printBoard();
    }
    
    //player 1 controls 'X' squares, player 2 controls 'O' squares
    if(state == States::playerX){
        moves = bestMoves( m_player1.player.getMove(toPlayerPerspective(state)));
    } else{
        moves = bestMoves( m_player2.player.getMove(toPlayerPerspective(state)));
    }

    //Make the best move from available squares
    for(int i = 0; i < NUM_OUTPUTS; ++i){
        if(getBoardAtPosition(moves[i]) == States::empty){
            setBoardAtPosition(moves[i], state);
            break;
        }
    }

    //Check if the move played was a winning move
    if(turn >= 5){
        if(hasWon()){
            if(state == States::playerX){
                m_player1.player.addToFitness(1.0 + (10.0 - turn) / 10.0);
            } else{
                m_player2.player.addToFitness(1.0 + (10.0 -  turn) / 10.0);
            }
            
            if(m_verbose){
                printBoard();
                
                char symbol;
                if(state == States::playerX){
                    symbol = 'X';
                } else{
                    symbol = 'O';
                }
                
                cout << "Player " << symbol << " has won the game!" << endl;
                cout << "=============" << endl;
            }
            return true;
        }
    }
    

    //Check if the board is now full
    if(turn == 9){
        if(isFull()){
            m_player1.player.addToFitness(1.0);
            m_player2.player.addToFitness(1.0);
            if(m_verbose){
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
*/
#endif
