#include <QCoreApplication>
#include "GeneticAlgorithm.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	TreeNode::expData.readExpressionFromFile("data.txt");
	population = initPopulation<TreeNode>(100);
	Hyperparameters hyperparams;
	hyperparams.crossoverChance = 30;
	hyperparams.mutationChance = 30;
	hyperparams.randomAdditionChance = 10;
	hyperparams.populationSize = 20;
	hyperparams.epochs = 30;

	auto c = process<Hyperparameters>(hyperparams);
	
	std::cout << "\nWINNER!!!!!!!!!!!!!!!!!" << std::endl;
	std::cout << c.first->toString() << "\n" << c.second << std::endl;

    return a.exec();
}
