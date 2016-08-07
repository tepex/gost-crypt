/**
 * Unit tests functions prototypes and constants.
 * Created on: Jun 24, 2016
 *
 * @author Tepex <tepex@mail.ru>
 */


#ifndef __LIB_TESTS_H__
#define __LIB_TESTS_H__

#include <stdint.h>
#include <stdio.h>
#include "../../main/jni/inc/libgost.h"

#define LIB_FILE "build/libgost.dylib"
#define LIB_CHUNK_SET "chunk_set"
#define LIB_INIT_CHUNK "init_chunk"
#define LIB_PRINT "sprint_hex_array"
#define LIB_TO_STRING "to_string"

#define LIB_GN "gN"
#define LIB_XOR "XOR"
#define LIB_S "S"
#define LIB_P "P"
#define LIB_L "L"
#define LIB_LPSX "LPSX"
#define LIB_ADD512 "add512"
#define LIB_STREEBOG "streebog"

#define CASES_DIR "tests/cases/"

typedef int (*FUNC_PRINT)(char *dst, const unsigned char *msg, uint64_t length);
typedef int (*FUNC_TO_STRING)(char *dst, const WORD *chunk, bool is_512, bool is_le);
typedef int (*FUNC_CHUNK_SET)(WORD *chunk, const WORD w);
typedef int (*FUNC_INIT_CHUNK)(WORD *chunk, const char *str);

typedef int (*FUNC_A)(WORD *chunk); /* For S, P, L */
typedef int (*FUNC_B)(WORD *dst, const WORD *chunk); /* For XOR, LPSX */
typedef int (*FUNC_C)(WORD *dst, const WORD *m, const WORD *N); /* For Gn */
typedef int (*FUNC_STREEBOG)(WORD *dst, const char *msg, uint64_t len, bool is_512); /* For Gn */

int test_streebog();
int test_cases(const char *func_name);
int test_case_a(FUNC_A lib_function, const char *func_name, unsigned int n);
int test_case_b(FUNC_B lib_function, const char *func_name, unsigned int n);
int test_case_c(FUNC_C lib_function, const char *func_name, unsigned int n);
int read_file(WORD *chunk, const char *func_name, const char *suffix, unsigned int test_n);

#endif /* __LIB_TESTS_H__ */
