/**
 * Base constants and core functions prototypes.
 * Created on: Jun 24, 2016
 *
 * @author Tepex <tepex@mail.ru>
 */

#ifndef __LIBGOST_H__
#define __LIBGOST_H__

#include <stdint.h>
#include <stdbool.h>

/* Set architecture mode bits. 8-bit, 16-bit, 32-bit, 64-bit */
#ifndef ARCH_MODE
	#define ARCH_MODE 32
#endif

#if ARCH_MODE == 8
	#define WORD uint8_t
	#define WADDING 0x01
#elif ARCH_MODE == 16
	#define WORD uint16_t
	#define WADDING 0x0101
#elif ARCH_MODE == 32
	#define WORD uint32_t
	#define WADDING 0x01010101
#else
	#define WORD uint64_t
	#define WADDING 0x0101010101010101
#endif

#define CHUNK_SIZE 64 /* Chunk size in bytes */
#define CHUNK_BITS 512 /* Chunk size in bits */
#define WORDS_IN_CHUNK (CHUNK_BITS / ARCH_MODE)
#define WORDS_IN_VECTOR (CHUNK_SIZE / ARCH_MODE) /* For L function. Number of words in 64-bit vector. */

/* Check multiplicities by 8 */
#define IS_MULT8(N) ((N & 7) == 0)

int streebog(WORD *hash, const unsigned char *msg, uint64_t length, bool is_512);
void gN(WORD *hash, const WORD *m, const WORD *N);
void XOR(WORD *dst, const WORD *chunk);
void LPSX(WORD *dst, const WORD *chunk);
void S(WORD *chunk);
void P(WORD *chunk);
void L(WORD *chunk);
void L1(WORD *chunk);
void L2(WORD *chunk);
void pad(WORD *padding, const unsigned char *offcut, unsigned int len);

int chunk_set(WORD *chunk, const WORD val);
int chunk_cpy(WORD *chunk, const WORD *src, bool is_512);
int init_chunk(WORD *chunk, const char *str);
int set_uint512(WORD *chunk, uint16_t val);
int add512(WORD *chunk, const WORD *val);
int sprint_hex_array(char *dst, const unsigned char *array, uint64_t length);
int to_string(char *dst, const WORD *chunk, bool is_512, bool is_le);
void reverse_bytes(unsigned char *reverse, const uint64_t val);
void reverse_chunk(WORD *chunk);

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#endif /* __LIBGOST_H__ */
