

#include "main.h"

int main(){
    //Where your player log files are stored
    string path = "data/";
    Population<TicTacToe> pop;
    
    
    ManualPlayer tempHuman1(cin, cout);
    playerContainer<ManualPlayer> human1(tempHuman1);
    ManualPlayer tempHuman2(cin, cout);
    playerContainer<ManualPlayer> human2(tempHuman2);
    
    UltimateTTT<ManualPlayer, ManualPlayer> ttt(human1, human2, true);
    ttt.printBoard();
    
    
    char loadPlayer;
    cout << "Do you want to load a trained player? (y/n): ";
    cin >> loadPlayer;
    
    
    if(loadPlayer == 'y' || loadPlayer == 'Y'){
        pop.loadBest(path);
    } else{
        pop.init(time(NULL));
        pop.train(false);
        
        char savePlayer;
        cout << "Do you want to save the best player to a file? (y/n): ";
        cin >> savePlayer;
        string name = "";
        
        if(savePlayer == 'y' || savePlayer == 'Y'){
            name = pop.saveBest(path);
        }
        
        char playBest;
        cout << "Do you want to play against the best player? (y/n): ";
        cin >> playBest;
        
        if(playBest == 'y' || playBest == 'Y'){
            pop.loadBest(path, name);
        }
    }
    
    return 0;
}









    



