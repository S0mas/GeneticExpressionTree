#pragma once
#include <map>
#include <iostream>
#include <QRandomGenerator>

class VariablesMap {
    inline static std::map<char, double> variablesMap;
public:
    static char getRandomVariable() noexcept {
        return std::next(variablesMap.begin(), QRandomGenerator::global()->bounded(static_cast<int>(variablesMap.size())))->first;
    }

    static double getVariableValue(const char sign) noexcept {
        return variablesMap[sign];
    }

    static void reset() noexcept {
        variablesMap.clear();
    }

    static void setVariableValue(const char variable, const double value) {
        variablesMap[variable] = value;
    }
};
