/*
 * ARTInitLagCalculator.cpp
 *
 *  Created on: Sep 22, 2016
 *      Author: nick
 */

#include <exception>

#include "boost/tokenizer.hpp"

#include "DayRangeCalculator.h"

namespace TransModel {

using namespace std;

void tokenize(const string& str, const string& separator, vector<string>& tokens) {
	boost::char_separator<char> sep{separator.c_str()};
	boost::tokenizer<boost::char_separator<char>> tok(str, sep);
	for (const auto& t : tok) {
		tokens.push_back(t);
	}
}


DayRangeBin::DayRangeBin(double min, double max) : gen(repast::Random::instance()->createUniIntGenerator(min, max)) {}
DayRangeBin::~DayRangeBin() {}

double DayRangeBin::calculateLag(float size_of_timestep) {
	return gen.next() / size_of_timestep;
}


DayRangeCalculatorCreator::DayRangeCalculatorCreator() : pd_creator{} {}

DayRangeCalculatorCreator::~DayRangeCalculatorCreator() {

}

void DayRangeCalculatorCreator::addBin(const std::string& bin_definition) {
	vector<string> tokens;
	tokenize(bin_definition, ",", tokens);

	if (tokens.size() != 2) {
		throw std::invalid_argument("Bad bin definition: " + bin_definition);
	}

	double prob = stod(tokens[0]);

	tokens.clear();
	tokenize(tokens[1], "-", tokens);
	double min = 0, max = 0;
	if (tokens.size() == 1) {
		min = stod(tokens[0]);
		max = min;
	} else if (tokens.size() == 2) {
		min = stod(tokens[0]);
		max = stod(tokens[1]);
	} else {
		throw std::invalid_argument("Bad bin definition: " + bin_definition);
	}

	if (min < 0 || max < 0 || max < min) {
		throw std::invalid_argument("Bad bin definition: " + bin_definition + ", probability, min and max must be > 0 and max must be > min.");
	}

	// std::cout << "adding bin " << prob << ", " << min << " - " << max << std::endl;

	if (prob != 0) {
		pd_creator.addItem(prob, make_shared<DayRangeBin>(min, max));
	}
}

std::shared_ptr<DayRangeCalculator> DayRangeCalculatorCreator::createCalculator() {
	return shared_ptr<DayRangeCalculator>(new DayRangeCalculator(pd_creator.createProbDist()));
}

DayRangeCalculator::DayRangeCalculator(ProbDist<DayRangeBin> prob_dist) : dist(prob_dist) {

}

DayRangeCalculator::~DayRangeCalculator() {}

double DayRangeCalculator::calculateLag(float size_of_timestep) {
	double prob = repast::Random::instance()->nextDouble();
	return dist.draw(prob)->calculateLag(size_of_timestep);
}


} /* namespace TransModel */
