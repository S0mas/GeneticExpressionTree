#pragma once
#include "Builder.h"
#include "utils.h"
#include "Hyperparameters.h"
#include <vector>

template<typename ObjectType>
void evaluation(std::vector<std::pair<std::unique_ptr<ObjectType>, double>>& population) {
	for (auto& pair : population)
		pair.second = evaluate(pair.first);
}

template<typename ObjectType>
void selection(std::vector<std::pair<std::unique_ptr<ObjectType>, double>>& population) {
	std::sort(population.begin(), population.end(), [](const auto& lhs, const auto& rhs) { return lhs.second <  rhs.second; });
	population.erase(population.begin() + population.size()/2, population.end());
}

template<typename ObjectType>
auto process(const Hyperparameters hyperparameters = Hyperparameters()) {
	std::vector<std::pair<std::unique_ptr<ObjectType>, double>> population;
	population.reserve(hyperparameters.populationSize);
	Builder builder;
	for(auto i = 0u; i < hyperparameters.populationSize; ++i)
		population.push_back({ builder.build<ObjectType>(), std::numeric_limits<double>::max() });

	for (auto epoch = 0u; epoch < hyperparameters.epochs; ++epoch) {
		evaluation(population);
		selection(population);
		std::vector<ObjectType*> selectedPopulation;
		selectedPopulation.reserve(population.size()/2);
		std::transform(population.begin(), population.end(), std::back_inserter(selectedPopulation), [](auto& pair) { return pair.first.get(); });

		while (!selectedPopulation.empty()) {
			auto r = static_cast<unsigned>(getRandomNumber(100));
			if (r < hyperparameters.crossoverChance && selectedPopulation.size() > 1) {
				auto parentId1 = getRandomNumber(selectedPopulation.size());
				auto parentId2 = getRandomNumber(selectedPopulation.size());
				while(parentId2 == parentId1)
					parentId2 = getRandomNumber(selectedPopulation.size());
				auto childs = crossover(selectedPopulation[parentId1], selectedPopulation[parentId2]);
				population.push_back({ std::move(childs.first), std::numeric_limits<double>::max() });
				population.push_back({ std::move(childs.second), std::numeric_limits<double>::max() });
				if (parentId1 > parentId2) {
					selectedPopulation.erase(selectedPopulation.begin() + parentId1);
					selectedPopulation.erase(selectedPopulation.begin() + parentId2);
				}
				else {
					selectedPopulation.erase(selectedPopulation.begin() + parentId2);
					selectedPopulation.erase(selectedPopulation.begin() + parentId1);
				}
			}
			else if(r < (hyperparameters.mutationChance + hyperparameters.crossoverChance)) {
				auto treeId = getRandomNumber(selectedPopulation.size());
				population.push_back({ mutation(selectedPopulation[treeId]), std::numeric_limits<double>::max() });
				selectedPopulation.erase(selectedPopulation.begin() + treeId);
			}
			else if (r < (hyperparameters.mutationChance + hyperparameters.crossoverChance + hyperparameters.randomAdditionChance)) {
				selectedPopulation.erase(selectedPopulation.begin());
				population.push_back({ builder.build<ObjectType>(), std::numeric_limits<double>::max() });
			}
			else {
				population.push_back({ selectedPopulation[0]->copy(), std::numeric_limits<double>::max() });
				selectedPopulation.erase(selectedPopulation.begin());				
			}
		}
	}
	evaluation(population);
	std::sort(population.begin(), population.end(), [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
	return std::move(population[0]);
}