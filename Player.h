#ifndef PLAYER
#define PLAYER

#include <vector>
#include "Matrix.h"
#include "NeuralNet.h"



class Player {
public:
    Player(const std::vector<unsigned int>& layerSizes);
    Player(const Player& p);
    
    void operator= (const Player& right);
    bool operator< (const Player& right) const;
    
    std::vector<double> getMove() const;
    std::vector<double> getMove(const Matrix& input) const;
    
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
    playerContainer(Player *p, const int i);
    playerContainer(const playerContainer& other);
    
    Player *player;
    int index;
    
    void operator= (const playerContainer& right);
    bool operator< (const playerContainer& right) const;
    
    friend bool comparePlayerContainer(const playerContainer& left, const playerContainer& right);
    friend void swap(playerContainer& left, playerContainer& right);
private:
    
};

void swap(playerContainer& left, playerContainer& right);
bool comparePlayerContainer(const playerContainer& left, const playerContainer& right);

#endif
