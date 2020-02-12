/**
 @file
 @brief 通用IO库 文件流接口定义

 @author 司马坑
 @date 2020/2/11
*/
#pragma once
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
		- Utiliteis::StreamWriter 流读取器
		- Utiliteis::StreamReader 流写入器
	- 计划中
		- Utilities::MemoryStream 内存流
		- Utilities::NetworkStream 网络流
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
		/// <summary>
		/// 关闭流对象 终止通讯
		/// </summary>
		virtual void Close() = 0;
		/// <summary>
		/// 获取当前流的可操作性
		/// </summary>
		/// <returns>可操作 : true | 不可操作 : false</returns>
		virtual bool IsVaild() = 0;
	private:
		Type streamType = Type::Unkonwn;
	public:
		inline Type GetStreamType() { return streamType; }
	};
}

namespace Utilities
{
	/// <summary>
	/// 文件流对象
	/// </summary>
	class FileStream : public Stream
	{
	public:
		/// <summary>
		/// 实例化一个文件流对象以访问文件
		/// </summary>
		/// <param name="fileName">文件名</param>
		/// <param name="ioType">操作类型</param>
		/// <param name="IstextMode">是否以文本模式打开</param>
		FileStream(const wchar_t* fileName, const Type& ioType, bool IstextMode = true);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~FileStream();
	public:
		/// <summary>
		/// 流对象读取接口
		/// </summary>
		/// <param name="len">要读取的数据长度</param>
		/// <param name="data">数据写入的目标缓冲区</param>
		virtual void Read(size_t len, void* data) override;
		/// <summary>
		/// 流对象写入接口
		/// </summary>
		/// <param name="len">要写入的数据长度</param>
		/// <param name="data">数据</param>
		virtual void Write(size_t len, const void* data) override;
		/// <summary>
		/// 关闭流对象
		/// </summary>
		virtual void Close() override;
		/// <summary>
		/// 检测流对象是否可用
		/// </summary>
		virtual bool IsVaild() override;
	public:
		/// <summary>
		/// 获取文件流的长度
		/// </summary>
		/// <returns></returns>
		uint64_t GetLength();
		/// <summary>
		/// 获取当前读取指针的位置
		/// </summary>
		/// <returns></returns>
		uint64_t GetPosition();
		/// <summary>
		/// 设置当前读取指针的位置
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(uint64_t pos);
		/// <summary>
		/// 移动当前读取指针的位置
		/// </summary>
		/// <param name="offset">偏移量</param>
		void Seek(int64_t offset);
	public:
		/// <summary>
		/// 获取由系统维护的文件句柄
		/// <para>
		/// 该返回值在 Winodows 操作系统下即为 File *
		/// </para>
		/// </summary>
		/// <returns></returns>
		Handle GetHandle();
	private:
		Handle handle = nullptr;
	};
}
#pragma once
/**
 @file
 @brief 通用IO流 StreamReader 接口定义及实现

 @author 司马坑
 @date 2020/2/12
*/

namespace Utilities
{
	/**
	
	使用方式：

	@code
		auto sr = StreamReader(stream);
		char ch = sr.read<char>();
		int i = sr.read<int>();
		float f = sr.read<float>();
		double d = sr.read<double>();
	@endcode
	*/
	/// <summary>
	/// 流读取器对象
	/// </summary>
	/// <typeparam name="StreamType">实例化的流类型</typeparam>
	template<typename StreamType>
	class StreamReader
	{
	private:
		StreamType& rs;
	public:
		StreamReader(StreamType& stream) : rs(stream) { };
		~StreamReader() { };
		/**
			例子：
			@code
				auto sr = StreamReader(stream);
				char ch = sr.read<char>();
				int i = sr.read<int>();
				float f = sr.read<float>();
				double d = sr.read<double>();
			@endcode
		*/
		/// <summary>
		/// 从流中读取指定类型的数据并返回
		/// </summary>
		template<typename DataType>
		DataType Read()
		{
			static_assert(std::is_pod_v<DataType>, "Object must be pod!");
			DataType t;
			rs.Read(sizeof(DataType), &t);
			return t;
		}
	};

#ifdef _INC_STDIO
	template<>
	class StreamReader<FILE*>
	{
	private:
		FILE* rs;
	public:
		StreamReader(FILE* stream) : rs(stream) { };
		~StreamReader() {};
		template<typename DataType>
		DataType Read()
		{
			static_assert(std::is_pod_v<DataType>, "Object must be pod!");
			DataType t;
			fread(&t, 1, sizeof(DataType), rs);
			return t;
		}
	};
#endif // _INC_STDIO


}
#pragma once
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
