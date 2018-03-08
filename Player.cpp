
#include "Player.h"

bool comparePlayerContainer(const playerContainer& left, const playerContainer& right){
    return left.player->getFitness() < right.player->getFitness();
}

void swap(playerContainer& left, playerContainer& right){
    Player *temp;
    temp = left.player;
    left.player = right.player;
    right.player = temp;
    
    int tempIndex;
    tempIndex = left.index;
    left.index = right.index;
    right.index = tempIndex;
}

playerContainer::playerContainer(){}

playerContainer::playerContainer(const playerContainer& other){
    player = other.player;
    index = other.index;
}

playerContainer::playerContainer(Player *p, const int i){
    player = p;
    index = i;
}

bool playerContainer::operator< (const playerContainer& right) const{
    return player->getFitness() < right.player->getFitness();
}

void playerContainer::operator= (const playerContainer& right){
    player = right.player;
    index = right.index;
}








Player::Player(const std::vector<unsigned int>& layerSizes)
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

std::vector<double> Player::getMove() const{
    Matrix temp(1, 9);
    temp.initRand(-1, 1);
    return temp.toVector();
}

std::vector<double> Player::getMove(const Matrix& input) const{
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




