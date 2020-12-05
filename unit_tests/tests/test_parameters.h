/*
 * test_parameters.h
 *
 *  Created on: Jan 17, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <unordered_map>
using namespace std;

class TestParameters {
public:
	unordered_map<string, float> values;
	TestParameters* put(string key, float value);
	float get(string key);
};

