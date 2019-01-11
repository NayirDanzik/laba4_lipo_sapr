/*
    Test mini-framework

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


#ifndef _yla_test_h
#define _yla_test_h

#include <string.h>
#include <stdio.h>


#define YLATEST_ASSERT_TRUE(expr, message) {\
  if (!(expr)) {\
    printf("%s:%d '%s' %s\n", __FILE__, __LINE__, #expr, message);\
    return -1;\
  }\
}

#define YLATEST_ASSERT_FALSE(expr, message) {\
  if ((expr)) {\
    printf("%s:%d '%s' %s\n", __FILE__, __LINE__, #expr, message);\
    return -1;\
  }\
}

#define YLATEST_RESULT(result) ((result)?"FAILED":"PASSED")

#define YLATEST_BEGIN(name) int name()\
{\
  int __yla_test_result = 0;\
  char *__yla_test_name = #name ;\
  printf("Start TEST %s\n", __yla_test_name);

#define YLATEST_END \
  printf("Finished TEST %s %s\n", __yla_test_name, YLATEST_RESULT(__yla_test_result));\
  return __yla_test_result;\
}


#define YLATEST_ADD_TEST_CASE(name) \
 {\
    char *__yla_test_case_name = #name;\
    printf("Start TESTCASE %s\n", __yla_test_case_name);\
    int __yla_test_case_result = name();\
    printf("Finished TESTCASE %s %s\n", __yla_test_case_name, YLATEST_RESULT(__yla_test_case_result));\
    __yla_test_result |= __yla_test_case_result;\
  }


#define YLATEST_SUITE_BEGIN(name) \
int name()\
{\
  int __yla_test_suite_result = 0;\
  char *__yla_test_suite_name = #name ;\
  printf("Start TESTSUITE %s\n", __yla_test_suite_name);

#define YLATEST_SUITE_END \
  printf("Finished TEST SUITE %s %s\n", __yla_test_suite_name, YLATEST_RESULT(__yla_test_suite_result));\
  return __yla_test_suite_result;\
}

#define YLATEST_ADD_TEST(name) \
 {\
    extern name();\
    int __yla_test_result = name();\
    __yla_test_suite_result |= __yla_test_result;\
  }


#endif

