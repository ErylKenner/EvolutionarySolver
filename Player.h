#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Matrix.h"
#include "NeuralNet.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

class Player {
public:
    Player(const vector<unsigned int>& layerSizes);
    Player(const Player& p);
    
    void operator= (const Player& right);
    bool operator< (const Player& right) const;
    
    vector<double> getMove() const;
    vector<double> getMove(const Matrix& input) const;
    
    void addToFitness(const double a);
    double getFitness() const;
    void resetFitness();
    
    NeuralNet neural;
private:
    
    double m_fitness;
};



class playerContainer{
public:
    playerContainer();
    playerContainer(Player *p);
    playerContainer(const playerContainer& other);
    
    Player *player;
    unsigned int index;
    static unsigned int count;
    
    void operator= (const playerContainer& right);
    bool operator< (const playerContainer& right) const;
    
    friend bool comparePlayerContainer(const playerContainer& left, const playerContainer& right);
    friend void swap(playerContainer& left, playerContainer& right);
private:
    
};

void swap(playerContainer& left, playerContainer& right);
bool comparePlayerContainer(const playerContainer& left, const playerContainer& right);

#endif
