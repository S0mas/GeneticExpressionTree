#pragma once
#include "TreeNode.h"
#include "OperatorMapper.h"
#include "utils.h"
#include <iostream>

class TreeBuilder {
	int minConstant = 0;
	int maxConstant = 0;
	std::unique_ptr<TreeNode> getRandomTreeNode() {
		switch (getRandom()% 4) {
		case 0:
			return buildTreeNode(getRandom(maxConstant, minConstant));
		case 1:
			return buildTreeNode(TreeNode::expData.getRandomVariable());
		default:
			return buildTreeNode(OperatorMapper::getRandomOperator());
		}
	}

	std::unique_ptr<TreeNode> buildTreeNode(const int value) {
		return std::make_unique<ValueNode>(value);
	}

	std::unique_ptr<TreeNode> buildTreeNode(const unsigned variableId) {
		return std::make_unique<VariableNode>(variableId);
	}

	std::unique_ptr<TreeNode> buildTreeNode(const std::string& operator_) {
		if (auto p = OperatorMapper::getOperator1ArgFunction(operator_))
			return std::make_unique<Operator1Arg>(p->first, p->second, getRandomTreeNode());
		if (auto p = OperatorMapper::getOperator2ArgFunction(operator_))
			return std::make_unique<Operator2Arg>(p->first, p->second, getRandomTreeNode(), getRandomTreeNode());
		throw std::runtime_error("This operator is not supported!");
	}
public:
	std::unique_ptr<TreeNode> generateTree(const int minConstant = -100, const int maxConstant = 100) {
		this->minConstant = minConstant;
		this->maxConstant = maxConstant;
		return getRandomTreeNode();
	}
};
