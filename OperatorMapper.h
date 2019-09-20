#pragma once
#include <array>
#include <cmath>
#include <numeric>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <iostream>
#include <QRandomGenerator>

class OperatorMapper {
    inline static const std::array<std::string, 17> operators = {"~", "abs", "exp", "log", "log2", "log10", "sin", "cos", "sinh", "cosh", "floor", "ceil", "*", "/", "+", "-", "^"};
    inline const static std::map<std::string, std::function<double(double)>> operators1ArgToFunctionsMap = {
    {"~", std::negate<double>()}, {"abs", [](const double value){return std::abs(value);}},
    {"exp", [](const double value){return std::exp(value);}}, {"log", [](const double value){return std::log(value);}},
    {"log2", [](const double value){return std::log2(value);}}, {"log10", [](const double value){return std::log10(value);}},
    {"sin", [](const double value){return std::sin(value);}}, {"cos", [](const double value){return std::cos(value);}},
    {"sinh", [](const double value){return std::sinh(value);}}, {"cosh", [](const double value){return std::cosh(value);}},
    {"floor", [](const double value){return std::floor(value);}}, {"ceil", [](const double value){return std::ceil(value);}}};

    inline const static std::map<std::string, const std::function<double(double, double)>&> operators2ArgToFunctionsMap = {
    {"*", [](const double lhs, const double rhs){ return lhs * rhs;}}, {"/", [](const double lhs, const double rhs){ return lhs / rhs;}},
    {"+", [](const double lhs, const double rhs){ return lhs + rhs;}}, {"-", [](const double lhs, const double rhs){ return lhs - rhs;}},
    {"^", [](const double base, const double exponent){return std::pow(base, exponent);}}};
public:
    static std::optional<std::pair<std::string, std::function<double(double)>>> getOperator1ArgFunction(const std::string& operator_) noexcept {
        auto found = operators1ArgToFunctionsMap.find(operator_);
        if (found == operators1ArgToFunctionsMap.end())
              return {};
        return *found;
    }

    static std::optional<std::pair<std::string, std::function<double(double, double)>>> getOperator2ArgFunction(const std::string& operator_) noexcept {
        auto found = operators2ArgToFunctionsMap.find(operator_);
        if (found == operators2ArgToFunctionsMap.end())
            return {};
        return *found;
    }

    static std::string getRandomOperator() noexcept {
        return operators[static_cast<unsigned>(QRandomGenerator::global()->bounded(static_cast<int>(operators.size())))];
    }
};
