#include <QCoreApplication>
#include "GeneticAlgorithm.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TreeNode::expData.readExpressionFromFile("data.txt");
    process();

    return a.exec();
}
