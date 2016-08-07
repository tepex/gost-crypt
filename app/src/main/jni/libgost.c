/**
 * Core functions.
 * Created on: Jun 24, 2016
 *
 * @author Tepex <tepex@mail.ru>
 */

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "inc/const.h"
#include "inc/libgost.h"

WORD C[12][WORDS_IN_CHUNK];
char tmp[1024];

/**
 * Implementation of hash function «ГОСТ Р 34.11—2012».
 * RFC: 6986 https://tools.ietf.org/html/rfc6986
 * GOST: http://protect.gost.ru/document.aspx?control=7&id=180209
 * 
 * @param hash Output buffer for 256/512-bit hash.
 * @param msg Entire message.
 * @param length Message length in bits.
 * @param is_512 true for 512-bit hash, false for 256-bit hash.
 * @return 0 — on success, 1 — on error.
 */
int streebog(WORD *hash, const unsigned char *msg, uint64_t length, bool is_512)
{
	WORD h[WORDS_IN_CHUNK];
	WORD N[WORDS_IN_CHUNK];
	WORD uint512[WORDS_IN_CHUNK]; /* 512-bit unsigned integer. Little endian. */
	WORD Sigma[WORDS_IN_CHUNK];
	WORD padding[WORDS_IN_CHUNK];
	WORD zero[WORDS_IN_CHUNK];
	WORD *m;
	uint64_t len = length;
	
	for(int i = 0; i < 12; ++i) init_chunk(C[i], C_STR[i]);
	
	chunk_set(N, 0);
	chunk_set(Sigma, 0);
	chunk_set(zero, 0);
	set_uint512(uint512, 512);
	
	m = (WORD *)msg;
	if(is_512) chunk_set(h, 0);
	else chunk_set(h, WADDING);
	
	/* stage 2 */
	while(len >= CHUNK_BITS)
	{
		gN(h, m, N);
		
		add512(N, uint512);
		add512(Sigma, m);
		len -= CHUNK_BITS;
		m += WORDS_IN_CHUNK;
	}
	/* stage 3 */
	/* 3.1. */
	pad(padding, msg + length / CHUNK_BITS * CHUNK_SIZE, len);
	/* 3.2. */	
	gN(h, padding, N);
	
	/* 3.3. */
	set_uint512(uint512, len);
	add512(N, uint512);
	/* 3.4. */
	add512(Sigma, padding);
	/* 3.5. */
	gN(h, N, zero);
	/* 3.6. */
	gN(h, Sigma, zero);
	/* 3.7. */
	chunk_cpy(hash, h, is_512);
	return 0;
}

void gN(WORD *h, const WORD *m, const WORD *N)
{
	WORD K[WORDS_IN_CHUNK];
	WORD data[WORDS_IN_CHUNK];
	WORD i;
	
	/* K = K1 = LPS(h ^ N)  */
	chunk_cpy(data, h, true);
	LPSX(data, N);
	chunk_cpy(K, data, true);
	LPSX(data, m);
	/* E(K, m) */
	for(i = 0; i < 12; ++i)
	{
		LPSX(K, C[i]);
		if(i == 11) break;
		LPSX(data, K);
	}
	//LPSX(K, C[11]);
	XOR(data, K);
	XOR(data, h);
	XOR(data, m);
	chunk_cpy(h, data, true);
}

inline void XOR(WORD *dst, const WORD *chunk)
{
	for(WORD i = 0; i < WORDS_IN_CHUNK; ++i) dst[i] ^= chunk[i];
}

inline void LPSX(WORD *dst, const WORD *chunk)
{
	XOR(dst, chunk);
	S(dst);
	P(dst);
	L(dst);
}

inline void S(WORD *chunk)
{
	unsigned char *ptr = (unsigned char *)chunk;
	
	for(unsigned char i = 0; i < CHUNK_SIZE; ++i) ptr[i] = PI[ptr[i]];
}

inline void P(WORD *chunk)
{
	unsigned char result[CHUNK_SIZE];
	unsigned char *ptr = (unsigned char *)chunk;
	
	for(WORD i = 0; i < CHUNK_SIZE; ++i) result[i] = ptr[TAU[i]];
	chunk_cpy(chunk, (WORD *)result, true);
}

inline void L(WORD *chunk)
{
	WORD *a_ptr;
	WORD i, j, k, l, word, test, ri;
	WORD result[WORDS_IN_CHUNK];
	
	
	chunk_set(result, 0);
	for(i = 0; i < 8; ++i) /* base vectors */ 
	{
		a_ptr = (WORD *)A;

		/* Test every bit in base vector and if set, do: step = A[] XOR previous_step */
		for(j = WORDS_IN_VECTOR; j > 0; --j)
		{
			word = chunk[WORDS_IN_VECTOR * i + j - 1];
			
			for(k = 0; k < ARCH_MODE; ++k) /* bits in word */ 
			{
				test = word & (1ULL << (ARCH_MODE - 1 - k));
				if(test)
				{
					for(l = 0; l < WORDS_IN_VECTOR; ++l)
					{
						ri = i * WORDS_IN_VECTOR + l;
						result[ri] ^= *a_ptr;
						++a_ptr;
					}
				}
				else a_ptr += WORDS_IN_VECTOR;
			}
		}
	}
	
	chunk_cpy(chunk, result, true);
}

/* Выравнивание до границы 512. Заполняется 000...01 
   offcut - 
   len - длина остатка
*/

inline void pad(WORD *padding, const unsigned char *offcut, unsigned int len)
{
	unsigned char i, shift, mask1 = 0xff, mask2 = 0x80;
	unsigned char *ptr = (unsigned char *)padding;
	
	chunk_set(padding, 0);
	i = len / 8;
	memcpy(padding, offcut, i + 1 - IS_MULT8(len));
	
	shift = 8 - (len % 8); /* last bit index */
	
	mask1 >>= shift;
	mask2 >>= (shift - 1);
	
	ptr[i] = (ptr[i] & mask1) | mask2;
}
