#pragma once
#include <string>
#include <random>
#include <iostream>
template <class Container>
void split(const std::string& str, Container& cont) {
	std::istringstream iss(str);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(cont));
}

int getRandom(const unsigned max = std::numeric_limits<unsigned>::max(), const int min = 0) {
	std::random_device rd;
	return rd()%max + min;
}