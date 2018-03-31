
#include "Player.h"



//----------Friend Functions-------------
bool comparePlayerContainer(const playerContainer& left, const playerContainer& right){
    return left.player->getFitness() < right.player->getFitness();
}

void swap(playerContainer& left, playerContainer& right){
    Player *tempPlayer = left.player;
    left.player = right.player;
    right.player = tempPlayer;
    
    unsigned int tempIndex = left.index;
    left.index = right.index;
    right.index = tempIndex;
}






//-------------playerContainer------------
unsigned int playerContainer::count = 0;

playerContainer::playerContainer()
    : index(playerContainer::count++){
    
    //cout << "default playerContainer constructor" << endl;
}

playerContainer::playerContainer(const playerContainer& other)
    : index(other.index){
    
    player = other.player;
    //cout << "copy playerContainer constructor" << endl;
}

playerContainer::playerContainer(Player *p)
    : index(playerContainer::count++){
    
    player = p;
    //cout << "player playerContainer constructor" << endl;
}

bool playerContainer::operator< (const playerContainer& right) const{
    return player->getFitness() < right.player->getFitness();
}

void playerContainer::operator= (const playerContainer& right){
    player = right.player;
    index = right.index;
    //cout << "playerContainer assignment operator" << endl;
}







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

vector<double> NeuralPlayer::getMove(const Matrix& input) const{
    Matrix temp(neural.forward(input));
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

vector<double> ManualPlayer::getMove(const Matrix& input) const{
    unsigned int row, col;
    do{
        char eater;
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







