#pragma once

#include <iostream>
#include <cassert>
#include "../helpers/bytes.h"

typedef Bytes::ByteString(*HmacFunc)(const Bytes::ByteString&, const Bytes::ByteString&);

inline uint32_t lrot32(uint32_t num, uint8_t rotcount)
{
	return (num << rotcount) | (num >> (32 - rotcount));
}

inline Bytes::ByteString sha1(const Bytes::ByteString& msg)
{
	const size_t size_bytes = msg.size();
	const uint64_t size_bits = size_bytes * 8;
	Bytes::ByteString bstr = msg;
	Bytes::ByteStringDestructor asplode(&bstr);

	bstr.push_back(0x80);
	while (bstr.size() % (512 / 8) != (448 / 8))
	{
		bstr.push_back(0x00);
	}

	bstr.append(Bytes::u64beToByteString(size_bits));

	assert(bstr.size() % (512 / 8) == 0);

	uint32_t h0 = 0x67452301;
	uint32_t h1 = 0xEFCDAB89;
	uint32_t h2 = 0x98BADCFE;
	uint32_t h3 = 0x10325476;
	uint32_t h4 = 0xC3D2E1F0;

	for (size_t i = 0; i < bstr.size() / 64; ++i)
	{
		Bytes::ByteString chunk(bstr.begin() + i * 64, bstr.begin() + (i + 1) * 64);
		Bytes::ByteStringDestructor xplode(&chunk);

		uint32_t words[80];
		size_t j;

		for (j = 0; j < 16; ++j)
		{
			words[j] =
				(chunk[4 * j + 0] << 24) |
				(chunk[4 * j + 1] << 16) |
				(chunk[4 * j + 2] << 8) |
				(chunk[4 * j + 3] << 0)
				;
		}

		for (j = 16; j < 32; ++j)
		{
			words[j] = lrot32(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1);
		}
		for (j = 32; j < 80; ++j)
		{
			words[j] = lrot32(words[j - 6] ^ words[j - 16] ^ words[j - 28] ^ words[j - 32], 2);
		}

		uint32_t a = h0;
		uint32_t b = h1;
		uint32_t c = h2;
		uint32_t d = h3;
		uint32_t e = h4;

		for (j = 0; j < 80; ++j)
		{
			uint32_t f = 0, k = 0;

			if (j < 20)
			{
				f = (b & c) | ((~b) & d);
				k = 0x5A827999;
			}
			else if (j < 40)
			{
				f = b ^ c ^ d;
				k = 0x6ED9EBA1;
			}
			else if (j < 60)
			{
				f = (b & c) | (b & d) | (c & d);
				k = 0x8F1BBCDC;
			}
			else if (j < 80)
			{
				f = b ^ c ^ d;
				k = 0xCA62C1D6;
			}
			else
			{
				assert(0 && "Something went wrong");
			}

			uint32_t tmp = lrot32(a, 5) + f + e + k + words[j];
			e = d;
			d = c;
			c = lrot32(b, 30);
			b = a;
			a = tmp;
		}

		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
	}

	Bytes::ByteString first = Bytes::u32beToByteString(h0);
	Bytes::ByteStringDestructor x1(&first);
	Bytes::ByteString second = Bytes::u32beToByteString(h1);
	Bytes::ByteStringDestructor x2(&second);
	Bytes::ByteString third = Bytes::u32beToByteString(h2);
	Bytes::ByteStringDestructor x3(&third);
	Bytes::ByteString fourth = Bytes::u32beToByteString(h3);
	Bytes::ByteStringDestructor x4(&fourth);
	Bytes::ByteString fifth = Bytes::u32beToByteString(h4);
	Bytes::ByteStringDestructor x5(&fifth);
	return first + second + third + fourth + fifth;
}

inline Bytes::ByteString hmacSha1(const Bytes::ByteString& key, const Bytes::ByteString& msg, size_t blockSize = 64)
{
	Bytes::ByteString realKey = key;
	Bytes::ByteStringDestructor asplode(&realKey);

	if (realKey.size() > blockSize)
	{
		Bytes::ByteString newRealKey = sha1(realKey);
		Bytes::swizzleByteStrings(&realKey, &newRealKey);
	}
	if (realKey.size() < blockSize)
	{
		realKey.resize(blockSize, 0x00);
	}

	Bytes::ByteString innerPadKey = realKey;
	Bytes::ByteStringDestructor xplodeI(&innerPadKey);
	Bytes::ByteString outerPadKey = realKey;
	Bytes::ByteStringDestructor xplodeO(&outerPadKey);

	for (size_t i = 0; i < realKey.size(); ++i)
	{
		innerPadKey[i] = innerPadKey[i] ^ 0x36;
		outerPadKey[i] = outerPadKey[i] ^ 0x5c;
	}

	Bytes::ByteString innerMsg = innerPadKey + msg;
	Bytes::ByteStringDestructor xplodeIM(&innerMsg);
	Bytes::ByteString innerHash = sha1(innerMsg);
	Bytes::ByteStringDestructor xplodeIH(&innerHash);
	Bytes::ByteString outerMsg = outerPadKey + innerHash;
	Bytes::ByteStringDestructor xplodeOM(&outerMsg);

	return sha1(outerMsg);
}