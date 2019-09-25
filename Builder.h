#pragma once
#include <memory>
#include "TreeBuilder.h"

class Builder {
public:

	template<typename ObjectToBuild>
	std::unique_ptr<ObjectToBuild> build() {
		return std::make_unique<ObjectToBuild>();
	}

	template<>
	std::unique_ptr<TreeNode> build() {
		TreeBuilder tb;
		return tb.generateTree();
	}
};