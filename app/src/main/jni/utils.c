/**
 * Utils functions.
 * Created on: Jun 24, 2016
 *
 * @author Tepex <tepex@mail.ru>
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "inc/libgost.h"

/**
 * Writes WORDS_IN_CHUNK words of value val to chunk.
 *
 * @param chunk Destination array.
 * @param val Value for the filling.
 * @return 0 — on success, -1 — on invalid memory pointer. 
*/
inline int chunk_set(WORD *chunk, const WORD val)
{
	for(WORD i = 0; i < WORDS_IN_CHUNK; ++i) chunk[i] = val;
	return 0;
}

/**
 * Copies WORDS_IN_CHUNK words from src to chunk.
 *
 * @param chunk Destination chunk.
 * @param src Source chunk.
 * @param is_512 true — 512-bit mode, false — 256-bit mode.
 * @return 0 — on success, -1 — on invalid memory pointer. 
*/
inline int chunk_cpy(WORD *chunk, const WORD *src, bool is_512)
{
	unsigned char size = WORDS_IN_CHUNK;
	
	if(!is_512) size /= 2;
	for(WORD i = 0; i < size; ++i)
	{
		if(is_512) chunk[i] = src[i];
		else chunk[i] = src[i + size];
	}
	return 0;
}

/**
 * Создает бинарный чанк (64-байтный кусок памяти) из текстовой строки в 16-ричном виде.
 */
int init_chunk(WORD *chunk, const char *str)
{
	char digit[2];
	char *c_ptr = (char *)chunk;
	int j = 0;
	
	for(int i = 0; i < 128; ++i)
	{
		digit[1] = str[127-i];
		digit[0] = str[126-i];
		if(i % 2 == 0) c_ptr[j++] = (char)strtol(digit, NULL, 16);
	}
	return 0;
}

/**
 * Writes 512-bit unsigned integer value in big-endian form.
 *
 * @param chunk Destination chunk.
 * @param val Value to write (0 — 511).
 * @return 0 — on success, -1 — on invalid memory pointer. 
*/
int set_uint512(WORD *chunk, uint16_t val)
{
	unsigned char *ptr = (unsigned char *)chunk;
	if(chunk_set(chunk, 0) != 0) return -1;
	ptr[0] = val & 0xff;
	ptr[1] = val >> 8;
	return 0;
}

/**
 * Addition modulo 512. Writes unsigned integer value in big-endian form.
 *
 * @param chunk Destination chunk.
 * @param val Value to add.
 * @return 0 — on success, -1 — on invalid memory pointer. 
*/
int add512(WORD *chunk, const WORD *val)
{
	WORD i = 0, prev, cf, old_cf = 0; /* carry flag */
	
	for(i = 0; i < WORDS_IN_CHUNK; ++i)
	{
		prev = chunk[i];
		chunk[i] += val[i];
		cf = (chunk[i] < val[i] || chunk[i] < prev);
		if(old_cf) ++chunk[i];
		old_cf = cf;
	}
	return 0;
}

/**
 * Outputs hex dump. Output is terminated by "\n\0".
 * 
 * @param dst Destination string.
 * @param array Array to print out.
 * @param length Number of bits to print.
 * @return String size (with \n\0) or -1 if error
 */
int sprint_hex_array(char *dst, const unsigned char *array, uint64_t length)
{
	const char header[] = "   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n   _______________________________________________\n";
	char *start = dst;
	char tmp[5];
	                 
	unsigned int size = length / 8 + !IS_MULT8(length);
	if(size >= CHUNK_SIZE * 4) size = CHUNK_SIZE * 4;
	
	char hexstr[size*2+2];
	
	dst = stpcpy(dst, header);
	for(unsigned int i = 0; i < size; ++i)
	{
		if(i % 16 == 0)
		{
			sprintf(tmp, "\n%u |", (i / 16));
			dst = stpcpy(dst, tmp);
		}
		sprintf(tmp, "%02x ", array[i]);
		dst = stpcpy(dst, tmp);
		hexstr[size*2-i*2] = tmp[1];
		hexstr[size*2-i*2-1] = tmp[0];
	}
	hexstr[0] = '\n';
	hexstr[size*2+1] = 0;
	sprintf(dst, "%s", hexstr);
	return (dst - start + 2);
}

/**
 * Prints hex string of array. Output is terminated by "\n\0".
 * 
 * @param dst Destination string.
 * @param chunk Chunk to print out.
 * @param is_512 true — 512-bit mode, false — 256-bit mode.
 * @param is_le true — prints in little-endian form. false — prints in big-endian form.
 * @return String size (with \n\0) or -1 if error
 */
int to_string(char *dst, const WORD *chunk, bool is_512, bool is_le)
{
	unsigned char i = 0, size = CHUNK_SIZE;
	unsigned char *ptr = (unsigned char *)chunk;
	int ret = 0;
	
	if(!is_512) size /= 2;
	for(; i < size; ++i)
	{
		if(is_le) i = size - i - 1;
		ret += sprintf(dst+ret, "%02x", ptr[i]);
	}
	return ret;
}

void reverse_bytes(unsigned char *reverse, const uint64_t val)
{
	unsigned char *bytes;
	void *ptr;
	
	ptr = (void *)&val;
	bytes = (unsigned char *)ptr;
	
	for(int i = 0; i < 8; ++i)
	{
		reverse[7-i] = bytes[i];
	}
	for(int i = 0; i < 8; ++i)
	{
		printf("%02x ", reverse[i]);
	}
}

void reverse_chunk(WORD *chunk)
{
	WORD reverse[WORDS_IN_CHUNK];
	unsigned char *chunk_ptr = (unsigned char *)chunk;
	unsigned char *reverse_ptr = (unsigned char *)reverse;
	
	chunk_cpy(reverse, chunk, true);
	for(WORD i = 0; i < CHUNK_SIZE; ++i) chunk_ptr[CHUNK_SIZE-i-1] = reverse_ptr[i];
}