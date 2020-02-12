/**
	@file
	@brief SHA1的实现

	这个文件里面是对SHA1的具体实现

	@see Utilities.Encryption.SHA1.h

	@author iriszero
	@date 2020/2/11
*/

#include "Utilities.Encryption.SHA1.h"

inline void BufToBlock(const char* buf, uint32_t block[16])
{
	for (size_t i = 0; i < 16; i++)
		block[i] =
		(buf[4 * i + 3] & 0xff) |
		(buf[4 * i + 2] & 0xff) << 8 |
		(buf[4 * i + 1] & 0xff) << 16 |
		(buf[4 * i + 0] & 0xff) << 24;
}

static uint32_t Rol(const uint32_t value, const size_t bits)
{
	return (value << bits) | (value >> (32 - bits));
}

static uint32_t Blk(const uint32_t block[16], const size_t i)
{
	return Rol(block[(i + 13) & 15] ^ block[(i + 8) & 15] ^ block[(i + 2) & 15] ^ block[i], 1);
}

inline void Swap(uint32_t& v, uint32_t& w, uint32_t& x, uint32_t& y, uint32_t& z)
{
	const auto temp = z;
	z = y;
	y = x;
	x = w;
	w = v;
	v = temp;
}

static void R0(const uint32_t block[16], uint32_t& v, uint32_t& w, uint32_t& x, uint32_t& y, uint32_t& z,
	const size_t i)
{
	z += ((w & (x ^ y)) ^ y) + block[i] + 0x5a827999 + Rol(v, 5);
	w = Rol(w, 30);
	Swap(v, w, x, y, z);
}

static void R1(uint32_t block[16], uint32_t& v, uint32_t& w, uint32_t& x, uint32_t& y, uint32_t& z, const size_t i)
{
	block[i] = Blk(block, i);
	z += ((w & (x ^ y)) ^ y) + block[i] + 0x5a827999 + Rol(v, 5);
	w = Rol(w, 30);
	Swap(v, w, x, y, z);
}

static void R2(uint32_t block[16], uint32_t& v, uint32_t& w, uint32_t& x, uint32_t& y, uint32_t& z, const size_t i)
{
	block[i] = Blk(block, i);
	z += (w ^ x ^ y) + block[i] + 0x6ed9eba1 + Rol(v, 5);
	w = Rol(w, 30);
	Swap(v, w, x, y, z);
}

static void R3(uint32_t block[16], uint32_t& v, uint32_t& w, uint32_t& x, uint32_t& y, uint32_t& z, const size_t i)
{
	block[i] = Blk(block, i);
	z += (((w | x) & y) | (w & x)) + block[i] + 0x8f1bbcdc + Rol(v, 5);
	w = Rol(w, 30);
	Swap(v, w, x, y, z);
}

static void R4(uint32_t block[16], uint32_t& v, uint32_t& w, uint32_t& x, uint32_t& y, uint32_t& z, const size_t i)
{
	block[i] = Blk(block, i);
	z += (w ^ x ^ y) + block[i] + 0xca62c1d6 + Rol(v, 5);
	w = Rol(w, 30);
	Swap(v, w, x, y, z);
}

static void Transform(uint32_t digest[], uint32_t* block, uint64_t& transforms)
{
	auto a = digest[0];
	auto b = digest[1];
	auto c = digest[2];
	auto d = digest[3];
	auto e = digest[4];
	for (auto i = 0; i < 16; ++i) R0(block, a, b, c, d, e, i);
	for (auto i = 0; i < 4; ++i) R1(block, a, b, c, d, e, i);
	for (auto i = 4; i < 16; ++i) R2(block, a, b, c, d, e, i);
	for (auto i = 0; i < 8; ++i) R2(block, a, b, c, d, e, i);
	for (auto i = 8; i < 16; ++i) R3(block, a, b, c, d, e, i);
	for (auto i = 0; i < 12; ++i) R3(block, a, b, c, d, e, i);
	for (auto i = 12; i < 16; ++i) R4(block, a, b, c, d, e, i);
	for (auto i = 0; i < 16; ++i) R4(block, a, b, c, d, e, i);
	digest[0] += a;
	digest[1] += b;
	digest[2] += c;
	digest[3] += d;
	digest[4] += e;
	transforms++;
}

