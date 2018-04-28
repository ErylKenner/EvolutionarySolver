
#include "Genetic.h"

float Genetic::bound(float value, float min, float max){
	if(value > max){
		value = max;
	}
	if(value < min){
		value = min;
	}
	return value;
}

//mutationRate is in the range [0, 1], greedyPercent is in the range [0, 1]
Genetic::Genetic(const float mutationRate, const float greedyPercent)
	: m_mutationRate(Genetic::bound(mutationRate, 0.0f, 1.0f))
	, m_greedyPercent(Genetic::bound(greedyPercent, 0.0f, 1.0f)){
		
}

void Genetic::setPopulationSize(int populationSize){
	m_populationSize = populationSize;
}

//Make new players based on how successful the current ones are
void Genetic::breed(vector<playerContainer<NeuralPlayer> >& population){
	vector<playerContainer<NeuralPlayer> > newPop;

	//Keep the best greedyPercent of last generation
	int numToKeep = (int)(m_greedyPercent * (float)m_populationSize);
	if(numToKeep < 1){
		numToKeep = 1;
	}
	
	//Copy the players which are being kept from greedyPercent
	for(int i = 0; i < numToKeep; ++i){
		playerContainer<NeuralPlayer> temp(population[m_populationSize - 1 - i]);
		newPop.push_back(temp);
	}
	
	//Iterates over the remaining number of necessary child elements
	for(int i = 0; i < m_populationSize - numToKeep; ++i){
		//New weights to be set, based on random parents
		vector<Matrix> newWeights = crossOver(pickParent(population), pickParent(population));
		
		playerContainer<NeuralPlayer> temp(population[i]);
		temp.player.neural.setWeights(newWeights);
		
		//Insert new child
		newPop.push_back(temp);
	}
	
	//Deep copy of newPop to population
	for(int i = 0; i < m_populationSize; ++i){
		population[i].index = newPop[i].index;
		
		//Copy newPop[i]'s weights to newWeights
		vector<Matrix> newWeights = newPop[i].player.neural.getWeights();
		
		//Copy newWeights to population[i]
	    population[i].player.neural.setWeights(newWeights);
	}
	
	for(int i = 0; i < m_populationSize; ++i){
		population[i].player.resetFitness();
	}
}

void Genetic::mutate(vector<playerContainer<NeuralPlayer> >& population){
	//Intialize random object for gaussian distribution (mean=0, dev=0.1)
	default_random_engine rd;
	normal_distribution<double> distribution(2, 0.05);

	for(int i  = 0; i < m_populationSize; ++i){
	    vector<Matrix> weights = population[i].player.neural.getWeights();
		size_t layers = weights.size();

		//For each layer
		for(size_t lay = 0; lay < layers; ++lay){
			int rows = weights[lay].numRows();
			int cols = weights[lay].numCols();

			//Randomly mutate each element
			for(int row = 0; row < rows; ++row){
				for(int col = 0; col < cols; ++col){
					//Mutate with a certain chance
					float guess = (float)rand() / float(RAND_MAX);
					if(guess < m_mutationRate){
						(weights[lay])(row, col) += distribution(rd) - 2;
					}
				}
			}
		}
	    population[i].player.neural.setWeights(weights);
	}
}

playerContainer<NeuralPlayer> Genetic::pickParent(
	const vector<playerContainer<NeuralPlayer> >& population) const{
	
	//The sum of all player's fitness within the population
	double totalPopulationFitness = 0;
	for(int i = 0; i < m_populationSize; ++i){
		double cur = population[i].player.getFitness();
		totalPopulationFitness += cur * cur;
	}
	
	//A random number in the range [0, totalPopulationFitness - 1]
	int threshold = rand() % (int)totalPopulationFitness;
	
	double sum = 0;
	for(int i = m_populationSize - 1; i >= 0; --i){
		double curFitness = population[i].player.getFitness();
		
		//Keep adding the current player's fitness until it reaches the threshold
		sum += curFitness * curFitness;
		if(sum >= threshold){
			return population[i];
		}
	}
	//Default, return the highest fitness player
	return population.back();
}

vector<Matrix> Genetic::crossOver(const playerContainer<NeuralPlayer> parent1, 
	const playerContainer<NeuralPlayer> parent2){
	
	vector<Matrix> weights1;
	vector<Matrix> weights2;
	
	//Parent 1
    weights1 = parent1.player.neural.getWeights();
    
    //Parent 2
    weights2 = parent2.player.neural.getWeights();

	size_t length = weights1.size();
	
	//For each layer
	for(size_t i = 0; i < length; ++i){
		int rows = weights1[i].numRows();
		int cols = weights1[i].numCols();
		
		//Cross breed matrix
		for(int row = 0; row < rows; ++row){
			for(int col = 0; col < cols; ++col){
				//50% chance of being from parent1 or parent2
				if(rand() % 2 == 0){
					(weights1[i])(row, col) = (weights2[i])(row, col);
				}
			}
		}
		
	}
	return weights1;
}

