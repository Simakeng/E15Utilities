#pragma once
#include "Utilities.Stream.h"
/**
 @file
 @brief 通用IO流 StreamWriter 接口定义及实现

 @author 司马坑
 @date 2020/2/12
*/

namespace Utilities
{
	/**
		文本模式写入:
		@code
		FileStream fs = FileStream("a.txt",Type::WriteOnly);
		auto sw = StreamWriter(fs);

		string s = u8"你好世界\n";
		sw.Write(s);
		sw.Write(u8"hello world\n");

		@endcode

		二进制模式写入:
		@code
		FileStream fs = FileStream("a.txt",Type::WriteOnly, false);
		auto sw = StreamWriter(fs);
		sw.Write(125);		//sw.write<int>()
		sw.Write(124.5);	//sw.write<double>()

		@endcode
	*/
	/// <summary>
	/// 流写入器对象
	/// </summary>
	/// <typeparam name="StreamType">实例化的流类型</typeparam>
	template<typename StreamType = Stream>
	class StreamWriter
	{
	private:
		StreamType& rs;
	public:
		StreamWriter(StreamType& stream) : rs(stream) { };
		~StreamWriter() { };
		/// <summary>
		/// 向流中写入数据
		/// </summary>
		template<typename DataType>
		void Write(const DataType& obj)
		{
			static_assert(std::is_pod_v<DataType>, "Object must be pod!");
			rs.Write(sizeof(DataType), &obj);
		}
		/// <summary>
		/// 显式的向流中写入字符串对象
		/// </summary>
		template<typename StringType = u8string>
		void WriteString(const StringType& string)
		{
			auto len = string.size();
			rs.Write(sizeof(string::value_type) * len, string.data());
		}


		/// <summary>
		/// 显式的向流中写入字符串
		/// </summary>
		void WriteString(const char* string)
		{
			auto len = strlen(string);
			rs.Write(len, string);
		}

		/// <summary>
		/// 特化模板
		/// <para>
		/// 对该函数的调用会被转发至 StreamWriter::WriteString()
		/// </para>
		/// </summary>
		template<>
		void Write<u8string>(const u8string& obj)
		{
			WriteString(obj);
		}
		/// <summary>
		/// 特化模板
		/// <para>
		/// 对该函数的调用会被转发至 StreamWriter::WriteString()
		/// </para>
		/// </summary>
		void Write(const char* string)
		{
			WriteString(string);
		}
	};


#ifdef _INC_STDIO
	/**
		使用案例:
		@code
			#include <stdio.h>

			FILE* fp = fopen("a.txt","w");
			auto sw = treamWriter(fp);
			sw.Write("Hello world");
		@endcode
	*/
	/// <summary>
	/// StreamWriter 的 FILE* 特化版本
	/// </summary>
	template<>
	class StreamWriter<FILE*>
	{
	private:
		FILE* rs;
	public:
		StreamWriter(FILE* stream);
		~StreamWriter();

		template<typename DataType>
		void Write(const DataType& obj)
		{
			static_assert(std::is_pod_v<DataType>, "Object must be pod!");
			fwrite(&obj, 1, sizeof(DataType), rs);
		}
		template<typename StringType>
		void WriteString(const StringType& string)
		{
			auto len = string.size();
			fwrite(string.data(), 1, sizeof(string::value_type) * len, rs);
		}

		void WriteString(const char* string);
		template<>
		void Write<u8string>(const u8string& obj);
		void Write(const char* string);
	};

#endif // _INC_STDIO

}