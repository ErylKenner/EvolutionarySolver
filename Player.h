#ifndef PLAYER_H
#define PLAYER_H

#include <Eigen/Dense>
using namespace Eigen;

#include <vector>
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
    
    virtual RowVectorXd getMove(const RowVectorXd& input) const = 0;
    
    void addToFitness(const double a);
    double getFitness() const;
    void resetFitness();
    
protected:
    
    double m_fitness;
};


//A player with a neural network brain
class NeuralPlayer : public Player{
public:
    NeuralPlayer();
    NeuralPlayer(const vector<unsigned int>& layerSizes);
    NeuralPlayer(const NeuralPlayer& p);
    virtual ~NeuralPlayer();
    
    void operator= (const NeuralPlayer& right);
    
    virtual RowVectorXd getMove(const RowVectorXd& input) const override;
    
    NeuralNet neural;
private:
    
};


//A player with a manual input brain
class ManualPlayer : public Player{
public:
    ManualPlayer(istream& is, ostream& os, const int rows, const int cols);
    ManualPlayer(const ManualPlayer& p);
    virtual ~ManualPlayer();
    
    void operator= (const ManualPlayer& right);
    
    virtual RowVectorXd getMove(const RowVectorXd& input) const override;
    
private:
    istream& m_is;
    ostream& m_os;
    const int m_rows;
    const int m_cols;
};
/*
//A player with a theoretically perfect brain
class PerfectPlayer : public Player{
public:
    PerfectPlayer();
    PerfectPlayer(const PerfectPlayer& p);
    virtual ~PerfectPlayer();
    
    void operator= (const PerfectPlayer& right);
    
    virtual vector<double> getMove(const VectorXd& input) const override;
    
private:
    double winningMove() const;
    double opponentWinningMove() const;
};
*/





template <class T>
class playerContainer;

template <class T>
bool comparePlayerContainer(const playerContainer<T>& left, 
                            const playerContainer<T>& right);

template <class T>
void swap(playerContainer<T>& left, playerContainer<T>& right);

//Object to store the player and index
template <class T>
class playerContainer{
public:
    playerContainer();
    playerContainer(T input);
    playerContainer(const playerContainer<T>& other);
    
    static unsigned int count;
    unsigned int index;
    T player;
    
    void operator= (const playerContainer<T>& right);
    bool operator< (const playerContainer<T>& right) const;
    
    friend bool comparePlayerContainer<>(const playerContainer<T>& left, 
                                         const playerContainer<T>& right);
    friend void swap<>(playerContainer<T>& left, playerContainer<T>& right);
private:
    
};

//----------Friend Functions-------------
template <class T>
bool comparePlayerContainer(const playerContainer<T>& left, 
                            const playerContainer<T>& right){
    return left.player.getFitness() < right.player.getFitness();
}

template <class T>
void swap(playerContainer<T>& left, playerContainer<T>& right){
    T tempPlayer(left.player);
    left.player = right.player;
    right.player = tempPlayer;
    
    unsigned int tempIndex = left.index;
    left.index = right.index;
    right.index = tempIndex;
}






//-------------playerContainer------------
template <class T>
unsigned int playerContainer<T>::count = 0;

template <class T>
playerContainer<T>::playerContainer()
        : index(playerContainer::count++){
    //cout << "default playerContainer constructor" << endl;
}

template <class T>
playerContainer<T>::playerContainer(const playerContainer<T>& other)
        : index(other.index)
        , player(other.player){
    //cout << "copy playerContainer constructor" << endl;
}

template <class T>
playerContainer<T>::playerContainer(T input)
        : index(playerContainer::count++)
        , player(input){
    //cout << "player playerContainer constructor" << endl;
}

template <class T>
bool playerContainer<T>::operator< (const playerContainer<T>& right) const{
    return player.getFitness() < right.player.getFitness();
}

template <class T>
void playerContainer<T>::operator= (const playerContainer<T>& right){
    player = right.player;
    index = right.index;
    //cout << "playerContainer assignment operator" << endl;
}

#endif
