#ifndef TEST_ENTRYPOINT
#define TEST_ENTRYPOINT test_leb128
#endif

#include "../src/base64.c"
#include "test.h"
#include <string.h>

typedef struct {
	char *buf;
	int len;
} Str;
#define STR(x) ((Str){x, sizeof(x) - 1})

test_base64_encoding(Str text, Str expected)
{
	int outLen;
	char *textOut = base64Encode(text.buf, text.len, &outLen);
	bool match = outLen == expected.len && !strncmp(textOut, expected.buf, expected.len);

	char msg[512];
	sprintf(msg, "'%s' encodes correctly", text.buf);
	test(msg, match);
	if (!match) printf("\tExpected %.*s, got %.*s\n", expected.len, expected.buf, outLen, textOut);
}

void test_base64()
{
	test_section("Base64 encoding");

	test_equal("Base64 lookup table should be 64 characters long", sizeof(base64LUT) - 1, 64);
	test_base64_encoding(STR(""), STR(""));
	test_base64_encoding(STR("AAA"), STR("QUFB"));
	test_base64_encoding(STR("h"), STR("aA=="));
	test_base64_encoding(STR("Hello world"), STR("SGVsbG8gd29ybGQ="));
	test_base64_encoding(STR("hello world"), STR("aGVsbG8gd29ybGQ="));

	test_section("Base64 decoding");
}
