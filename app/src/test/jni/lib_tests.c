/**
 * Unit tests functions.
 * Created on: Jun 24, 2016
 *
 * @author Tepex <tepex@mail.ru>
 */

#include "lib_tests.h"
#include "minunit.h"

#include <dlfcn.h>
#include <string.h>
#include <arpa/inet.h>

static void *lib = NULL;
static FUNC_PRINT lib_print;
static FUNC_CHUNK_SET lib_chunk_set;
static FUNC_INIT_CHUNK lib_init_chunk;

 /* for printing */
static char print_src[2048];
static char print_chunk[1024];

static int ret = 0;
START_TESTS();

int test_dlopen()
{
	lib = dlopen(LIB_FILE, RTLD_NOW);
	ASSERT(lib != NULL, "Failed to open the library \"%s\" to test.", LIB_FILE);
	return 0;
}

int test_dlclose()
{
	ret = dlclose(lib);
	ASSERT(ret == 0, "Failed to close \"%s\".", LIB_FILE);
	return 0;
}

int test_sprint()
{
	lib_print = dlsym(lib, LIB_PRINT);
	ASSERT(lib_print != NULL, "Did not find %s function in the library %s: %s", LIB_PRINT, LIB_FILE, dlerror());
	return 0;
}

int test_chunk_set()
{
	lib_chunk_set = dlsym(lib, LIB_CHUNK_SET);
	ASSERT(lib_chunk_set != NULL, "Did not find %s function in the library %s: %s", LIB_CHUNK_SET, LIB_FILE, dlerror());
	return 0;
}

int test_init_chunk()
{
	lib_init_chunk = dlsym(lib, LIB_INIT_CHUNK);
	ASSERT(lib_init_chunk != NULL, "Did not find %s function in the library %s: %s", LIB_INIT_CHUNK, LIB_FILE, dlerror());
	return 0;
}


int test_streebog()
{
	WORD hash[WORDS_IN_CHUNK];
	WORD gage[WORDS_IN_CHUNK];
	//char msg[] = "012345678901234567890123456789012345678901234567890123456789012";
	//char gage_str[] = "486f64c1917879417fef082b3381a4e211c324f074654c38823a7b76f830ad00fa1fbae42b1285c0352f227524bc9ab16254288dd6863dccd5b9f54a1ad0541b";
	/* "Се ветри, Стрибожи внуци, веютъ с моря стрелами на храбрыя плъкы Игоревы" */
	char msg[] = {0xd1, 0xe5, 0x20, 0xe2, 0xe5, 0xf2, 0xf0, 0xe8, 0x2c, 0x20, 0xd1, 0xf2, 0xf0, 0xe8, 0xe1, 0xee, 0xe6, 0xe8, 0x20, 0xe2, 0xed, 0xf3, 0xf6, 0xe8, 0x2c, 0x20, 0xe2, 0xe5, 0xfe, 0xf2, 0xfa, 0x20, 0xf1, 0x20, 0xec, 0xee, 0xf0, 0xff, 0x20, 0xf1, 0xf2, 0xf0, 0xe5, 0xeb, 0xe0, 0xec, 0xe8, 0x20, 0xed, 0xe0, 0x20, 0xf5, 0xf0, 0xe0, 0xe1, 0xf0, 0xfb, 0xff, 0x20, 0xef, 0xeb, 0xfa, 0xea, 0xfb, 0x20, 0xc8, 0xe3, 0xee, 0xf0, 0xe5, 0xe2, 0xfb, 0x00};
	char gage_str[] = "28fbc9bada033b1460642bdcddb90c3fb3e56c497ccd0f62b8a2ad4935e85f037613966de4ee00531ae60f3b5a47f8dae06915d5f2f194996fcabf2622e6881e";

	
	lib_init_chunk(gage, gage_str);
	
	FUNC_STREEBOG streebog = dlsym(lib, LIB_STREEBOG);
	ASSERT(streebog != NULL, "Did not find %s function in the library %s: %s", LIB_STREEBOG, LIB_FILE, dlerror());
	
	streebog(hash, msg, strlen(msg)*8, true);
	
	lib_print(print_chunk, (unsigned char *)hash, CHUNK_BITS);
	
	ASSERT(memcmp(hash, gage, CHUNK_SIZE) == 0, "Assert fail.\n%s", print_chunk);
	return 0;
}

int test_cases(const char *func_name)
{
	FUNC_B lib_function = dlsym(lib, func_name);
	ASSERT(lib_function != NULL, "Did not find %s function in the library %s: %s", func_name, LIB_FILE, dlerror());
	DEBUG("found lib function %s.", func_name);
	
	for(int n = 1;;++n)
	{
		int ret = test_case_b(lib_function, func_name, n);
		if(ret == 1) break; /* Next test file not found. Finish testing. */
		++tests_run;
		printf("%u. Test %s %d: ", tests_run, func_name, n);
		if(ret != 0)
		{
			++tests_errors;
			printf("fail.\n");
		}
		else printf("OK.\n");
	}
	return 0;
}

