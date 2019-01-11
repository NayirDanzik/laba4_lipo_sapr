/*
    Test for test mini-framework
    This file is part of YLA VM (Yet another Language for Academic purpose: Virtual Machine).

    YLA VM is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "yla_test.h"

static int test_assertions()
{
	YLATEST_ASSERT_TRUE(1, "true assertion incorrect");
	YLATEST_ASSERT_FALSE(0, "false assertion incorrect");

	return 0;
}

static int test_assertions2()
{
	YLATEST_ASSERT_TRUE(1, "true assertion incorrect");

// uncomment this to check if test suite work correctly (failing) with failed test case
//	YLATEST_ASSERT_FALSE(1, "false assertion incorrect");

	return 0;
}

YLATEST_BEGIN(yla_test_test)
  YLATEST_ADD_TEST_CASE(test_assertions)
  YLATEST_ADD_TEST_CASE(test_assertions2)
YLATEST_END
