
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
Player::Player(const vector<unsigned int>& layerSizes)
    : neural(layerSizes){
    m_fitness = 0;
}


Player::Player(const Player& p)
    : neural(p.neural){
    m_fitness = p.m_fitness;
}

bool Player::operator< (const Player& right) const{
    return m_fitness < right.m_fitness;
}

void Player::operator= (const Player& right){
    m_fitness = right.m_fitness;
    neural = right.neural;
}

vector<double> Player::getMove() const{
    Matrix temp(1, 9);
    temp.initRand(-1, 1);
    return temp.toVector();
}

vector<double> Player::getMove(const Matrix& input) const{
    Matrix temp(neural.forward(input));
    return temp.toVector();
}


void Player::addToFitness(const double a){
    m_fitness += a;
}

double Player::getFitness() const{
    return m_fitness;
}

void Player::resetFitness(){
    m_fitness = 0.0f;
}




