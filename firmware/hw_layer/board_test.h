/**
 * @file	board_test.h
 *
 * @date Mar 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BOARD_TEST_H_
#define BOARD_TEST_H_

void initBoardTest(void);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

bool isBoardTestMode(void);
void printBoardTestState(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BOARD_TEST_H_ */
