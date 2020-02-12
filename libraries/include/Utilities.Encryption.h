/**
	@file
	@brief CRC32的实现

	CRC32是一种散列函数
	这个文件里面是对SHA1的接口定义

	循环冗余校验是一种根据网络数据包或电脑文件等数据产生简短固定位数校验码的一种散列函数，主要用来检测或校验数据传输或者保存后可能出现的错误。

	@see https://en.wikipedia.org/wiki/Cyclic_redundancy_check

	@author iriszero
	@date 2020/2/12
*/


#pragma once
#include <numeric>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <cctype>

static auto GenerateTable()
{
	static uint32_t table[256] = { 0 };
	for (auto i = 0; i < 256; ++i)
	{
		auto checksum = static_cast<std::uint32_t>(i);
		for (auto j = 0; j < 8; ++j) checksum = (checksum >> 1) ^ (checksum & 0x1 ? 0xEDB88320 : 0);
		table[i] = checksum;
	}
	return table;
}

static auto const Table = GenerateTable();

namespace Utilities::Encryption
{
	/// <summary>
	///		CRC32类
	/// </summary>
	class CRC32
	{
	public:
		/// <summary>
		///		CRC32核心类
		/// </summary>
		class Core
		{
			uint32_t crc{};
		public:
			/// <summary>
			///		CRC32 hash类
			/// </summary>
			/// <typeparam name="HashType">数据类型(4字节)</typeparam>
			template <typename HashType = uint32_t>
			class Hash
			{
			public:
				//! hash数据
				HashType HashData;
				static_assert(sizeof(HashType) == sizeof(uint32_t), "hash data size is not 4 bytes");

				Hash() = default;
				~Hash() = default;

				//! 复制构造函数
				Hash(const Hash& hash)
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					HashData = hash.HashData;
				}

				//! 移动构造函数
				Hash(Hash&& hash) noexcept
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					HashData = hash.HashData;
					hash.HashData = 0;
				}

				//! 复制赋值函数
				Hash& operator=(const Hash& hash)
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					HashData = hash.HashData;
					return *this;
				}

