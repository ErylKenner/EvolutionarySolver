#include <iostream>
#include <vector>
using namespace std;

#include "Matrix.h"
#include "NeuralNet.h"

#define NUM_INPUTS 9
#define NUM_OUTPUTS 9


int main(){

    //Get NN size
    int hiddenLayers;
    cout << "Number of hidden layers: ";
    cin >> hiddenLayers;
    
    //init layerSizes
    Matrix layerSizes(1, hiddenLayers + 2);
    
    /*
    cout << "Number of input neurons: ";
    cin >> layerSizes(0, 0);
    cout << "Number of output neurons: ";
    cin >> layerSizes(0, hiddenLayers + 1);
    */
    
    //Populate layerSizes
    layerSizes(0, 0) = NUM_INPUTS;
    layerSizes(0, hiddenLayers + 1) = NUM_OUTPUTS;
    for(int i = 1; i <= hiddenLayers; ++i){
        cout << "Number in hidden layer " << i << ": ";
        cin >> layerSizes(0, i);
    }
    
    //Print what has been entered
    cout << "LayerSizes:" << endl;
    layerSizes.printData();
    
    //Instantiate the Player
    NeuralNet Player(layerSizes);
    
    
    
    //Player.printWeights();
    
    
   
    
    return 0;
}
