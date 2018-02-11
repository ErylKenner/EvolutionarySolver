#include "TicTacToe.h"


TicTacToe::TicTacToe(NeuralNet *player1, NeuralNet *player2, bool displayStates)
	: board_(3, 3)
	, displayStates_(displayStates){

	player1_ = player1;
	player2_ = player2;
	numSquares_ = 9;
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			board_(i, j) = 0;
		}
	}
}


void TicTacToe::playGame(){
	while(true){
		Matrix moves1(pickMove(player1_->forward(flattenBoard())));
		for(int i = 0; i < numSquares_; ++i){
			if(getBoardPosition(moves1(0, i)) == 0.0){
				setBoardPosition(moves1(0, i), 1);
				break;
			}
		}
		if(displayStates_){
			printBoard();
		}
		if(hasWon(1)){
			player1_->addToFitness(1.0);
			if(displayStates_){
				std::cout << "========================" << std::endl;
			}
			return;
		}
		if(isFull()){
			player1_->addToFitness(0.5);
			player2_->addToFitness(0.5);
			if(displayStates_){
				std::cout << "========================" << std::endl;
			}
			return;
		}


		Matrix moves2(pickMove(player2_->forward(flattenBoard())));
		for(int i = 0; i < numSquares_; ++i){
			if(getBoardPosition(moves2(0, i)) == 0.0){
				setBoardPosition(moves2(0, i), -1);
				break;
			}
		}
		if(displayStates_){
			printBoard();
		}
		if(hasWon(-1)){
			player2_->addToFitness(1.0);
			if(displayStates_){
				std::cout << "========================" << std::endl;
			}
			return;
		}
		if(isFull()){
			player1_->addToFitness(0.5);
			player2_->addToFitness(0.5);
			if(displayStates_){
				std::cout << "========================" << std::endl;
			}
			return;
		}
	}
}

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

Matrix TicTacToe::pickMove(const Matrix input) const{
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

void TicTacToe::printBoard() const{
	std::cout << "Board:" << std::endl;
	board_.printData();
}

Matrix TicTacToe::flattenBoard() const{
	Matrix temp(1,numSquares_);
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			temp(0, 3 * i + j) = board_(i, j);
		}
	}
	return temp;
}


double TicTacToe::getBoardPosition(const int position) const{
	return board_(position / 3, position % 3);
}

void TicTacToe::setBoardPosition(const int position, const int value){
	board_(position / 3, position % 3) = value;
}

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