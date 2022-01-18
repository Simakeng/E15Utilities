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
