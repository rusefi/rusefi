/*
 * test_parameters.h
 *
 *  Created on: Jan 17, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#ifndef TESTS_TEST_PARAMETERS_H_
#define TESTS_TEST_PARAMETERS_H_

#include <unordered_map>
using namespace std;

class TestParameters {
public:
	unordered_map<string, float> values;
	TestParameters* put(string key, float value);
	float get(string key) const;
};

#endif /* TESTS_TEST_PARAMETERS_H_ */
