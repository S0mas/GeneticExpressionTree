#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "utils.h"

class ExpressionData {
	std::vector<std::vector<double>> valuesLines;

	mutable uint64_t activeValuesId = 0;
public:
	unsigned getVariablesNo() const noexcept {
		return valuesLines.empty() ? 0 : static_cast<unsigned>(valuesLines[0].size() - 1);
	}
	void readExpressionFromFile(const std::string & fileName) noexcept {
		std::ifstream file(fileName);
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				std::vector<std::string> valuesStr;
				split(line, valuesStr);
				std::vector<double> values;
				std::transform(valuesStr.begin(), valuesStr.end(), std::back_inserter(values),
					[](const std::string & s) { return stod(s); });
				if (valuesLines.empty() || (!valuesLines.empty() && values.size() == valuesLines.back().size()))
					valuesLines.push_back(std::move(values));
			}
		}
	}

	unsigned int getRandomVariable() const noexcept {
		return getRandomNumber(getVariablesNo());
	}

	double getVariableValue(const unsigned variableId) const noexcept {
		return valuesLines[activeValuesId][variableId];
	}

	double getExpectedResult() const noexcept {
		return valuesLines[activeValuesId].back();
	}

	bool next() const noexcept {
		if (activeValuesId + 1 < valuesLines.size()) {
			++activeValuesId;
			return true;
		}
		activeValuesId = 0;
		return false;
	}
};


