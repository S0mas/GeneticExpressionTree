#include <QCoreApplication>
#include "GeneticAlgorithm.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	TreeNode::expData.readExpressionFromFile("data.txt");
	auto c = process<TreeNode>();
	while (c.second != 0) {
		c = process<TreeNode>();
		std::cout << c.first->toString() << "\n" << c.second << std::endl;
	}

	std::cout << "WINNER!!!!!!!!!!!!!!!!!" << std::endl;

    return a.exec();
}
