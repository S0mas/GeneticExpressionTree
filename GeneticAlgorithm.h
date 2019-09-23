#pragma once
#include "TreeBuilder.h"
#include "utils.h"
#include <vector>

void getRandomNodeHelper(std::vector<TreeNode*>& nodes, TreeNode* node) noexcept {
	nodes.push_back(node);
	for (auto child : node->getChilds())
		getRandomNodeHelper(nodes, child);
}

TreeNode* getRandomNode(TreeNode* tree) noexcept {
	std::vector<TreeNode*> nodes;
	getRandomNodeHelper(nodes, tree);
	return nodes[getRandom(nodes.size())];
}

void evaluation(std::vector<std::unique_ptr<TreeNode>>& population) {
	for (auto& tree : population)
		tree->evaluate();
}

void selection(std::vector<std::unique_ptr<TreeNode>>& population) {
	std::sort(population.begin(), population.end(), [](const auto & lhs, const auto & rhs) { return lhs->getFitness() < rhs->getFitness(); });
	population.erase(population.begin() + population.size()/2, population.end());
}

std::pair<std::unique_ptr<TreeNode>, std::unique_ptr<TreeNode>> crossover(const TreeNode* lhs, const TreeNode* rhs) {
	std::pair<std::unique_ptr<TreeNode>, std::unique_ptr<TreeNode>> childs{ lhs->copy(), rhs->copy() };
	auto randomNode1 = getRandomNode(childs.first.get());
	auto randomNode2 = getRandomNode(childs.second.get());
	auto tmp = randomNode1->copy();
	*randomNode1 = std::move(*randomNode2);
	*randomNode2 = std::move(*tmp.get());
	return childs;
}

std::unique_ptr<TreeNode> mutation(const TreeNode* tree) {
	auto copyToMutate = tree->copy();
	auto randomNode = getRandomNode(copyToMutate.get());
	TreeBuilder builder;
	*randomNode = std::move(*builder.generateTree());
	return copyToMutate;
}

void process(const unsigned populationSize = 1000, const unsigned crossoverChance = 10, const unsigned mutationChance = 0, const unsigned epochs = 50) {
	std::vector<std::unique_ptr<TreeNode>> population;
	population.reserve(1000);
	TreeBuilder builder;
	for(auto i = 0u; i < populationSize; ++i)
		population.push_back(builder.generateTree());

	for (auto epoch = 0u; epoch < epochs; ++epoch) {
		evaluation(population);
		selection(population);
		std::vector<TreeNode*> selectedPopulation;
		selectedPopulation.reserve(499);
		std::transform(population.begin()+1, population.end(), std::back_inserter(selectedPopulation), [](auto & treePtr) { return treePtr.get(); });

		while (!selectedPopulation.empty()) {
			auto r = getRandom(100);
			if (r < crossoverChance && selectedPopulation.size() > 1) {
				auto parentId1 = getRandom(selectedPopulation.size());
				auto parentId2 = getRandom(selectedPopulation.size());
				while(parentId2 == parentId1)
					parentId2 = getRandom(selectedPopulation.size());
				auto childs = crossover(selectedPopulation[parentId1], selectedPopulation[parentId2]);
				population.push_back(std::move(childs.first));
				population.push_back(std::move(childs.second));
				if (parentId1 > parentId2) {
					selectedPopulation.erase(selectedPopulation.begin() + parentId1);
					selectedPopulation.erase(selectedPopulation.begin() + parentId2);
				}
				else {
					selectedPopulation.erase(selectedPopulation.begin() + parentId2);
					selectedPopulation.erase(selectedPopulation.begin() + parentId1);
				}
			}
			else if(false) {
				auto treeId = getRandom(selectedPopulation.size());
				population.push_back(mutation(selectedPopulation[treeId]));
				selectedPopulation.erase(selectedPopulation.begin() + treeId);
			}
			else {
				TreeBuilder tb;
				selectedPopulation.erase(selectedPopulation.begin());
				population.push_back(tb.generateTree());
			}
		}
		std::cout << "Best:\n" << population[0]->toString() << std::endl << population[0]->getFitness() << std::endl;
	}
}