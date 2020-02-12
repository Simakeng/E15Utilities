#pragma once
/**
 @file
 @brief 通用IO流 接口定义文件

 @author 司马坑
 @date 2020/2/11
*/


/**
	@defgroup Utilities_Stream Utilities::Stream 通用输入输出流
	该模块对常用IO协议进行封装，以简便的方式提供访问功能

	类列表：
	- 已完成
		- Utilities::Stream 通用IO流
		- Utilities::FileStream 文件流
	- 计划中
		- Utilities::MemoryStream 内存流
		- Utilities::NetworkStream 网络流
		- Utiliteis::StreamWriter 流读取器
		- Utiliteis::StreamReader 流写入器
*/

#include <Utilities.h>

namespace Utilities
{
	/// <summary>
	/// 通用IO流对象
	/// <para>
	/// 该类型不可直接实例化
	/// </para>
	/// </summary>
	class Stream
	{
	public:
		/// <summary>
		/// 流IO类型
		/// </summary>
		enum class Type
		{
			ReadOnly,   //!< 只读流
			WriteOnly,  //!< 只写流
			ReadWrite,  //!< 可读写流
			Unkonwn     //!< 未知流
		};
	public:
		Stream() = delete;
		Stream(const Stream&) = delete;
		Stream& operator=(const Stream&) = delete;
	public:
		Stream(Stream&& rhs) noexcept;
		Stream(const Type& _streamType) noexcept;
		virtual ~Stream();
		/// <summary>
		/// 流对象读取接口
		/// </summary>
		/// <param name="len">要读取的数据长度</param>
		/// <param name="data">数据写入的目标缓冲区</param>
		virtual void Read(size_t len, void* data) = 0;
		/// <summary>
		/// 流对象写入接口
		/// </summary>
		/// <param name="len">要写入的数据长度</param>
		/// <param name="data">数据</param>
		virtual void Write(size_t len,const void* data) = 0;
		virtual void Close() = 0;
	private:
		Type streamType = Type::Unkonwn;
	public:
		inline Type GetStreamType() { return streamType; }
	};
}