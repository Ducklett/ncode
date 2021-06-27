#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t u8;

const char base64LUT[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";
const char base64Pad = '=';

const u8 BASE64_ILLEGAL = 254;

// turns base64 character {c} into its binary value
// returns {BASE64_ILLEGAL} if the character is not valid
u8 base64ValueFromCharacter(char c)
{
	if (c >= 'A' && c <= 'Z') {
		return c - 'A';
	} else if (c >= 'a' && c <= 'z') {
		return (c - 'a') + 26;
	} else if (c >= '0' && c <= '9') {
		return (c - '0') + 52;
	} else if (c == '+') {
		return 62;
	} else if (c == '/') {
		return 63;
	} else {
		return BASE64_ILLEGAL;
	}
}

char *base64Encode(u8 *bytes, int len, int *outputLength)
{
	int bufferSize = 0;
	for (int i = 0; i < len; i += 3)
		bufferSize += 4;

	char *buffer = malloc(sizeof(char) * bufferSize);
	int bufferIndex = 0;

	int byte = 0;
	int carry = 0;

	const int FIRST6 = 0xFC;
	const int LAST2 = 0x03;
	const int FIRST4 = 0xF0;
	const int LAST4 = 0x0F;
	const int FIRST2 = 0xC0;
	const int LAST6 = 0x3F;
	for (int i = 0; i < len; i++) {
		switch (byte) {
		case 0: {
			buffer[bufferIndex++] = base64LUT[(bytes[i] & FIRST6) >> 2];
			carry = (bytes[i] & LAST2) << 4;
		} break;
		case 1: {
			buffer[bufferIndex++] = base64LUT[((bytes[i] & FIRST4) >> 4) + carry];
			carry = (bytes[i] & LAST4) << 2;
		} break;
		case 2: {
			buffer[bufferIndex++] = base64LUT[((bytes[i] & FIRST2) >> 6) + carry];
			buffer[bufferIndex++] = base64LUT[(bytes[i] & LAST6)];
		} break;
		}

		byte = ((byte + 1) % 3);
	}

	bool carryLeftOver = byte != 0;
	if (carryLeftOver) {
		buffer[bufferIndex++] = base64LUT[carry];
	}

	while (bufferIndex % 4 != 0) {
		buffer[bufferIndex++] = base64Pad;
	}

	*outputLength = bufferSize;
	return buffer;
}

// zeroes out padding and decrements the length of the returned string
static u8 base64Unpad(u8 value, int *len)
{
	if (value == BASE64_ILLEGAL) {
		(*len)--;
		return 0;
	} else {
		return value;
	}
}

u8 *base64Decode(char *blob, int len, int *outputLength)
{
	assert(len % 4 == 0 && "Base64 blob should have a length multiple of 4");

	int outLen = len / 4 * 3;

	u8 *buffer = malloc(sizeof(char) * outLen);
	int bufferIndex = 0;

	const u8 FIRST2 = 0x30;
	const int FIRST4 = 0x3c;
	const u8 LAST4 = 0x0F;
	const int LAST2 = 0x03;
	for (int i = 0; i < len; i += 4) {
		u8 byte1 = base64Unpad(base64ValueFromCharacter(blob[i + 0]), &outLen);
		u8 byte2 = base64Unpad(base64ValueFromCharacter(blob[i + 1]), &outLen);
		u8 byte3 = base64Unpad(base64ValueFromCharacter(blob[i + 2]), &outLen);
		u8 byte4 = base64Unpad(base64ValueFromCharacter(blob[i + 3]), &outLen);
		buffer[bufferIndex++] = (byte1 << 2) + ((byte2 & FIRST2) >> 4);
		buffer[bufferIndex++] = ((byte2 & LAST4) << 4) + ((byte3 & FIRST4) >> 2);
		buffer[bufferIndex++] = ((byte3 & LAST2) << 6) + (byte4);
	}

	*outputLength = outLen;
	return buffer;
}
