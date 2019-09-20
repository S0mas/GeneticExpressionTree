#pragma once
#include "TreeNode.h"
#include "OperatorMapper.h"
#include <iostream>

class TreeBuilder {
    int minConstant = 0;
    int maxConstant = 0;
    std::unique_ptr<TreeNode> getRandomTreeNode() {
        switch(QRandomGenerator::global()->generate()%3) {
        case 0:
            return buildTreeNode(QRandomGenerator::global()->bounded(minConstant, maxConstant));
        case 1:
            return buildTreeNode(VariablesMap::getRandomVariable());
        case 2:
            return buildTreeNode(OperatorMapper::getRandomOperator());
        }
        throw std::runtime_error("Impossible error reached!");
    }

    std::unique_ptr<TreeNode> buildTreeNode(const int value) {
        return std::make_unique<ValueNode>(value);
    }

    std::unique_ptr<TreeNode> buildTreeNode(const char sign) {
        return std::make_unique<VariableNode>(sign);
    }

    std::unique_ptr<TreeNode> buildTreeNode(const std::string& operator_) {
        if(auto p = OperatorMapper::getOperator1ArgFunction(operator_))
            return std::make_unique<Operator1Arg>(p->first, p->second, getRandomTreeNode());
        if(auto p = OperatorMapper::getOperator2ArgFunction(operator_))
            return std::make_unique<Operator2Arg>(p->first, p->second, getRandomTreeNode(), getRandomTreeNode());
        throw std::runtime_error("This operator is not supported!");
    }
public:
    std::unique_ptr<TreeNode> generateTree(const int minConstant, const int maxConstant) {
        this->minConstant = minConstant;
        this->maxConstant = maxConstant;
        return getRandomTreeNode();
    }
};
