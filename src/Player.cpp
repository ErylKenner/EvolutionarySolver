
#include "Player.h"

//---------------Player---------------
Player::Player() : index(Player::count++), fitness(0) {}

Player::Player(const Player &other)
    : index(other.index), fitness(other.fitness) {}

Player::~Player() {}

bool Player::operator<(const Player &right) const {
  return fitness < right.fitness;
}

void Player::operator=(const Player &right) { fitness = right.fitness; }

bool Player::ComparePlayer(const Player *left, const Player *right) {
  return left->fitness < right->fitness;
}

void Player::Swap(Player *left, Player *right) {
  unsigned int indexTemp = left->index;
  double fitnessTemp = left->fitness;

  left->index = right->index;
  left->fitness = right->fitness;

  right->index = indexTemp;
  right->fitness = fitnessTemp;
}

unsigned int Player::count = 0;

//----------NeuralPlayer--------------
NeuralPlayer::NeuralPlayer() : Player(), neural() {}

NeuralPlayer::NeuralPlayer(const vector<unsigned int> &layerSizes)
    : Player(), neural(layerSizes) {}

NeuralPlayer::NeuralPlayer(const NeuralPlayer &other)
    : Player(other), neural(other.neural) {}

NeuralPlayer::~NeuralPlayer() {}

void NeuralPlayer::operator=(const NeuralPlayer &right) {
  Player::operator=(right);
  neural = right.neural;
}

RowVectorXd NeuralPlayer::getMove(const RowVectorXd &input) const {
  return neural.forward(input);
}

//----------ManualPlayer--------------
ManualPlayer::ManualPlayer(istream &is, ostream &os, const int rows,
                           const int cols)
    : Player(), m_is(is), m_os(os), m_rows(rows), m_cols(cols) {}

ManualPlayer::ManualPlayer(const ManualPlayer &other)
    : Player(other),
      m_is(other.m_is),
      m_os(other.m_os),
      m_rows(other.m_rows),
      m_cols(other.m_cols) {}

ManualPlayer::~ManualPlayer() {}

void ManualPlayer::operator=(const ManualPlayer &right) {
  Player::operator=(right);
}

RowVectorXd ManualPlayer::getMove(const RowVectorXd &input) const {
  unsigned int row, col;
  char eater;
  do {
    m_os << "Your move, of the form \"row, col\": ";
    m_is >> row >> eater >> col;
  } while (row < 1 || row > (unsigned int)m_rows || col < 1 ||
           col > (unsigned int)m_cols);
  row--;
  col--;

  RowVectorXd temp(1);
  temp << row * m_cols + col;
  return temp;
}

//----------RandomPlayer--------------
RandomPlayer::RandomPlayer(const int _size) : Player(), size(_size) {}

RandomPlayer::RandomPlayer(const RandomPlayer &other)
    : Player(other), size(other.size) {}

RandomPlayer::~RandomPlayer() {}

void RandomPlayer::operator=(const RandomPlayer &right) {
  Player::operator=(right);
}

RowVectorXd RandomPlayer::getMove(const RowVectorXd &input) const {
  RowVectorXd ret(size);
  const int resolution = 10000;
  for (int i = 0; i < size; ++i) {
    ret(i) = (double)(rand() % (resolution + 1)) / resolution;
  }
  return ret;
}

//----------PerfectPlayer--------------

PerfectPlayer::PerfectPlayer() : Player() {}

PerfectPlayer::PerfectPlayer(const PerfectPlayer &other) : Player(other) {}

PerfectPlayer::~PerfectPlayer() {}

void PerfectPlayer::operator=(const PerfectPlayer &right) {
  Player::operator=(right);
}

RowVectorXd PerfectPlayer::getMove(const RowVectorXd &input) const {
  RowVectorXd ret(1);
  const int resolution = 10000;
  ret << (double)(rand() % (resolution + 1)) / resolution;
  return ret;
}
