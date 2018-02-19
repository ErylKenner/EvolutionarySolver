#include "Genetic.h"

Genetic::Genetic(const double mutationRate)
	: m_mutationRate(mutationRate){

}

void Genetic::breed(std::vector<NeuralNet>& population){
	int size = population.size();
	
	std::vector<NeuralNet> newPop;

	//Keep the best 10% if last generation
	int numToKeep = (int)(0.1 * (double)size);
	if(numToKeep < 1){
		numToKeep = 1;
	}
	for(int i = size - 1; i > size - 1 - numToKeep; --i){
		newPop.push_back(population[i]);
	}
	
	for(int child = 0; child < size - numToKeep; ++child){
		NeuralNet temp = population[child];
		//New child based on two randomly chosen parents
		temp.setWeights(crossOver(pickParent(population), pickParent(population)));
		//Insert new child
		newPop.push_back(temp);
	}
	
	population = newPop;
	
	for(int i = 0; i < size; ++i){
		population[i].resetFitness();
	}
}

void Genetic::mutate(std::vector<NeuralNet>& population){
	//Intialize random object for gaussian distribution (mean=0, dev=0.1)
	std::default_random_engine rd;
	std::normal_distribution<double> distribution(2, 0.05);

	int popSize = population.size();

	for(int i  = 0; i < popSize; ++i){
		std::vector<Matrix> weights = population[i].getWeights();
		int length = weights.size();

		//For each layer
		for(int layer = 0; layer < length; ++layer){
			int rows = weights[layer].numRows();
			int cols = weights[layer].numCols();

			//Randomly mutate each element
			for(int row = 0; row < rows; ++row){
				for(int col = 0; col < cols; ++col){
					double guess = rand() / double(RAND_MAX);
					if(guess <= m_mutationRate){
						(weights[layer])(row, col) += distribution(rd) - 2;
					}
				}
			}
		}
		population[i].setWeights(weights);
	}

	
}

NeuralNet Genetic::pickParent(const std::vector<NeuralNet>& population) const{
	int size = population.size();
	int totalFitness = size * (size - 1);

	double total = 0;
	int result = rand() % totalFitness;
	for(int i = size - 1; i >= 0; --i){
		double normFitness = population[i].getFitness() / (double)totalFitness;
		normFitness = 1000 * normFitness * normFitness * normFitness;
		//std::cout << " " << normFitness << std::endl;
		//total += population[i].getFitness();
		total += normFitness;
		if(result <= total){
			return population[i];
		}
	}
	return population.back();
}

std::vector<Matrix> Genetic::crossOver(NeuralNet parent1, const NeuralNet parent2){
	std::vector<Matrix> weights1 = parent1.getWeights();
	std::vector<Matrix> weights2 = parent2.getWeights();

	int length = weights1.size();
	
	//For each layer
	for(int i = 0; i < length; ++i){
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

