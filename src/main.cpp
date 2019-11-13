

#include <Eigen/Dense>
#include "Population.h"
#include "main.h"

#define TEST_MODE 0

int main() {
  srand((unsigned int)time(NULL));

  // Where your player log files are stored
  string path = "data/";
  Population<TicTacToe> pop;

#if TEST_MODE
  ManualPlayer tempHuman1(cin, cout, 3, 3);
  playerContainer<ManualPlayer> human1(tempHuman1);

  PerfectPlayer perfPlayer;
  playerContainer<PerfectPlayer> perf(perfPlayer);

  TicTacToe<ManualPlayer, PerfectPlayer> game1(human1, perf, true);
  game1.playGame();
  TicTacToe<PerfectPlayer, ManualPlayer> game2(perf, human1, true);
  game2.playGame();
#else

  pop.Init();
  time_t trainingTime = pop.Train(false);

  cout << "Time to train: " << trainingTime
       << (trainingTime == 1 ? "second" : " seconds") << endl;

  // pop.SavePlayer(path);
  pop.PlayBest();

#endif

  return 0;
}
