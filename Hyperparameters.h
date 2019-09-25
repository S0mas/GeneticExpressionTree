#pragma once

struct Hyperparameters {
	unsigned populationSize = 200;
	unsigned crossoverChance = 20;
	unsigned mutationChance = 30;
	unsigned randomAdditionChance = 5;
	unsigned epochs = 100;
};