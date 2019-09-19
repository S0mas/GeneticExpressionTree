#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

std::map<char, int> variablesMap;

class TreeNode {
protected:
    std::vector<std::unique_ptr<TreeNode>> childs;
public:
    TreeNode() noexcept  = default;
    TreeNode(const TreeNode& node) {
        for(auto const& child : node.childs)
            childs.emplace_back(child->copy());
    }
    TreeNode(TreeNode&& node) noexcept = default;
    TreeNode& operator=(const TreeNode& node) = default;
    TreeNode& operator=(TreeNode&& node) noexcept = default;
    virtual ~TreeNode() = default;


    virtual int getValue() const = 0;
    virtual std::unique_ptr<TreeNode> copy() const = 0;
};

class ValueNode : public TreeNode {
    int value;
public:
    ValueNode(const int value) noexcept : TreeNode(), value(value){}

    int getValue() const override {
        return value;
    }
    void setValue(const int newValue) {
        value = newValue;
    }
    std::unique_ptr<TreeNode> copy() const override {
        return std::make_unique<ValueNode>(*this);
    }
};

class OperatorNode : public TreeNode {
public:
    OperatorNode() noexcept : TreeNode(){}
};

class Operator1Arg : public OperatorNode {
    std::function<int(int)> f;
public:
    Operator1Arg(const std::function<int(int)>& f, TreeNode* argNode) noexcept : OperatorNode(), f(f){
        childs.push_back(std::unique_ptr<TreeNode>(argNode));
    }

    int getValue() const override {
        return f(childs[0]->getValue());
    }
    std::unique_ptr<TreeNode> copy() const override {
        return std::make_unique<Operator1Arg>(*this);
    }
};

class Operator2Arg : public OperatorNode {
    std::function<int(int, int)> f;
public:
    Operator2Arg(const std::function<int(int, int)>& f, TreeNode* argNode1, TreeNode* argNode2) noexcept : OperatorNode(), f(f){
        childs.push_back(std::unique_ptr<TreeNode>(argNode1));
        childs.push_back(std::unique_ptr<TreeNode>(argNode2));
    }

    int getValue() const override {
        return f(childs[0]->getValue(), childs[1]->getValue());
    }

    std::unique_ptr<TreeNode> copy() const override {
        return std::make_unique<Operator2Arg>(*this);
    }
};

class VariableNode : public TreeNode {
    char sign;
public:
    int getValue() const override {
        return variablesMap[sign];
    }
    std::unique_ptr<TreeNode> copy() const override  {
        return std::make_unique<VariableNode>(*this);
    }
};
