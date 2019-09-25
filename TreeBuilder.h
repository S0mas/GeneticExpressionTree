#pragma once
#include "TreeNode.h"
#include "OperatorMapper.h"
#include "utils.h"
#include <iostream>

class TreeBuilder {
	int minConstant = 0;
	int maxConstant = 0;
	std::unique_ptr<TreeNode> getRandomTreeNode() {
		switch (getRandomNumber(3)) {
		case 0:
			return buildTreeNode(getRandomNumber(maxConstant, minConstant));
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

void getChildsCountHelper(unsigned& childsCount, const std::unique_ptr<TreeNode>& node) noexcept {
	for (const auto& child : node->getChilds())
		getChildsCountHelper(++childsCount, child);
}

unsigned getChildsCount(const std::unique_ptr<TreeNode> & node) noexcept {
	auto childsCount = 0u;
	getChildsCountHelper(childsCount, node);
	return childsCount;
}

void getRandomNodeHelper(int& randomNodeId, std::unique_ptr<TreeNode> & node, std::unique_ptr<TreeNode> * *result) noexcept {
	if (randomNodeId == 0)
		* result = &node;
	for (auto& child : node->getChilds())
		getRandomNodeHelper(--randomNodeId, child, result);
}

std::unique_ptr<TreeNode>& getRandomNode(std::unique_ptr<TreeNode> & tree) noexcept {
	auto randomNodeId = getRandomNumber(getChildsCount(tree) + 1);
	std::unique_ptr<TreeNode> * *ptr = new std::unique_ptr<TreeNode> * ();
	*ptr = nullptr;
	getRandomNodeHelper(randomNodeId, tree, ptr);
	return **ptr;
}

std::pair<std::unique_ptr<TreeNode>, std::unique_ptr<TreeNode>> crossover(const TreeNode* lhs, const TreeNode* rhs) {
	std::pair<std::unique_ptr<TreeNode>, std::unique_ptr<TreeNode>> childs{ lhs->copy(), rhs->copy() };
	auto& randomNode1 = getRandomNode(childs.first);
	auto& randomNode2 = getRandomNode(childs.second);
	randomNode1.swap(randomNode2);
	return childs;
}

std::unique_ptr<TreeNode> mutation(const TreeNode* tree) {
	auto copyToMutate = tree->copy();
	auto& randomNode = getRandomNode(copyToMutate);
	TreeBuilder builder;
	auto ptr = builder.generateTree();
	randomNode.swap(ptr);
	return copyToMutate;
}
