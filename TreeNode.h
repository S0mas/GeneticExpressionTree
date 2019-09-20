#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "VariablesMap.h"

class TreeNode {
protected:
    std::vector<std::unique_ptr<TreeNode>> childs;
public:
    TreeNode() noexcept  = default;

    TreeNode(const TreeNode& node) {
        for(auto const& child : node.childs)
            childs.emplace_back(child->copy());
    }

    TreeNode(TreeNode&& node) noexcept {
        childs = std::move(node.childs);
    }

    TreeNode& operator=(const TreeNode& node) {
        childs.clear();
        for(auto const& child : node.childs)
            childs.emplace_back(child->copy());
        return *this;
    }

    TreeNode& operator=(TreeNode&& node) noexcept {
        childs = std::move(node.childs);
        return *this;
    }

    virtual ~TreeNode() = default;
    virtual double getValue() const = 0;
    virtual std::unique_ptr<TreeNode> copy() const = 0;
    virtual std::string toString() const = 0;
};

class ValueNode : public TreeNode {
    double value;
public:
    ValueNode(const double value) noexcept : TreeNode(), value(value){}

    double getValue() const override {
        return value;
    }

    void setValue(const double newValue) {
        value = newValue;
    }

    std::unique_ptr<TreeNode> copy() const override {
        return std::make_unique<ValueNode>(*this);
    }

    std::string toString() const override {
        return std::to_string(value);
    }
};

class VariableNode : public TreeNode {
    char sign;
public:
    VariableNode(const char variable) noexcept : TreeNode(), sign(variable){}
    double getValue() const override {
        return VariablesMap::getVariableValue(sign);
    }

    std::unique_ptr<TreeNode> copy() const override  {
        return std::make_unique<VariableNode>(*this);
    }

    std::string toString() const override {
        return std::string(1, sign);
    }
};

class OperatorNode : public TreeNode {
protected:
    std::string operator_;
public:
    OperatorNode(const std::string& operator_) noexcept : TreeNode(), operator_(operator_){}
};

class Operator1Arg : public OperatorNode {
    std::function<double(double)> f;
public:
    Operator1Arg(const std::string& op_, const std::function<double(double)>& f, std::unique_ptr<TreeNode>&& argNode) noexcept : OperatorNode(op_), f(f){
        childs.push_back(std::move(argNode));
    }

    double getValue() const override {
        return f(childs[0]->getValue());
    }

    std::unique_ptr<TreeNode> copy() const override {
        return std::make_unique<Operator1Arg>(*this);
    }

    std::string toString() const override {
        return operator_ + "(" + childs[0]->toString() + ")";
    }
};

class Operator2Arg : public OperatorNode {
    std::function<double(double, double)> f;
public:
    Operator2Arg(const std::string& op_, const std::function<double(double, double)>& f, std::unique_ptr<TreeNode>&& argNode1, std::unique_ptr<TreeNode>&& argNode2) noexcept : OperatorNode(op_), f(f){
        childs.push_back(std::move(argNode1));
        childs.push_back(std::move(argNode2));
    }

    double getValue() const override {
        return f(childs[0]->getValue(), childs[1]->getValue());
    }

    std::unique_ptr<TreeNode> copy() const override {
        return std::make_unique<Operator2Arg>(*this);
    }

    std::string toString() const override {
        return "(" + childs[0]->toString() + " " + operator_ + " " + childs[1]->toString() + ")";
    }
};
