

#include "main.h"

int main(){
    //Where your player log files are stored
    string path = "data/";
    Population<TicTacToe> pop;
    
    
    char loadPlayer;
    cout << "Do you want to load a trained player? (y/n): ";
    cin >> loadPlayer;
    
    
    if(loadPlayer == 'y' || loadPlayer == 'Y'){
        pop.loadBest(path);
    } else{
        pop.init(time(NULL));
        pop.train(false);
        
        char savePlayer;
        cout << "Do you want to save the best player? (y/n): ";
        cin >> savePlayer;
        string name = "";
        
        if(savePlayer == 'y' || savePlayer == 'Y'){
            name = pop.saveBest(path);
        }
        
        char playBest;
        cout << "Do you want to play the best player? (y/n): ";
        cin >> playBest;
        
        if(playBest == 'y' || playBest == 'Y'){
            pop.loadBest(path, name);
        }
    }
    
    return 0;
}









    



