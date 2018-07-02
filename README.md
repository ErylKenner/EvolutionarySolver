# TicTacToeMachineLearning
The goal of this project is to train a computer to play tic tac toe at a 
superhuman level. Although this is a trivial problem, I'm just using it as
an opportunity to learn more about machine learning, and to have fun.

I'm using a neural network trained by a genetic algorithm to learn the game.
I start with a random population of players, and each generation they play the
other players round robin. Fitness points are awarded as: 0 points for 
each loss, 0.5 for each tie, and 1 point for each win. I then evolve the the 
players based on normal genetic algorithm techniques (mutation, breeding, 
crossover, etc.). The goal is to maximize the player's fitness.