				//! 移动赋值函数
				Hash& operator=(Hash&& hash) noexcept
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					HashData = hash.HashData;
					hash.HashData = 0;
					return *this;
				}

				/// <summary>
				///	字符串转换操作符
				///	<returns>
				///	例如: "414fa339"
				///	</returns>
				/// </summary>
				explicit operator std::string()
				{
					std::ostringstream res;
					res << std::hex << std::setfill('0') << std::setw(8) << HashData;
					return res.str();
				}

				/// <summary>
				///	字符串转换函数
				///	<returns>
				///	例如: "414fa339"
				///	</returns>
				/// </summary>
				std::string ToString()
				{
					return std::string(*this);
				}

				//! 判断两个hash字符串是否相等
				bool operator==(const std::string& hashStr)
				{
					auto const str = ToString();
					return str.length() == hashStr.length() && std::equal(
						str.begin(), str.end(), hashStr.begin(), hashStr.end(), [](auto a, auto b)
						{
							return std::tolower(a) == std::tolower(b);
						});
				}

				//! 判断两个hash字符串是否不相等
				bool operator!=(const std::string& hashStr)
				{
					return !(*this == hashStr);
				}

				//! 判断两个hash对象是否相等
				bool operator==(const Hash<HashType>& hash)
				{
					return hash.HashData == HashData;
				}

				//! 判断两个hash对象是否不相等
				bool operator!=(const Hash<HashType>& hash)
				{
					return !(*this == hash);
				}
			};

			using hash_value_type = uint32_t;
			using hash_type = Hash<hash_value_type>;

			Core();

			~Core() = default;

			//! 复制构造函数
			Core(const Core& core);

			//! 移动构造函数
			Core(Core&& core) noexcept;

			//! 复制赋值函数
			Core& operator=(const Core& core) = default;

			//! 移动赋值函数
			Core& operator=(Core&& core) noexcept;

			//! 重置
			void Reset();

			/// <summary>添加数据</summary>
			/// <param name="pData">要添加的数据的地址</param>
			/// <param name="size">要添加的数据的长度</param>
			void AppendData(const void* pData, size_t size);

			/// <summary>添加数据</summary>
			template <typename _it>
			void AppendData(const _it& begin, const _it& end)
			{
				crc = std::accumulate(begin, end, crc, [](const uint32_t checksum, const uint8_t value)
					{
						return (Table[(checksum ^ value) & 0xFFu] ^ (checksum >> 8));
					});
			}

			/// <summary>添加数据</summary>
			template <typename _cont>
			void AppendData(const _cont& container)
			{
				AppendData(container.begin(), container.end());
			}

			/// <summary>获取结果</summary>
			[[nodiscard]] hash_type Get() const
			{
				hash_type res;
				res.HashData = 0xFFFFFFFF ^ crc;
				return res;
			}
		};

		CRC32();

		~CRC32() = default;

		//! 复制构造函数
		CRC32(const CRC32& crc32);

		//! 移动构造函数
		CRC32(CRC32&& crc32) noexcept;

		//! 复制赋值函数
		CRC32& operator=(const CRC32& crc32) = default;

		//移动赋值函数
		CRC32& operator=(CRC32&& crc32) noexcept;

		/// <summary>添加数据</summary>
		/// <param name="pData">要添加的数据的地址</param>
		/// <param name="size">要添加的数据的长度</param>
		CRC32(const void* pData, size_t size);

		/// <summary>添加数据</summary>
		template <typename _cont>
		explicit CRC32(const _cont& container)
		{
			core = Core();
			core.AppendData(container);
		}

		/// <summary>添加数据</summary>
		template <typename _it>
		CRC32(const _it& begin, const _it& end)
		{
			core = Core();
			core.AppendData(begin, end);
		}

		/// <summary>获取结果</summary>
		[[nodiscard]] Core::hash_type Get() const;

		Core core;
	};
}
/**
	@file
	@brief SHA1的实现

	SHA1是一种密码散列函数
	这个文件里面是对SHA1的接口定义

	SHA-1可以生成一个被称为消息摘要的160位（20字节）散列值，散列值通常的呈现形式为40个十六进制数。

	@see https://en.wikipedia.org/wiki/SHA-1

	@author iriszero
	@date 2020/2/11
*/

#pragma once
#include <string>
#include <numeric>
#include <algorithm>
#include <locale>
#include <iomanip>
#include <sstream>

namespace Utilities::Encryption
{
	/// <summary>
	///		SHA1类
	/// </summary>
	class SHA1
	{
	public:
		/// <summary>
		///		SHA1核心类
		/// </summary>
		class Core
		{
		public:
			/// <summary>
			///		SHA1 hash类
			/// </summary>
			/// <typeparam name="HashType">数据类型(20字节)</typeparam>
			template <typename HashType = uint32_t[5]>
			class Hash
			{
			public:
				//! hash数据
				HashType HashData;
				static_assert(sizeof(HashType) == sizeof(uint8_t[20]), "hash data size is not 20 bytes");

				Hash() = default;
				~Hash() = default;

				//! 复制构造函数
				Hash(const Hash& hash)
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					memcpy(HashData, hash.HashData, sizeof HashData);
				}

				//! 移动构造函数
				Hash(Hash&& hash) noexcept
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					memcpy(HashData, hash.HashData, sizeof HashData);
					memset(hash.HashData, 0, sizeof hash.HashData);
				}

				//! 复制赋值函数
				Hash& operator=(const Hash& hash)
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					memcpy(HashData, hash.HashData, sizeof HashData);
					return *this;
				}

