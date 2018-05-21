

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
        
        if(savePlayer == 'y' || savePlayer == 'Y'){
            pop.saveBest(path);
        }
    }
    
    return 0;
}









    



