
#include "Player.h"

//---------------Player---------------
Player::Player(){
    m_fitness = 0;
}


Player::Player(const Player& p){
    m_fitness = p.m_fitness;
}

Player::~Player(){
    
}

bool Player::operator< (const Player& right) const{
    return m_fitness < right.m_fitness;
}

void Player::operator= (const Player& right){
    m_fitness = right.m_fitness;
}
/*
vector<double> Player::getMove() const{
    Matrix temp(1, 9);
    temp.initRand(-1, 1);
    return temp.toVector();
}*/

void Player::addToFitness(const double a){
    m_fitness += a;
}

double Player::getFitness() const{
    return m_fitness;
}

void Player::resetFitness(){
    m_fitness = 0.0f;
}



//----------NeuralPlayer--------------
NeuralPlayer::NeuralPlayer()
    : Player()
    , neural(){
    
}

NeuralPlayer::NeuralPlayer(const vector<unsigned int>& layerSizes)
    : Player()
    , neural(layerSizes){
    
}

NeuralPlayer::NeuralPlayer(const NeuralPlayer& p)
    : Player(p)
    , neural(p.neural){
    
}

NeuralPlayer::~NeuralPlayer(){
    
}

void NeuralPlayer::operator= (const NeuralPlayer& right){
    Player::operator=(right);
    neural = right.neural;
}

vector<double> NeuralPlayer::getMove(const Matrix& input, int squareIdentity) const{
    //Append squareIdentity to the input so it knows which player it is
    Matrix appendedInput(input);
    appendedInput.addRowsCols(1, 0, (double)squareIdentity);
    
    Matrix temp(neural.forward(appendedInput));
    return temp.toVector();
}

//----------ManualPlayer--------------
ManualPlayer::ManualPlayer(istream& is, ostream& os)
    : Player()
    , m_is(is)
    , m_os(os){
    
}

ManualPlayer::ManualPlayer(const ManualPlayer& p)
    : Player(p)
    , m_is(p.m_is)
    , m_os(p.m_os){
    
}

ManualPlayer::~ManualPlayer(){
    
}

void ManualPlayer::operator= (const ManualPlayer& right){    
    Player::operator=(right);
}

vector<double> ManualPlayer::getMove(const Matrix& input, int squareIdentity) const{
    unsigned int row, col;
    do{
        char eater;
        m_os << "You control \"" << squareIdentity << "\" squares" << endl;
        m_os << "Your move, in the range 1-3, of the form \"row, col\": ";
        m_is >> row >> eater >> col;
        row --;
        col --;
        if(row > 2){
            row = 2;
        }
        if(col > 2){
            col = 2;
        }
    } while((int)input(0, 3 * row + col) != 0);
    
    Matrix temp(3,3);
    temp.initialize(0);
    temp(row, col) = 1;
    return temp.toVector();
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

vector<double> PerfectPlayer::getMove(const Matrix& input, int squareIdentity) const{
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