				//! 移动赋值函数
				Hash& operator=(Hash&& hash) noexcept
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					memcpy(HashData, hash.HashData, sizeof HashData);
					memset(hash.HashData, 0, sizeof hash.HashData);
					return *this;
				}

				/// <summary>
				///	字符串转换操作符
				///	<returns>
				///	例如: "da39a3ee5e6b4b0d3255bfef95601890afd80709"
				///	</returns>
				/// </summary>
				explicit operator std::string()
				{
					uint32_t data[5] = { 0 };
					memcpy(data, HashData, sizeof(uint32_t[5]));
					std::ostringstream result;
					for (const auto& i : data) result << std::hex << std::setfill('0') << std::setw(8) << i;
					return result.str();
				}

				/// <summary>
				///	字符串转换函数
				///	<returns>
				///	例如: "da39a3ee5e6b4b0d3255bfef95601890afd80709"
				///	</returns>
				/// </summary>
				std::string ToString()
				{
					return std::string(*this);
				}

				//! 判断两个hash字符串是否相等
				bool operator==(const std::string& hashStr)
				{
					auto str = ToString();
					return str.length() == hashStr.length() && std::equal(
						str.begin(), str.end(), hashStr.begin(), hashStr.end(), [](auto a, auto b)
						{
							return std::tolower(a) == std::tolower(b);
						});
				}

				//! 判断两个hash字符串是否不相等
				bool operator!=(const std::string& hashStr)
				{
					return !(*this == hashStr);
				}

				//! 判断两个hash对象是否相等
				bool operator==(const Hash<HashType>& hash)
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					return memcmp(HashData, hash.HashData, sizeof(HashData)) == 0 ? true : false;
				}

				//! 判断两个hash对象是否不相等
				bool operator!=(const Hash<HashType>& hash)
				{
					static_assert(sizeof(HashData) == sizeof(hash.HashData), "hash data size is not match");
					return !(*this == hash);
				}
			};

			using hash_value_type = uint32_t[5];
			using hash_type = Hash<hash_value_type>;

			Core();
			~Core() = default;

			//! 复制构造函数
			Core(const Core& core);

			//! 移动构造函数
			Core(Core&& core) noexcept;

			//! 复制赋值函数
			Core& operator=(const Core& core);

			//! 移动赋值函数
			Core& operator=(Core&& core) noexcept;

			/// <summary>添加数据</summary>
			/// <param name="pData">要添加的数据的地址</param>
			/// <param name="size">要添加的数据的长度</param>
			void AppendData(const void* pData, uint64_t size);

			/// <summary>添加数据</summary>
			template <typename _it>
			void AppendData(const _it& begin, const _it& end)
			{
				auto size = std::distance(begin, end);
				auto temp = new uint8_t[size];
				std::for_each(begin, end, [&, i = 0](auto x) mutable { temp[i++] = x; });
				AppendData(static_cast<void*>(temp), size);
				delete[] temp;
			}

			/// <summary>添加数据</summary>
			template <typename _cont>
			void AppendData(const _cont& container)
			{
				AppendData(container.begin(), container.end());
			}

			/// <summary>重置</summary>
			void Reset();

			/// <summary>获取结果</summary>
			[[nodiscard]] hash_type Get() const;

		private:
			uint32_t digest[5]{};
			uint64_t transforms{};
			char buf[64]{};
			uint64_t bufLength{};
		};

		SHA1();
		~SHA1() = default;

		/// <summary>添加数据</summary>
		/// <param name="pData">要添加的数据的地址</param>
		/// <param name="size">要添加的数据的长度</param>
		SHA1(const void* pData, uint64_t size);

		/// <summary>获取结果</summary>
		[[nodiscard]] Core::hash_type Get() const;

		/// <summary>添加数据</summary>
		template <typename _it>
		SHA1(const _it& begin, const _it& end)
		{
			core = Core();
			core.AppendData(begin, end);
		}

		/// <summary>添加数据</summary>
		template <typename _cont>
		explicit SHA1(const _cont& container)
		{
			core = Core();
			core.AppendData(container);
		}

		//复制构造函数
		SHA1(const SHA1& sha1);

		//移动构造函数
		SHA1(SHA1&& sha1) noexcept;

		//复制赋值函数
		SHA1& operator=(const SHA1& sha1) = default;

		//移动赋值函数
		SHA1& operator=(SHA1&& sha1) noexcept;

		Core core;
	};
}
