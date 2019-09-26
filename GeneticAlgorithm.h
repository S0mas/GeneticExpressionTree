#pragma once
#include "Builder.h"
#include "utils.h"
#include "Hyperparameters.h"
#include <vector>
#include <optional>

template<typename ObjectType>
auto initPopulation(const size_t populationSize) {
	std::vector<std::pair<std::unique_ptr<ObjectType>, double>> population;
	population.reserve(populationSize);
	for (auto i = 0u; i < populationSize; ++i)
		population.push_back({ Builder::build<ObjectType>(), std::numeric_limits<double>::max() });
	return population;
}

template<typename ObjectType>
std::optional<std::pair<std::unique_ptr<ObjectType>, double>> evaluation(std::vector<std::pair<std::unique_ptr<ObjectType>, double>>& population) {
	for (auto& pair : population) {
		auto result = evaluate(pair.first);
		pair.second = result;
		if (floor(result) == 0)
			return std::move(pair);
	}
	return {};
}

template<typename ObjectType>
void selection(std::vector<std::pair<std::unique_ptr<ObjectType>, double>>& population) {
	std::sort(population.begin(), population.end(), [](const auto& lhs, const auto& rhs) { return lhs.second <  rhs.second; });
	population.erase(population.begin() + population.size()/2, population.end());
}

template<typename ObjectType>
auto process(const Hyperparameters hyperparameters = Hyperparameters(), const std::vector<std::pair<std::unique_ptr<ObjectType>, double>>& initPop = {}) {
	std::vector<std::pair<std::unique_ptr<ObjectType>, double>> population;
	if(initPop.empty())
		population = initPopulation<ObjectType>(hyperparameters.populationSize);
	else {
		for (auto const& element : initPop)
			population.push_back({ element.first->copy(), std::numeric_limits<double>::max() });
	}
	for (auto epoch = 0u; epoch < hyperparameters.epochs; ++epoch) {
		if(auto result = evaluation(population))
			return std::move(*result);
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
			else {
				selectedPopulation.erase(selectedPopulation.begin());
				population.push_back({ Builder::build<ObjectType>(), std::numeric_limits<double>::max() });
			}
		}
	}

	return std::move(population[0]);
}

std::vector<std::pair<std::unique_ptr<TreeNode>, double>> population;

double evaluate(const std::unique_ptr<TreeNode>& tree) noexcept {
	double result = 0;
	do {
		result += abs(TreeNode::expData.getExpectedResult() - tree->getValue());
	} while (TreeNode::expData.next());

	return result;
}

double evaluate(const std::unique_ptr<Hyperparameters>& hyperparams) noexcept {
	std::cout << hyperparams->toString();
	auto start = std::chrono::high_resolution_clock::now();
	std::pair<std::unique_ptr<TreeNode>, double> c;
	do {
		c = process<TreeNode>(*hyperparams, population);
		if ((std::chrono::high_resolution_clock::now() - start).count() > 2000000000) {
			std::cout << "\n TARDED\n";
			return 2000000000;
		}

		std::cout << ".";
	} while (c.second != 0);
	
	auto result = (std::chrono::high_resolution_clock::now() - start).count();
	std::cout << "\n time[ms]: " << result/1000'000 << "\n";
	return result;
}