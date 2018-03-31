#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Matrix.h"
#include "NeuralNet.h"

using std::cout;
using std::cin;
using std::endl;
using std::istream;
using std::ostream;
using std::vector;

//Base class Player
class Player {
public:
    Player();
    Player(const Player& p);
    virtual ~Player();
    
    void operator= (const Player& right);
    bool operator< (const Player& right) const;
    
    virtual vector<double> getMove(const Matrix& input) const = 0;
    
    void addToFitness(const double a);
    double getFitness() const;
    void resetFitness();
    
protected:
    
    double m_fitness;
};


//A player with a neural network brain
class NeuralPlayer : public Player{
public:
    NeuralPlayer(const vector<unsigned int>& layerSizes);
    NeuralPlayer(const NeuralPlayer& p);
    virtual ~NeuralPlayer();
    
    void operator= (const NeuralPlayer& right);
    
    virtual vector<double> getMove(const Matrix& input) const override;
    
    NeuralNet neural;
private:
    
};


//A player with a manual input brain
class ManualPlayer : public Player{
public:
    ManualPlayer(istream& is, ostream& os);
    ManualPlayer(const ManualPlayer& p);
    virtual ~ManualPlayer();
    
    void operator= (const ManualPlayer& right);
    
    virtual vector<double> getMove(const Matrix& input) const override;
    
private:
    istream& m_is;
    ostream& m_os;
};








//Object to store the player and index
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
