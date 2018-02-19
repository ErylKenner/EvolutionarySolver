#include "TicTacToe.h"


TicTacToe::TicTacToe(NeuralNet& player1, NeuralNet& player2, bool verbose)
	: m_board(NUM_ROWS, NUM_COLS)
	, m_player1(player1)
	, m_player2(player2)
	, m_verbose(verbose){

	for(int row = 0; row < NUM_ROWS; ++row){
		for(int col = 0; col < NUM_COLS; ++col){
			m_board(row, col) = 0;
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
	for(int row = 0; row < NUM_ROWS; ++row){
		for(int col = 0; col < NUM_COLS; ++col){
			if(m_board(row, col) != 0.0){
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
	for(int row = 0; row < NUM_ROWS; ++row){
		for(int col = 0; col < NUM_COLS; ++col){
			if(m_board(row, col) == 0.0){
				full = false;
				break;
			}
		}
	}
	return full;
}

//Returns a matrix of the preferred moves starting with most preffered
Matrix TicTacToe::bestMoves(const Matrix& input) const{
	//Stores the list of preferred moves
	Matrix maxIndexes(1, NUM_OUTPUTS);

	//After each iteration below, this stores the squares still available
	std::vector<int> avail(NUM_OUTPUTS, 0);
	for(int i = 0; i < NUM_OUTPUTS; ++i){
		avail[i] = i;
	}

	/* Loops over avail[] (the remaining items), finding the max item left.
	 * It adds the index of that value to maxIndexes, removes it from
	 * avail[], and repeats. Ends when avail[] is gone.
	 */
	for(int choice = 0; choice < NUM_OUTPUTS; ++choice){
		//Stores the max value
		double max = input(0, avail[0]);
		//Stores the index of max in m_board
		int index = avail[0];
		//Stores the index of max in avail[]
		int availIndex = 0;

		for(unsigned int i = 1; i < avail.size(); ++i){
			if(input(0, avail[i]) > max){
				max = input(0, avail[i]);
				index = avail[i];
				availIndex = i;
			}
		}
		//Adds the index of max in m_board to maxIndexes
		maxIndexes(0, choice) = index;
		//Removes max from avail[]
		avail.erase(avail.begin() + availIndex);
	}
	return maxIndexes;
}

//Prints the current board to the console
void TicTacToe::printBoard() const{
	std::cout << "Board:" << std::endl;
	m_board.printData();
}

/* The board is stored as a 3x3 matrix.
 * This helper method converts that into a 1D column matrix
 * representation so it can be passed into other methods.
 */
Matrix TicTacToe::flattenBoard() const{
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
double TicTacToe::getBoardAtPosition(const int position) const{
	return m_board(position / NUM_COLS, position % NUM_COLS);
}

/* This method sets the board square given by 'position' to a given value. 
 */
void TicTacToe::setBoardAtPosition(const int position, const int value){
	m_board(position / NUM_COLS, position % NUM_COLS) = value;
}

//Helper function to determine if a player has won
bool TicTacToe::hasWon(const double val) const{
	//Check rows
	for(int row = 0; row < NUM_ROWS; ++row){
		if(m_board(row, 0) == val && m_board(row, 1) == val && m_board(row, 2) == val){
			return true;
		}
	}
	//Check columns
	for(int col = 0; col < NUM_COLS; ++col){
		if(m_board(0, col) == val && m_board(1, col) == val && m_board(2, col) == val){
			return true;
		}
	}
	//Check diagonals
	if(m_board(0, 0) == val && m_board(1, 1) == val && m_board(2, 2) == val){
		return true;
	}
	if(m_board(0, 2) == val && m_board(1, 1) == val && m_board(2, 0) == val){
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
	Matrix moves(1, NUM_OUTPUTS);
	if(player1){
		moves = bestMoves(m_player1.forward(flattenBoard()));
	} else{
		moves = bestMoves(m_player2.forward(flattenBoard()));
	}

	//Make the best move from available squares
	for(int i = 0; i < NUM_OUTPUTS; ++i){
		if(getBoardAtPosition(moves(0, i)) == 0.0){
			setBoardAtPosition(moves(0, i), squareIdentity);
			break;
		}
	}

	//Diagnostics
	if(m_verbose){
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
			m_player1.addToFitness(1.0);
		} else{
			m_player2.addToFitness(1.0);
		}
		
		if(m_verbose){
			std::cout << "========================" << std::endl;
		}
		return true;
	}

	//Check if the board is now full
	if(isFull()){
		m_player1.addToFitness(0.0);
		m_player2.addToFitness(0.0);
		if(m_verbose){
			std::cout << "========================" << std::endl;
		}
		return true;
	}

	//If the game is not over, return false
	return false;
}