/**
 * Compares arrays.
 */
int test_case_b(FUNC_B lib_function, const char *func_name, unsigned int test_n)
{
	int ret = 0;
	bool is_b = true;
	
	WORD a[WORDS_IN_CHUNK];
	WORD b[WORDS_IN_CHUNK];
	WORD N[WORDS_IN_CHUNK];
	WORD gage[WORDS_IN_CHUNK];
	
	/* Loading data for testing. {FUNC}_a_{N}.bin and (or) {FUNC}_b_{N}.bin */
	ret = read_file(a, func_name, "a", test_n);
	if(ret != 0) return ret;
	ret = read_file(b, func_name, "b", test_n);
	if(ret != 0)
	{
		/* Optional arg */
		if(ret == -1) return ret;
		is_b = false;
	}
	
	/* Loading data for expected result. {FUNC}_gage_{N}.bin */
	ret = read_file(gage, func_name, "gage", test_n);
	if(ret != 0) return ret;
	
	lib_print(print_chunk, (unsigned char *)a, CHUNK_BITS);
	int sz = sprintf(print_src, "Data a:\n%s", print_chunk);
	if(is_b)
	{
		lib_print(print_chunk, (unsigned char *)b, CHUNK_BITS);
		sz += sprintf(print_src+sz, "Data b:\n%s", print_chunk);
	}
	lib_print(print_chunk, (unsigned char *)gage, CHUNK_BITS);
	sz += sprintf(print_src+sz, "Gage:\n%s", print_chunk);
	lib_chunk_set(N, 0);
	
	/* */
	lib_function(a, b);
	/* */
	
	lib_print(print_chunk, (unsigned char *)a, CHUNK_BITS);
	sz += sprintf(print_src+sz, "Result:\n%s", print_chunk);
	
	ASSERT(memcmp(a, gage, CHUNK_SIZE) == 0, "Assert fail.\n%s", print_src);
	DEBUG("Assert %s %u OK!", func_name, test_n);
	return 0;
}

int read_file(WORD *dst, const char *func_name, const char *suffix, unsigned int test_n)
{
	char filename[64];
	FILE *f = NULL;
	size_t read_sz = 0;
	int ret = 0;
	
	sprintf(filename, "%s%s_%s_%u.bin", CASES_DIR, func_name, suffix, test_n);
	if((f = fopen(filename, "rb")) == NULL)
	{
		DEBUG("File %s not found.", filename);
		return 1;
	}
	DEBUG("Reading case: %s", filename);
	read_sz = fread(dst, sizeof(WORD), WORDS_IN_CHUNK, f);
	if((ferror(f) != 0) || (read_sz != WORDS_IN_CHUNK))
	{
		LOG_ERR("Error reading %s. Read %lu words (%lu bytes).", filename, read_sz, read_sz * sizeof(WORD));
		ret = -1;
	}
	else DEBUG("OK");
	fclose(f);
	return ret;
}

int test_offcut()
{
	/*
	uint64_t length = 22;
	*/
	
	
	return 0;
}

int test_failures()
{

	return 0;
}

int all_tests()
{
	RUN_TEST(test_dlopen);
	RUN_TEST(test_sprint);
	RUN_TEST(test_chunk_set);
	RUN_TEST(test_init_chunk);
	/*
	RUN_TEST(test_offcut);
	
	test_cases(LIB_ADD512);
	
	
	test_cases(LIB_GN);
	
	test_cases(LIB_LPSX);
	test_cases(FUNC_XOR);
	test_cases(FUNC_S);
	test_cases(FUNC_P);
	test_cases(FUNC_L);
	*/
	
	RUN_TEST(test_streebog);
	/*
	int ret = test_streebog();
	if(ret != 0) return ret;
	*/
	
	RUN_TEST(test_failures);
	RUN_TEST(test_dlclose);
	return 0;
}

int main(int argc, char *argv[])
{
	argc = 1;
	DEBUG("%u-bit Mode. CHUNK: %ux%u", ARCH_MODE, ARCH_MODE / 8, WORDS_IN_CHUNK);
	DEBUG("\n----- RUNNING: %s", argv[0]);
	printf("----\nRUNNING: %s\n", argv[0]);
	
	all_tests();
	if(tests_errors != 0) printf("FAILED\n");
	else printf("ALL TESTS PASSED\n");
    printf("Tests run: %u. Tests errors: %u.\n", tests_run, tests_errors);
    return tests_errors;
}
