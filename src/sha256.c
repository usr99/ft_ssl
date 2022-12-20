/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 00:14:01 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/19 21:52:30 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define BLOCK_SIZE			64 // 512 bits
#define SHA256_DIGEST_SIZE	32 // 256 bits

static uint32_t rotate_right(uint32_t value, uint8_t shift)
{
	return (value >> shift) | (value << (32 - shift));
}

static void ft_memrcpy(void* dest, void* src, size_t n)
{
	uint8_t* d = dest;
	uint8_t* s = src;
	size_t i;
	
	for (i = 0; i < n; i++)
		d[i] = s[n - i - 1];
}

t_hash* sha256(const char* src, size_t length)
{
	static const uint32_t K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};

	uint64_t bitlen = length * 8;
	uint64_t pad = bitlen + 1 + 64;
	pad = ALIGN(pad, 512);
	uint64_t size = pad / 8;

	uint8_t* message = malloc(size);
	if (!message)
		return NULL;

	/* Padding */
	ft_memcpy(message, src, length);											// copy original message
	message[length] = (1 << 7);													// append 1 bit
	ft_memset(message + length + 1, 0, size - length - 1 - sizeof(uint64_t));	// append 0 bits until size is a multiple of (512 - 64) = 448
	ft_memrcpy(message + size - sizeof(uint64_t), &bitlen, sizeof(uint64_t));	// append the initial message length to reach 512 bits (in big endian)

	/* Initialization */
	uint64_t i;
	uint32_t digest[] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
	for (i = 0; i < size; i += BLOCK_SIZE)
	{
		uint32_t* words = (uint32_t*)(message + i);
		uint32_t W[64] = {0};
		int j;

		for (j = 0; j < 16; j++)
			ft_memrcpy(W + j, words + j, sizeof(uint32_t));

		for ( ; j < 64; j++)
		{
			uint32_t s0 = rotate_right(W[j - 15], 7) ^ rotate_right(W[j - 15], 18) ^ (W[j - 15] >> 3);
			uint32_t s1 = rotate_right(W[j - 2], 17) ^ rotate_right(W[j - 2], 19) ^ (W[j - 2] >> 10);
			W[j] = W[j - 16] + s0 + W[j - 7] + s1;
		}
		
		uint32_t a = digest[0];
		uint32_t b = digest[1];
		uint32_t c = digest[2];
		uint32_t d = digest[3];
		uint32_t e = digest[4];
		uint32_t f = digest[5];
		uint32_t g = digest[6];
		uint32_t h = digest[7];

		for (j = 0; j < 64; j++)
		{
			uint32_t S1 = rotate_right(e, 6) ^ rotate_right(e, 11) ^ rotate_right(e, 25);			
			uint32_t ch = (e & f) ^ (~e & g);
			uint32_t tmp1 = h + S1 + ch + K[j] + W[j];
			uint32_t S0 = rotate_right(a, 2) ^ rotate_right(a, 13) ^ rotate_right(a, 22);
			uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
			uint32_t tmp2 = S0 + maj;

			h = g;
			g = f;
			f = e;
			e = d + tmp1;
			d = c;
			c = b;
			b = a;
			a = tmp1 + tmp2;
		}

		digest[0] += a;
		digest[1] += b;
		digest[2] += c;
		digest[3] += d;
		digest[4] += e;
		digest[5] += f;
		digest[6] += g;
		digest[7] += h;
	}

	free(message);
	t_hash* result = malloc(sizeof(t_hash));
	if (!result)
		return NULL;

	result->digest = malloc(SHA256_DIGEST_SIZE);
	if (!result->digest)
	{
		free(result);
		return NULL;
	}

	for (i = 0; i < 8; i++)
		ft_memrcpy(result->digest + i * sizeof(uint32_t), digest + i, sizeof(uint32_t));

	result->size = SHA256_DIGEST_SIZE;
	return result;
}
