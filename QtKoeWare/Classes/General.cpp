#pragma once

#include <string>
#include <random>
#include <list>

static bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

static int Random(int min, int max) {
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_real_distribution<double> dist(min, max);

	return int(dist(mt));
}

struct checkReturnData {
	std::list<std::string> errors;
	std::list<std::string> alerts;
};