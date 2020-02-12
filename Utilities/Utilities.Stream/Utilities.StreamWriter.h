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
	/// <summary>
	/// 流读取器对象
	/// </summary>
	/// <typeparam name="StreamType">实例化的流类型</typeparam>
	template<typename StreamType>
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
	};

#ifdef _INC_STDIO
	template<>
	class StreamWriter<FILE*>
	{
	private:
		FILE* rs;
	public:
		StreamWriter(FILE* stream) : rs(stream) { };
		~StreamWriter() { };
		/// <summary>
		/// 向流中写入数据
		/// </summary>
		template<typename DataType>
		void Write(const DataType& obj)
		{
			static_assert(std::is_pod_v<DataType>, "Object must be pod!");
			fwrite(&obj, 1, sizeof(DataType), rs);
		}
	};
#endif // _INC_STDIO


}