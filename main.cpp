#include <QCoreApplication>
#include "GeneticAlgorithm.h"
#include <iostream>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::string pathToDataFile = "D:/Programowanie/GeneticExpressionTree/data.txt";
    static_assert(false, "Specify pathToDataFile (example file is in the repo, data.txt) and comment this out");
    TreeNode::expData.readExpressionFromFile(pathToDataFile);

	std::vector<std::pair<std::unique_ptr<Hyperparameters>, double>> paramsVec;
	for (auto i = 0u; i < 7; ++i) {
		auto hyp = HyperparametersBuilder::generateHyperparams();
		hyp->crossoverChance = 65 - i*5;
		hyp->mutationChance = 20 + i*5;
        hyp->epochs = 200;
		paramsVec.push_back({ std::move(hyp) , 0 });
	}
	auto iterationNo = 100;
	std::unique_ptr<TreeNode> bestAprox;
	double bestResult = std::numeric_limits<double>::max();
	for(auto iteration = 0; iteration < iterationNo; ++iteration){
		population = initPopulation<TreeNode>(paramsVec[0].first->populationSize);
		for (auto i = 0u; i < paramsVec.size(); ++i) {
			auto result = process<TreeNode>(*paramsVec[i].first, population);
			if (result.second < bestResult) {
				bestAprox = std::move(result.first);
				bestResult = result.second;
			}

			system("CLS");
            std::cout << "Iteration :" << iteration + 1 << "/" << iterationNo << "\nObjects tested: " << i + 1 << "/" << paramsVec.size() << std::flush;
			std::cout << "\nBest Result :" << bestResult << ":" << bestAprox->toString() << std::flush;
		}
	}
	for (auto i = 0u; i < paramsVec.size(); ++i)
		paramsVec[i].second /= iterationNo;
	std::sort(paramsVec.begin(), paramsVec.end(), [](auto const& lhs, auto const& rhs) { return lhs.second < rhs.second; });
	for (auto i = 0u; i < paramsVec.size(); ++i)
		std::cout << std::endl << paramsVec[i].first->toString() << std::endl << paramsVec[i].second << "[ms]\n";
	population.clear();

    return a.exec();
}
