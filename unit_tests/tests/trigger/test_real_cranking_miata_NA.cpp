/*
 * @file test_real_cranking_miata_NA.cpp
 *
 * @date May 22, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "engine_test_helper.h"

TEST(cranking, realNA) {
	FILE *fp = fopen("tests/trigger/recourses/cranking_na_3.csv", "r");

	char *line = nullptr;
	size_t len = 0;
	ssize_t read;

	int index = -1;
	while ((read = getline(&line, &len, fp)) != -1) {
		index++;
		if (index == 0) {
			// skip header
			continue;
		}
		const char s[2] = ",";

		char *token;

		/* get the first token */
		token = strtok(line, s);

		/* walk through other tokens */
		while (token != NULL) {
//			printf(" %s\n", token);

			token = strtok(NULL, s);
		}

	}

	ASSERT_TRUE(fp != nullptr);
}
