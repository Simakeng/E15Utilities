/**
	@file
	@brief CRC32的实现

	这个文件里面是对CRC32的具体实现

	@see Utilities.Encryption.CRC32.h

	@author iriszero
	@date 2020/2/12
*/

#include "Utilities.Encryption.CRC32.h"

namespace Utilities::Encryption
{
	CRC32::Core::Core()
	{
		Reset();
	}

	CRC32::Core::Core(const Core& core)
	{
		crc = core.crc;
	}

	CRC32::Core::Core(Core&& core) noexcept : crc(core.crc)
	{
		core.Reset();
	}

	CRC32::Core& CRC32::Core::operator=(Core&& core) noexcept
	{
		crc = core.crc;
		core.Reset();
		return *this;
	}

	void CRC32::Core::Reset()
	{
		crc = 0 ^ 0xFFFFFFFF;
	}

	void CRC32::Core::AppendData(const void* pData, const size_t size)
	{
		for (size_t i = 0; i < size; ++i)
			crc = (Table[(crc ^ static_cast<const uint8_t*>(pData)[i]) & 0xFFu] ^ (crc >> 8));
	}

	CRC32::CRC32(const void* pData, const size_t size)
	{
		core.AppendData(pData, size);
	}

	CRC32::Core::hash_type CRC32::Get() const
	{
		return core.Get();
	}

	CRC32& CRC32::operator=(CRC32&& crc32) noexcept
	{
		core = crc32.core;
		crc32.core.Reset();
		return *this;
	}

	CRC32::CRC32(CRC32&& crc32) noexcept
	{
		core = crc32.core;
		crc32.core.Reset();
	}

	CRC32::CRC32(const CRC32& crc32)
	{
		core = crc32.core;
	}

	CRC32::CRC32()
	{
		core = Core();
	}
}
