#pragma once
#include <memory>
#include "TreeBuilder.h"
#include "Hyperparameters.h"

class Builder {
public:

	template<typename ObjectToBuild>
	inline static std::unique_ptr<ObjectToBuild> build() {
		return std::make_unique<ObjectToBuild>();
	}

	template<>
	inline static std::unique_ptr<TreeNode> build() {
		TreeBuilder tb;
		return tb.generateTree();
	}

	template<>
	inline static std::unique_ptr<Hyperparameters> build() {
		return HyperparametersBuilder::generateHyperparams();
	}
};