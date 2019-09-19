#include <QCoreApplication>
#include "TreeNode.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto v1 = new ValueNode(4);
    auto v2 = new ValueNode(5);
    Operator2Arg t1(std::multiplies<int>(), v1, v2);
    std::cout << t1.getValue() <<std::endl;
    auto t2(t1);
    std::cout << t2.getValue() <<std::endl;
    v1->setValue(10);
    v2->setValue(15);
    std::cout << t1.getValue() <<std::endl;
    std::cout << t2.getValue() <<std::endl;
    return a.exec();
}
