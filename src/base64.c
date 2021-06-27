#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint8_t u8;

const char base64LUT[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";
const char base64Pad = '=';

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

u8 *base64Decode(char *blob, int len, int *outputLength)
{
	//
	return 0;
}
