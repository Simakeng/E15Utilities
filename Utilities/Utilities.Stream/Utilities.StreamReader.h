#pragma once
#include "Utilities.Stream.h"
/**
 @file
 @brief 通用IO流 StreamReader 接口定义及实现

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
	class StreamReader
	{
	private:
		StreamType& rs;
	public:
		StreamReader(StreamType& stream) : rs(stream) { };
		~StreamReader() { };
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