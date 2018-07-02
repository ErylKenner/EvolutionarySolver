

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

    char loadPlayer;
    cout << "Do you want to load a trained player? (y/n): ";
    cin >> loadPlayer;


    if (loadPlayer == 'y' || loadPlayer == 'Y') {
        pop.loadBest(path);
    } else {
        pop.init((unsigned int)time(NULL));
        time_t trainingTime = pop.train(false);

        cout << "Time to train: " << trainingTime << endl;

        char savePlayer;
        cout << "Do you want to save the best player to a file? (y/n): ";
        cin >> savePlayer;
        string name = "";

        if (savePlayer == 'y' || savePlayer == 'Y') {
            name = pop.saveBest(path);
        }

        char playBest;
        cout << "Do you want to play against the best player? (y/n): ";
        cin >> playBest;

        if (playBest == 'y' || playBest == 'Y') {
            pop.loadBest(path, name);
        }
    }

    return 0;
}