namespace Utilities::Encryption
{
	SHA1::Core::Core()
	{
		Reset();
	}

	SHA1::Core::Core(const Core& core) : transforms(core.transforms), bufLength(core.bufLength)
	{
		memcpy(digest, core.digest, sizeof digest);
		memcpy(buf, core.buf, sizeof buf);
	}

	SHA1::Core::Core(Core&& core) noexcept : transforms(core.transforms), bufLength(core.bufLength)
	{
		memcpy(digest, core.digest, sizeof digest);
		memcpy(buf, core.buf, sizeof buf);
		core.Reset();
	}

	SHA1::Core& SHA1::Core::operator=(const Core& core)
	{
		transforms = core.transforms;
		bufLength = core.bufLength;
		memcpy(digest, core.digest, sizeof digest);
		memcpy(buf, core.buf, sizeof buf);
		return *this;
	}

	SHA1::Core& SHA1::Core::operator=(Core&& core) noexcept
	{
		transforms = core.transforms;
		bufLength = core.bufLength;
		memcpy(digest, core.digest, sizeof digest);
		memcpy(buf, core.buf, sizeof buf);
		core.Reset();
		return *this;
	}

	void SHA1::Core::AppendData(const void* pData, uint64_t size)
	{
		uint32_t block[16];
		auto ptr = static_cast<const char*>(pData);
		while (true)
		{
			while (bufLength < 64)
			{
				if (size < 1) return;
				size--;
				++bufLength;
				buf[bufLength - 1] = *ptr;
				ptr++;
			}
			BufToBlock(buf, block);
			Transform(digest, block, transforms);
			bufLength = 0;
		}
	}

	void SHA1::Core::Reset()
	{
		digest[0] = 0x67452301;
		digest[1] = 0xefcdab89;
		digest[2] = 0x98badcfe;
		digest[3] = 0x10325476;
		digest[4] = 0xc3d2e1f0;
		transforms = 0;
		bufLength = 0;
	}

	SHA1::Core::hash_type SHA1::Core::Get() const
	{
		char lastBuf[64];
		memcpy(lastBuf, buf, sizeof lastBuf);
		auto lastBufLength = bufLength;
		auto lastTransforms = transforms;
		uint32_t lastDigest[5];
		memcpy(lastDigest, digest, sizeof lastDigest);
		const auto total = (lastTransforms * 64 + lastBufLength) * 8;
		lastBuf[lastBufLength++] = 0x80;
		for (auto i = lastBufLength; i < 64; ++i) lastBuf[i] = 0x00;
		uint32_t lastBlock[16];
		BufToBlock(lastBuf, lastBlock);
		if (lastBufLength > 64 - 8)
		{
			Transform(lastDigest, lastBlock, lastTransforms);
			for (size_t i = 0; i < 16 - 2; i++) lastBlock[i] = 0;
		}
		lastBlock[16 - 1] = static_cast<uint32_t>(total);
		lastBlock[16 - 2] = static_cast<uint32_t>(total >> 32);
		Transform(lastDigest, lastBlock, lastTransforms);
		hash_type res{};
		memcpy(res.HashData, lastDigest, sizeof lastDigest);
		return res;
	}

	SHA1::SHA1()
	{
		core = Core();
	}

	SHA1::SHA1(const void* pData, const uint64_t size)
	{
		core = Core();
		core.AppendData(pData, size);
	}

	SHA1::Core::hash_type SHA1::Get() const
	{
		return core.Get();
	}

	SHA1::SHA1(const SHA1& sha1)
	{
		core = sha1.core;
	}

	SHA1::SHA1(SHA1&& sha1) noexcept : core(sha1.core)
	{
		sha1.core.Reset();
	}

	SHA1& SHA1::operator=(SHA1&& sha1) noexcept
	{
		core = sha1.core;
		sha1.core.Reset();
		return *this;
	}
}
