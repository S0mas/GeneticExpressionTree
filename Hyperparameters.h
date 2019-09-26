#pragma once
#include "utils.h"
#include <memory>
#include <map>
#include <thread>
#include <chrono>

struct Hyperparameters {
	unsigned populationSize = 100;
	unsigned crossoverChance = 20;
	unsigned mutationChance = 30;
	unsigned populationErase = 50;
	unsigned randomAdditionChance = 5;
	unsigned epochs = 100;

	void setParameterByNumber(const unsigned parameterId, const unsigned value) noexcept {
		switch (parameterId) {
		//case 0:
		//	populationSize = value;
		//	return;
		case 0:
			crossoverChance = value;
			return;
		case 1:
			mutationChance = value;
			return;
		case 2:
			randomAdditionChance = value;
			return;
		default:
			epochs = value;
			return;
		}
	}

	unsigned getParameterByNumber(const unsigned parameterId) const noexcept {
		switch (parameterId) {
		//case 0:
		//	return populationSize;
		case 0:
			return crossoverChance;
		case 1:
			return mutationChance;
		case 2:
			return randomAdditionChance;
		default:
			return epochs;
		}
	}

	std::string toString() const {
		return "PopulationSize:" + std::to_string(populationSize) + " crossoverChance:" + std::to_string(crossoverChance) + " mutationChance:" + std::to_string(mutationChance) +
			" randomAdditionChance:" + std::to_string(randomAdditionChance) + " populationErase:" + std::to_string(populationErase) +" epochs:" + std::to_string(epochs);
	}

	std::unique_ptr<Hyperparameters> copy() const noexcept {
		return std::make_unique<Hyperparameters>(*this);
	}
};

class HyperparametersBuilder {
public:
	inline static std::unique_ptr<Hyperparameters> generateHyperparams() noexcept {
		auto hyperparams = std::make_unique<Hyperparameters>();
		hyperparams->populationSize = 100;
		hyperparams->crossoverChance = getRandomNumber(95, 55);
		hyperparams->mutationChance = getRandomNumber(95,55);
		hyperparams->randomAdditionChance = getRandomNumber(15, 5);
		hyperparams->epochs = getRandomNumber(60, 40);
		hyperparams->populationErase = getRandomNumber(50, 20);
		return hyperparams;
	}
};

std::pair<std::unique_ptr<Hyperparameters>, std::unique_ptr<Hyperparameters>> crossover(const Hyperparameters* lhs, const Hyperparameters* rhs) {
	std::pair<std::unique_ptr<Hyperparameters>, std::unique_ptr<Hyperparameters>> childs{ std::make_unique<Hyperparameters>(*lhs), std::make_unique<Hyperparameters>(*rhs) };
	auto parameterToCross1 = getRandomNumber(4);
	auto parameterToCross2 = getRandomNumber(4);
	while(parameterToCross1 == parameterToCross2)
		parameterToCross2 = getRandomNumber(4);

	auto newParamValue1 = (lhs->getParameterByNumber(parameterToCross1) + rhs->getParameterByNumber(parameterToCross1)) / 2;
	childs.first->setParameterByNumber(parameterToCross1, newParamValue1);
	auto newParamValue2 = (lhs->getParameterByNumber(parameterToCross2) + rhs->getParameterByNumber(parameterToCross2)) / 2;
	childs.second->setParameterByNumber(parameterToCross2, newParamValue1);

	return childs;
}

std::unique_ptr<Hyperparameters> mutation(const Hyperparameters* params) {
	auto copyToMutate = std::make_unique<Hyperparameters>(*params);
	auto parameterToMutate = getRandomNumber(4);
	auto newValue = copyToMutate->getParameterByNumber(parameterToMutate) + getRandomNumber(10, -10);
	if (newValue < 0)
		newValue = 0;
	else if (parameterToMutate == 3 && newValue > 100)
		newValue = 100;
	copyToMutate->setParameterByNumber(parameterToMutate, newValue);
	return copyToMutate;
}
