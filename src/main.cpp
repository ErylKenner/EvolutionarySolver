

#include "main.h"
#include "Population.h"
#include <Eigen/Dense>


int main() {
    srand((unsigned int)time(NULL));

    //Where your player log files are stored
    string path = "data/";
    Population<TicTacToe> pop;

    /*
    ManualPlayer tempHuman1(cin, cout, 10, 9);
    playerContainer<ManualPlayer> human1(tempHuman1);

    std::vector<unsigned int> layersizes;
    layersizes.push_back(81);
    layersizes.push_back(1);
    layersizes.push_back(90);
    NeuralPlayer NeuralPlayer1(layersizes);
    NeuralPlayer NeuralPlayer2(layersizes);
    playerContainer<NeuralPlayer> machine1(NeuralPlayer1);
    playerContainer<NeuralPlayer> machine2(NeuralPlayer1);

    UltimateTTT<NeuralPlayer, NeuralPlayer> ttt(machine1, machine2, true);
    ttt.playGame();
    */



    //if (!pop.load(path)) {
        pop.init();
        time_t trainingTime = pop.train(false);

        cout << "Time to train: " << trainingTime;
        if (trainingTime == 1) {
            cout << " second" << endl;
        } else {
            cout << " seconds" << endl;
        }

        //pop.save(path);
        //pop.playBest();
    //}

    return 0;
}













