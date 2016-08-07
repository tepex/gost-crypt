/**
 * Macros for unit testing.
 * Created on: Jun 24, 2016
 *
 * @author Tepex <tepex@mail.ru>
 */

#ifndef __MINUNIT_H__
#define __MINUNIT_H__

#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"

#define START_TESTS()\
static int tests_run = 0;\
static int tests_errors = 0;\

#define ASSERT(T, M, ...) if (!(T))\
{\
	LOG_ERR(M, ##__VA_ARGS__);\
	return -1;\
}\

#define RUN_TEST(F)\
	++tests_run;\
	printf("%u. Test %s: ", tests_run, #F);\
	if(F() != 0)\
	{\
		++tests_errors;\
		printf("fail.\n");\
		return -1;\
	}\
	else printf("OK.\n");\

#endif /* __MINUNIT_H__ */
