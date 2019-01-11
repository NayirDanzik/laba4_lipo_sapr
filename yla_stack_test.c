/*
    Int stack: Test

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

#include "yla_stack.h"
#include "yla_test.h"

static int test_init_0()
{
    yla_stack stack;
    yla_stack_init(&stack, 0);

    yla_int_type result = 0;

    YLATEST_ASSERT_TRUE(yla_stack_is_empty(&stack), "stack must be empty after pull last value");
    YLATEST_ASSERT_TRUE(yla_stack_is_full(&stack), "stack couldn't be full after last pull");

    yla_stack_done(&stack);
}

static int test_init_3()
{
    yla_stack stack;
    yla_stack_init(&stack, 3);

    yla_int_type result = 0;

    YLATEST_ASSERT_TRUE(yla_stack_is_empty(&stack), "stack must be empty after pull last value");
    YLATEST_ASSERT_FALSE(yla_stack_is_full(&stack), "stack couldn't be full after last pull");

    YLATEST_ASSERT_FALSE(yla_stack_pull(&stack, &result), "couldn't pull value from empty stack");

    yla_stack_done(&stack);
}

static int test_push_pull()
{
    yla_stack stack;
    yla_stack_init(&stack, 3);

    yla_int_type result = 0;

    YLATEST_ASSERT_TRUE(yla_stack_is_empty(&stack), "stack must empty just after creating");
    YLATEST_ASSERT_FALSE(yla_stack_is_full(&stack), "stack size more then 0 couldn't be full just after creating");
    YLATEST_ASSERT_FALSE(yla_stack_top(&stack, &result), "no top can be accessible just after creating");

    YLATEST_ASSERT_TRUE(yla_stack_push(&stack, 33), "stack with deep 3 could able to push 1 value");
    YLATEST_ASSERT_TRUE(yla_stack_top(&stack, &result), "stack with deep 3 could able to push 1 value");

    YLATEST_ASSERT_FALSE(yla_stack_is_empty(&stack), "stack must NOT empty after 1 push");
    YLATEST_ASSERT_TRUE(yla_stack_push(&stack, 45), "stack with deep 3 could able to push 2 value");
    YLATEST_ASSERT_TRUE(yla_stack_push(&stack, 77), "stack with deep 3 could able to push 3 value");

    YLATEST_ASSERT_TRUE(yla_stack_is_full(&stack), "stack with deep 3 must be full after 3 push");
    YLATEST_ASSERT_FALSE(yla_stack_push(&stack, 99), "stack with deep 3 could NOT able to push 4 value");
    YLATEST_ASSERT_TRUE(yla_stack_is_full(&stack), "stack with deep 3 must be full after 3 push");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 1 value");
    YLATEST_ASSERT_TRUE(result==77, "incorrect value after 1 pull");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 2 value");
    YLATEST_ASSERT_TRUE(result==45, "incorrect value after 2 pull");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 3 value");
    YLATEST_ASSERT_TRUE(result==33, "incorrect value after 3 pull");


    YLATEST_ASSERT_FALSE(yla_stack_pull(&stack, &result), "stack with deep 3 could NOT able to pull 4 value");

    YLATEST_ASSERT_TRUE(yla_stack_is_empty(&stack), "stack must be empty after pull last value");
    YLATEST_ASSERT_FALSE(yla_stack_is_full(&stack), "stack couldn't be full after last pull");

    yla_stack_done(&stack);
}

static int test_deep_0()
{
    yla_stack stack;
    yla_stack_init(&stack, 3);

    yla_int_type result = 0;

    YLATEST_ASSERT_TRUE(yla_stack_is_empty(&stack), "for empty stack");
    YLATEST_ASSERT_FALSE(yla_stack_is_full(&stack), "for empty stack");

    YLATEST_ASSERT_FALSE(yla_stack_get_deep(&stack, 0, &result), "for empty stack");
    YLATEST_ASSERT_FALSE(yla_stack_set_deep(&stack, 0, 99), "for empty stack");

    yla_stack_done(&stack);
}

static int test_deep_n()
{
    yla_stack stack;
    yla_stack_init(&stack, 3);

    yla_int_type result = 0;

    yla_stack_push(&stack, 33);

    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 0, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==33, "yla_stack_get_deep returns incorrect value for index 0");

    YLATEST_ASSERT_FALSE(yla_stack_get_deep(&stack, 1, &result), "yla_stack_get_deep must return OK if stack deep OK");


    yla_stack_push(&stack, 45);

    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 0, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==45, "yla_stack_get_deep returns incorrect value for index 0");

    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 1, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==33, "yla_stack_get_deep returns incorrect value for index 1");

    YLATEST_ASSERT_FALSE(yla_stack_get_deep(&stack, 2, &result), "yla_stack_get_deep must return OK if stack deep OK");


    yla_stack_push(&stack, 77);

    YLATEST_ASSERT_FALSE(yla_stack_get_deep(&stack, 100, &result), "yla_stack_get_deep must return error if stack deep exceeded");

    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 0, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==77, "yla_stack_get_deep returns incorrect value for index 0");

    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 1, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==45, "yla_stack_get_deep returns incorrect value for index 1");

    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 2, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==33, "yla_stack_get_deep returns incorrect value for index 2");

    YLATEST_ASSERT_FALSE(yla_stack_get_deep(&stack, 3, &result), "yla_stack_get_deep must return error if stack deep exceeded");


    YLATEST_ASSERT_FALSE(yla_stack_set_deep(&stack, 100, 98), "yla_stack_set_deep must return error if stack deep exceeded");

    YLATEST_ASSERT_TRUE(yla_stack_set_deep(&stack, 0, 78), "yla_stack_set_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 0, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==78, "yla_stack_get_deep returns incorrect value for index 0");

    YLATEST_ASSERT_TRUE(yla_stack_set_deep(&stack, 1, 46), "yla_stack_set_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 1, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==46, "yla_stack_get_deep returns incorrect value for index 1");

    YLATEST_ASSERT_TRUE(yla_stack_set_deep(&stack, 2, 34), "yla_stack_set_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(yla_stack_get_deep(&stack, 2, &result), "yla_stack_get_deep must return OK if stack deep OK");
    YLATEST_ASSERT_TRUE(result==34, "yla_stack_get_deep returns incorrect value for index 2");

    YLATEST_ASSERT_FALSE(yla_stack_set_deep(&stack, 3, 99), "yla_stack_get_deep must return error if stack deep exceeded");


    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 1 value");
    YLATEST_ASSERT_TRUE(result==78, "incorrect value after 1 pull");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 2 value");
    YLATEST_ASSERT_TRUE(result==46, "incorrect value after 2 pull");

    YLATEST_ASSERT_TRUE(yla_stack_pull(&stack, &result), "stack with deep 3 could able to pull 3 value");
    YLATEST_ASSERT_TRUE(result==34, "incorrect value after 3 pull");


    yla_stack_done(&stack);
}

YLATEST_BEGIN(yla_stack_test)
  YLATEST_ADD_TEST_CASE(test_init_0)
  YLATEST_ADD_TEST_CASE(test_init_3)
  YLATEST_ADD_TEST_CASE(test_push_pull)
  YLATEST_ADD_TEST_CASE(test_deep_0)
  YLATEST_ADD_TEST_CASE(test_deep_n)
YLATEST_END
