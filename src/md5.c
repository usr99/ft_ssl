/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 20:48:31 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/18 19:31:29 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "ft_ssl.h"

#define BLOCK_SIZE		64 // 512 bits
#define MD5_DIGEST_SIZE	16 // 128 bits

static uint32_t rotate_left(uint32_t value, uint8_t shift)
{
	return (value << shift) | (value >> (32 - shift));
}

t_hash* md5(const char* src)
{
	static const uint8_t S[64] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};

	static const uint32_t K[64] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	uint64_t length = ft_strlen(src);
	uint64_t size = (length / BLOCK_SIZE + 1) * BLOCK_SIZE;
	uint8_t* message = malloc(size);
	if (!message)
		return NULL;

	/* Padding */
	ft_memcpy(message, src, length);											// copy original message
	message[length] = (1 << 7);													// append 1 bit
	ft_memset(message + length + 1, 0, size - length - 1 - sizeof(uint64_t));	// append 0 bits until size is a multiple of (512 - 64) = 448
	length *= 8;
	ft_memcpy(message + size - sizeof(uint64_t), &length, sizeof(uint64_t));	// append the initial message length to reach 512 bits

	/* Initialization */
	uint64_t i;
	uint32_t digest[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
	for (i = 0; i < size; i += BLOCK_SIZE)
	{
		uint32_t* words = (uint32_t*)(message + i);
		uint32_t a = digest[0];
		uint32_t b = digest[1];
		uint32_t c = digest[2];
		uint32_t d = digest[3];

		int j;
		for (j = 0; j < BLOCK_SIZE; j++)
		{
			int f, g;

			if (j < 16)
			{
				f = (b & c) | (~b & d);
				g = j;
			}
			else if (j < 32)
			{
				f = (d & b) | (~d & c);
				g = (5 * j + 1) % 16;
			}
			else if (j < 48)
			{
				f = b ^ c ^ d;
				g = (3 * j + 5) % 16;
			}
			else
			{
				f = c ^ (b | ~d);
				g = (7 * j) % 16;
			}

			f = f + a + K[j] + words[g];
			a = d;
			d = c;
			c = b;
			b = b + rotate_left(f, S[j]);
		}

		digest[0] += a;
		digest[1] += b;
		digest[2] += c;
		digest[3] += d;
	}
	free(message);

	t_hash* result = malloc(sizeof(t_hash));
	if (!result)
		return NULL;

	result->digest = malloc(MD5_DIGEST_SIZE);
	if (!result->digest)
	{
		free(result);
		return NULL;
	}

	ft_memcpy(result->digest, digest, MD5_DIGEST_SIZE);
	result->size = MD5_DIGEST_SIZE;
	return result;
}
