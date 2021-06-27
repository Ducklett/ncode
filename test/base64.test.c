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

test_base64_decoding(Str expected, Str buffer)
{
	int outLen;
	char *textOut = base64Decode(buffer.buf, buffer.len, &outLen);
	bool match = outLen == expected.len && !strncmp(textOut, expected.buf, expected.len);

	char msg[512];
	sprintf(msg, "'%s' decodes correctly", buffer.buf);
	test(msg, match);
	if (!match)
		printf("\tExpected '%.*s', got '%.*s' %d %d\n", expected.len, expected.buf, outLen, textOut, expected.len,
			   outLen);
}

void test_base64_value_index_is_binary_value()
{
	int failedAt = -1;

	for (int i = 0; i < 64; i++) {
		char c = base64LUT[i];
		u8 value = base64ValueFromCharacter(c);
		if (value != i) {
			failedAt = i;
			break;
		}
	}

	test("Base64 value index is binary value", failedAt == -1);
	if (failedAt != -1) {
		printf("Failed at %c, expected %d but got %d\n", base64LUT[failedAt], failedAt,
			   base64ValueFromCharacter(base64LUT[failedAt]));
	}
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

	test_base64_value_index_is_binary_value();
	test_base64_decoding(STR(""), STR(""));
	test_base64_decoding(STR("AAA"), STR("QUFB"));
	test_base64_decoding(STR("h"), STR("aA=="));
	test_base64_decoding(STR("Hello world"), STR("SGVsbG8gd29ybGQ="));
	test_base64_decoding(STR("hello world"), STR("aGVsbG8gd29ybGQ="));
}
