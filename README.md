# TicTacToeMachineLearning
The goal of this project is to train the computer to play tic tac toe at a superhuman level. This is a rather trivial problem, but I'm simply using it as a chance to learn more about machine learning. My approach is overkill, but this is just a fun project.

I'm using a neural network trained by a genetic algorithm to learn tic tac toe. I start with a random population of players. Each generation they play every other player once. Fitness points are awarded for their record: 0 points for each loss, 0.5 for each tie, and 1 point for each win. I then evolve the the players based on normal genetic algorithm techniques (mutation, breeding, crossover, etc.). The goal is to maximize the player's fitness.
