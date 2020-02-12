#pragma once
#include "Utilities.Stream.h"
/**
 @file
 @brief ͨ��IO�� StreamWriter �ӿڶ��弰ʵ��

 @author ˾���
 @date 2020/2/12
*/

namespace Utilities
{
	/// <summary>
	/// ����ȡ������
	/// </summary>
	/// <typeparam name="StreamType">ʵ������������</typeparam>
	template<typename StreamType>
	class StreamWriter
	{
	private:
		StreamType& rs;
	public:
		StreamWriter(StreamType& stream) : rs(stream) { };
		~StreamWriter() { };
		/// <summary>
		/// ������д������
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
		/// ������д������
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