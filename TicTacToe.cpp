#include "TicTacToe.h"


TicTacToe::TicTacToe(NeuralNet& player1, NeuralNet& player2, bool displayStates)
	: board_(3, 3)
	, displayStates_(displayStates)
	, player1_(player1)
	, player2_(player2){

	numSquares_ = 9;
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			board_(i, j) = 0;
		}
	}
}

/* Plays until a player wins or the board is full.
 * Player's fitness is updated within takeTurn().
 */
void TicTacToe::playGame(){
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
bool TicTacToe::isEmpty() const{
	bool empty = true;
	for(int i = 0; i < numSquares_; ++i){
		for(int j =0; j < 3; ++j){
			if(board_(i, j) != 0.0){
				empty = false;
				break;
			}
		}
	}
	return empty;
}

//Helper method to determine if the board is full
bool TicTacToe::isFull() const{
	bool full = true;
	for(int i = 0; i < 3; ++i){
		for(int j =0; j < 3; ++j){
			if(board_(i, j) == 0.0){
				full = false;
				break;
			}
		}
	}
	return full;
}

//Returns a matrix of the preferred moves in descending order
Matrix TicTacToe::bestMoves(const Matrix input) const{
	Matrix maxIndexes(1, numSquares_);
	std::vector<int> avail(numSquares_, 0);
	for(int i = 0; i < numSquares_; ++i){
		avail[i] = i;
	}
	for(int choice = 0; choice < numSquares_; ++choice){
		double max = input(0, avail[0]);
		int index = avail[0];
		int availIndex = 0;
		for(int i = 1; i < avail.size(); ++i){
			if(input(0, avail[i]) > max){
				max = input(0, avail[i]);
				index = avail[i];
				availIndex = i;
			}
		}
		maxIndexes(0, choice) = index;
		avail.erase(avail.begin() + availIndex);
	}
	return maxIndexes;
}

//Prionts the current board to the console
void TicTacToe::printBoard() const{
	std::cout << "Board:" << std::endl;
	board_.printData();
}

/* The board is stored as a 3x3 matrix.
 * This helper method converts that into a 1D (matrix)
 * representation so it can be passed into other methods.
 */
Matrix TicTacToe::flattenBoard() const{
	Matrix temp(1, numSquares_);
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			temp(0, 3 * i + j) = board_(i, j);
		}
	}
	return temp;
}

/* 'position' is an integer between [0,8], but the board is a 3x3 matrix
 * This method returns the board position at that position. 
 */
double TicTacToe::getBoardAtPosition(const int position) const{
	return board_(position / 3, position % 3);
}

/* This method sets the board square given by 'position' to a given value. 
 */
void TicTacToe::setBoardAtPosition(const int position, const int value){
	board_(position / 3, position % 3) = value;
}

//Helper function to determine if a player has won
bool TicTacToe::hasWon(const double playerValue) const{
	//Rows
	for(int i = 0; i < 3; ++i){
		if(board_(i, 0) == playerValue && board_(i, 1) == playerValue
			&& board_(i, 2) == playerValue){
			return true;
		}
	}
	//Columns
	for(int j = 0; j < 3; ++j){
		if(board_(0, j) == playerValue && board_(1, j) == playerValue
			&& board_(2, j) == playerValue){
			return true;
		}
	}
	//Diagonals
	if(board_(0, 0) == playerValue && board_(1, 1) == playerValue
		&& board_(2, 2) == playerValue){
		return true;
	}
	if(board_(0, 2) == playerValue && board_(1, 1) == playerValue
		&& board_(2, 0) == playerValue){
		return true;
	}
	return false;
}

//helper function to handle the steps required to take a turn
bool TicTacToe::takeTurn(int squareIdentity){
	bool player1;

	//Set up player specifics depending on player1 or player2
	if(squareIdentity == 1){
		player1 = true;
	} else{
		player1 = false;
		squareIdentity = -1;
	}

	//moves holds the list of desired moves in order of preference
	Matrix moves(1, 9);
	if(player1){
		moves = bestMoves(player1_.forward(flattenBoard()));
	} else{
		moves = bestMoves(player2_.forward(flattenBoard()));
	}

	//Make the best move from available squares
	for(int i = 0; i < numSquares_; ++i){
		if(getBoardAtPosition(moves(0, i)) == 0.0){
			setBoardAtPosition(moves(0, i), squareIdentity);
			break;
		}
	}

	//Diagnostics
	if(displayStates_){
		if(player1){
			std::cout << "Player 1's move:" << std::endl;
		} else{
			std::cout << "Player 2's move:" << std::endl;
		}
		printBoard();
	}

	//Check if the move played was a winning move
	if(hasWon(squareIdentity)){
		if(player1){
			player1_.addToFitness(1.0);
		} else{
			player2_.addToFitness(1.0);
		}
		
		if(displayStates_){
			std::cout << "========================" << std::endl;
		}
		return true;
	}

	//Check if the board is now full
	if(isFull()){
		player1_.addToFitness(0.5);
		player2_.addToFitness(0.5);
		if(displayStates_){
			std::cout << "========================" << std::endl;
		}
		return true;
	}

	//If the game is not over, return false
	return false;
}