
#include "TicTacToe.h"

//TODO: Rewrite with bitwise representation.

TicTacToe::TicTacToe(Player *player1, Player *player2, bool verbose)
	: m_board(NUM_ROWS, NUM_COLS)
	, m_player1(player1)
	, m_player2(player2)
	, m_verbose(verbose){

	m_board.initialize(0);
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
bool TicTacToe::isFull() const{
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
std::vector<unsigned int> TicTacToe::bestMoves(const std::vector<double>& input) const{
	std::vector<unsigned int> temp;
	std::vector< std::pair<double, unsigned int> > inputPair;
	
	//Populate inputPair
	for(unsigned int i = 0; i < NUM_OUTPUTS; ++i){
		inputPair.push_back( std::make_pair(input[i], i) );
	}
	
	std::sort(inputPair.begin(), inputPair.end());
	
	//Populate temp
	for(unsigned int i = 0; i < NUM_OUTPUTS; ++i){
		temp.push_back(inputPair[i].second);
	}
	
	//Reverse temp
	std::reverse(temp.begin(), temp.end());
	return temp;
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
bool TicTacToe::hasWon(const int val) const{
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
	std::vector<unsigned int> moves;
	if(player1){
		moves = bestMoves( m_player1->getMove(flattenBoard()) );
	} else{
		moves = bestMoves( m_player2->getMove(flattenBoard()) );
	}

	//Make the best move from available squares
	for(int i = 0; i < NUM_OUTPUTS; ++i){
		if((int)getBoardAtPosition(moves[i]) == 0){
			setBoardAtPosition(moves[i], squareIdentity);
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
			m_player1->addToFitness(1.0);
		} else{
			m_player2->addToFitness(1.0);
		}
		
		if(m_verbose){
			std::cout << "========================" << std::endl;
		}
		return true;
	}

	//Check if the board is now full
	if(isFull()){
		m_player1->addToFitness(0.5);
		m_player2->addToFitness(0.5);
		if(m_verbose){
			std::cout << "========================" << std::endl;
		}
		return true;
	}

	//If the game is not over, return false
	return false;
}