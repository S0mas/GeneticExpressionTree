#include <QCoreApplication>
#include "TreeBuilder.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    VariablesMap::setVariableValue('x', 4);
    VariablesMap::setVariableValue('y', 7);
    TreeBuilder builder;
    auto tree = builder.generateTree(0, 20);
    std::cout << tree->toString() <<std::endl;
    std::cout << tree->getValue() <<std::endl;
    return a.exec();
}
