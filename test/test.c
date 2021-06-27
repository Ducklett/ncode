#ifndef TEST_ENTRYPOINT
#define TEST_ENTRYPOINT test_main
#endif

#include "test.h"
#include "base64.test.c"
#include "leb128.test.c"

void test_main()
{
	test_leb128();
	test_base64();
}
