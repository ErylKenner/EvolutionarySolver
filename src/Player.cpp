
#include "Player.h"

//---------------Player---------------
Player::Player() {
	m_fitness = 0;
}


Player::Player(const Player& p) {
	m_fitness = p.m_fitness;
}

Player::~Player() {

}

bool Player::operator< (const Player& right) const {
	return m_fitness < right.m_fitness;
}

void Player::operator= (const Player& right) {
	m_fitness = right.m_fitness;
}

void Player::addToFitness(const double a) {
	m_fitness += a;
}

double Player::getFitness() const {
	return m_fitness;
}

void Player::resetFitness() {
	m_fitness = 0.0f;
}



//----------NeuralPlayer--------------
NeuralPlayer::NeuralPlayer()
	: Player()
	, neural() {}

NeuralPlayer::NeuralPlayer(const vector<unsigned int>& layerSizes)
	: Player()
	, neural(layerSizes) {}

NeuralPlayer::NeuralPlayer(const NeuralPlayer& p)
	: Player(p)
	, neural(p.neural) {}

NeuralPlayer::~NeuralPlayer() {

}

void NeuralPlayer::operator= (const NeuralPlayer& right) {
	Player::operator=(right);
	neural = right.neural;
}

RowVectorXd NeuralPlayer::getMove(const RowVectorXd& input) const {
	return neural.forward(input);
}

//----------ManualPlayer--------------
ManualPlayer::ManualPlayer(istream& is, ostream& os,
	const int rows, const int cols)
	: Player()
	, m_is(is)
	, m_os(os)
	, m_rows(rows)
	, m_cols(cols) {}

ManualPlayer::ManualPlayer(const ManualPlayer& p)
	: Player(p)
	, m_is(p.m_is)
	, m_os(p.m_os)
	, m_rows(p.m_rows)
	, m_cols(p.m_cols) {}

ManualPlayer::~ManualPlayer() {

}

void ManualPlayer::operator= (const ManualPlayer& right) {
	Player::operator=(right);
}

RowVectorXd ManualPlayer::getMove(const RowVectorXd& input) const {
	unsigned int row, col;
	char eater;
	do {
		m_os << "Your move, of the form \"row, col\": ";
		m_is >> row >> eater >> col;
	} while (row < 1 || row >(unsigned int)m_rows || col < 1 || col >(unsigned int)m_cols);
	row--;
	col--;

	RowVectorXd temp(1);
	temp << row * m_cols + col;
	return temp;
}

//----------RandomPlayer--------------
RandomPlayer::RandomPlayer(const int size)
	: Player() {}

RandomPlayer::RandomPlayer(const RandomPlayer& p)
	: Player(p) {}

RandomPlayer::~RandomPlayer() {

}

void RandomPlayer::operator= (const RandomPlayer& right) {
	Player::operator=(right);
}

RowVectorXd RandomPlayer::getMove(const RowVectorXd& input) const {
	RowVectorXd ret(1);
	ret << (double)rand() / RAND_MAX;
	return ret;
}

//----------PerfectPlayer--------------
/*
PerfectPlayer::PerfectPlayer()
	: Player(){

}

PerfectPlayer::PerfectPlayer(const PerfectPlayer& p)
	: Player(p){

}

PerfectPlayer::~PerfectPlayer(){

}

void PerfectPlayer::operator= (const PerfectPlayer& right){

}

vector<double> PerfectPlayer::getMove(const Matrix& input,
									  int squareIdentity) const{
	Matrix board(3, 3);
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			board(i, j) = input(3 * i + j, 0);
		}
	}

	double nextMove = -1;

	//First check if you can win immediately
	nextMove = winningMove();
	if(nextMove >= 0){
		vector<double> ret;
		ret.push_back(nextMove);
		return ret;
	}

	nextMove = opponentWinningMove();
	if(nextMove >= 0){
		vector<double> ret;
		ret.push_back(nextMove);
		for(int i = 0; i < 8; ++i){
			ret.push_back(0);
		}
		return ret;
	}


	ret vector<double>;
}

double PerfectPlayer::winningMove() const{
	return 0;
}

double PerfectPlayer::opponentWinningMove() const{
	return 0;
}
*/



