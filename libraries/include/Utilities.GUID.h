/**
 @file 
 @brief 代码库通用GUID实现

 这个文件里面是对通用GUID对象的接口定义
 它表示一个全局唯一标识符对象

 全局唯一标识符是一种由算法生成的唯一标识，通常表示成32个16进制数字（0－9，A－F）组成的字符串
 如：{21EC2020-3AEA-1069-A2DD-08002B30309D}，它实质上是一个128位长的二进制整数。GUID一词有时也专指微软对UUID标准的实现。

 @see https://zh.wikipedia.org/wiki/%E5%85%A8%E5%B1%80%E5%94%AF%E4%B8%80%E6%A0%87%E8%AF%86%E7%AC%A6

 @author 司马坑
 @date 2020/2/10
*/

/**
 @defgroup Utilities_GUID Utilities::GUID 通用唯一标识符
 该模块定义了一个全局唯一标识符对象

 全局唯一标识符是一种由算法生成的唯一标识，通常表示成32个16进制数字（0－9，A－F）组成的字符串
 如：{21EC2020-3AEA-1069-A2DD-08002B30309D}，它实质上是一个128位长的二进制整数。GUID一词有时也专指微软对UUID标准的实现。

 @see https://zh.wikipedia.org/wiki/%E5%85%A8%E5%B1%80%E5%94%AF%E4%B8%80%E6%A0%87%E8%AF%86%E7%AC%A6

 具体使用方式以及接口定义请参考 Utilities::GUID

 @{

*/

#pragma once
#include <string>
namespace Utilities
{
	/// <summary>
	/// 全局唯一标识符对象
	/// </summary>
	class GUID
	{
	public:
		GUID() = delete;
		~GUID() = default;
	public:
		/// <summary>
		/// 使用一个std::string对象实例化一个GUID对象
		/// </summary>
		/// <param name="rhs">字符串对象</param>
		GUID(const std::string& rhs);
		/// <summary>
		/// 使用一个字符串数组来初始化一个GUID对象
		/// <para>
		/// 例如: 
		/// <example>
		/// GUID guid = GUID("62b164f2-911f-425f-bef9-6e6d5e9bcc6c");
		/// </example>
		/// </para>
		/// </summary>
		/// <param name="rhs">字符串数组</param>
		GUID(const char* rhs);
		//! 复制构造函数
		GUID(const GUID& rhs);
		//! 移动构造函数
		GUID(GUID&& rhs);
	public:
		//! 复制赋值函数
		GUID& operator=(const std::string& rhs);
		//! 复制赋值函数
		GUID& operator=(const GUID& rhs);
		//! 复制赋值函数
		GUID& operator=(GUID&& rhs);
	public:
		/// <summary>
		/// 判断两个GUID对象是否相等
		/// </summary>
		/// <param name="rhs">等号右边的GUID对象</param>
		/// <returns>bool</returns>
		bool operator==(const GUID& rhs)const;
		/// <summary>
		/// 判断两个GUID对象是否不相等
		/// </summary>
		/// <param name="rhs">不等号右边的GUID对象</param>
		/// <returns>bool</returns>
		bool operator!=(const GUID& rhs)const;
	public:
		//! 字符串转换函数
		operator std::string() const;
		//! 字符串转换函数
		std::string ToString() const;
	private:
		struct
		{
			uint32_t data1 = 0;
			uint16_t data2 = 0;
			uint16_t data3 = 0;
			uint64_t data4 = 0;
		} datas;
		static_assert(sizeof(decltype(datas)) == 16, "GUID data size is not 128bit, please check your compilier settings");
	public:
		/// <summary>
		/// 使用一个内存布局上和GUID等价的对象初始化GUID
		/// <para>
		///		@warning 该对象 <b>必须</b> 是以 4:2:2:8 的形式存储GUID数据
		/// </para>
		/// <typeparam name="T"> 以 4:2:2:8 的形式存储GUID数据的类型 </typeparam>
		/// <param name="rhs">GUID对象</param>
		/// </summary>
		template<typename T>
		GUID(const T& rhs)
		{
			constexpr int avalue = sizeof(decltype(datas));
			static_assert(sizeof(T) == sizeof(decltype(this->datas)), "GUID copy target's size is not match");
			memcpy(&(this->datas), &rhs, sizeof(::Utilities::GUID));
		}
	public:
		/// <summary>
		/// 生成一个随机的GUID对象
		/// </summary>
		/// <returns>随机的GUID对象</returns>
		static GUID New();
		/// <summary>
		/// 全为 “0” 的 GUID 对象
		/// <para>
		/// {00000000-0000-0000-0000-000000000000}
		/// </para>
		/// </summary>
		static const GUID Nil;
	};

	
}

/**
 @}
*/
